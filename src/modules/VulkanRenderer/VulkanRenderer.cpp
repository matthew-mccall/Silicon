// BSD 2-Clause License
//
// Copyright (c) 2022, Matthew McCall
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//
// Created by Matthew McCall on 11/20/22.
//

#include "Silicon/Event.hpp"
#include "Silicon/Log.hpp"
#include "Silicon/Types.hpp"
#include "Silicon/Renderer/Vertex.hpp"

#include "Silicon/Desktop/Event.hpp"
#include "Silicon/Desktop/Window.hpp"

#include "VulkanRenderer/VulkanRenderer.hpp"

#include "Buffer.hpp"
#include "CommandPool.hpp"
#include "FrameData.hpp"
#include "Framebuffer.hpp"
#include "Pipeline.hpp"
#include "Semaphore.hpp"

class VulkanRendererImpl : public Si::Renderer
{
public:
    explicit VulkanRendererImpl(Si::Window& window)
        : Si::Renderer()
        , m_window(window)
        , m_surface(s_instance, window)
        , m_physicalDevice(*Si::Vulkan::PhysicalDevice::getBest(
              s_instance,
              m_surface,
              {
                  { "VK_KHR_portability_subset", false },
                  { VK_KHR_SWAPCHAIN_EXTENSION_NAME }
              }))
        , m_device(m_physicalDevice)
        , m_swapChain(m_device, window, m_surface)
        , m_renderPass(m_device)
        , m_pipeline(m_renderPass)
        , m_commandPool(m_device)
        , m_vertexBuffer(s_instance, m_device, sizeof(Si::Vertex) * 3)
        , m_resizeHandler([this](const Si::Desktop::Event::WindowResize &event) {
            resize = true;
        })
    {
        m_pipeline.create();
        m_swapChain.create();

        Si::Vector<Si::Vulkan::ImageView>& imageViews = m_swapChain.getImageViews();
        m_maxFrames = imageViews.size();

        vk::CommandBufferAllocateInfo commandBufferAllocateInfo { *m_commandPool, vk::CommandBufferLevel::ePrimary, static_cast<uint32_t>(m_maxFrames) };
        m_commandBuffers = m_device->allocateCommandBuffers<Si::Allocator<vk::CommandBuffer>>(commandBufferAllocateInfo);

        m_fences.reserve(m_maxFrames);
        m_framebuffers.reserve(m_maxFrames);
        m_imageAvailableSemaphores.reserve(m_maxFrames);
        m_renderFinishedSemaphores.reserve(m_maxFrames);

        for (unsigned i = 0; i < m_maxFrames; i++) {
            m_fences.emplace_back(m_device);
            m_fences.back().create();

            m_framebuffers.emplace_back(m_renderPass, imageViews[i], m_swapChain);
            m_framebuffers.back().create();

            m_imageAvailableSemaphores.emplace_back(m_device);
            m_imageAvailableSemaphores.back().create();

            m_renderFinishedSemaphores.emplace_back(m_device);
            m_renderFinishedSemaphores.back().create();
        }
    }

    bool Draw() override
    {
        std::array<vk::Fence, 1> fences = { *m_fences[m_frameIndex] };
        auto waitResult = m_device->waitForFences(fences, VK_TRUE, std::numeric_limits<std::uint64_t>::max());

        auto [result, imageIndex] = m_device->acquireNextImageKHR(*m_swapChain, std::numeric_limits<std::uint64_t>::max(), *m_imageAvailableSemaphores[m_frameIndex], VK_NULL_HANDLE);

        if (result == vk::Result::eErrorOutOfDateKHR) {
            OnResize();
            return false;
        } else if ((result != vk::Result::eSuccess) && (result != vk::Result::eSuboptimalKHR)) {
            Si::Engine::Error("Failed to get next image!");
            return false;
        }

        m_device->resetFences(fences);

        vk::CommandBuffer commandBuffer = m_commandBuffers[m_frameIndex];

        vk::CommandBufferBeginInfo commandBufferBeginInfo { vk::CommandBufferUsageFlagBits::eOneTimeSubmit };
        commandBuffer.begin(commandBufferBeginInfo);

        vk::Viewport viewport {
            0,
            0,
            static_cast<float>(m_swapChain.getExtent().width),
            static_cast<float>(m_swapChain.getExtent().height),
            0,
            1
        };

        vk::Rect2D scissor { { 0, 0 }, m_swapChain.getExtent() };

        commandBuffer.setViewport(0, { viewport });
        commandBuffer.setScissor(0, { scissor });

        vk::ClearValue clearValue { vk::ClearColorValue().setFloat32({ 0, 0, 0, 0 }) };
        vk::RenderPassBeginInfo renderPassBeginInfo { *m_renderPass, *(m_framebuffers[imageIndex]), { { 0, 0 }, m_swapChain.getExtent() }, clearValue };
        commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *m_pipeline);

        std::array<vk::Buffer, 1> vertexBuffers { *m_vertexBuffer };
        std::array<vk::DeviceSize, 1> vertexOffsets { 0 };

        commandBuffer.bindVertexBuffers(0, vertexBuffers, vertexOffsets);

        commandBuffer.draw(m_vertices.size(), 1, 0, 0);

        commandBuffer.endRenderPass();
        commandBuffer.end();

        std::array<vk::Semaphore, 1> waitSemaphores { *m_imageAvailableSemaphores[m_frameIndex] };
        std::array<vk::Semaphore, 1> signalSemaphores { *m_renderFinishedSemaphores[m_frameIndex] };
        std::array<vk::PipelineStageFlags, 1> waitStages { vk::PipelineStageFlagBits::eColorAttachmentOutput };

        std::array<vk::CommandBuffer, 1> commandBuffers = { commandBuffer };

        vk::SubmitInfo submitInfo { waitSemaphores, waitStages, commandBuffers, signalSemaphores };

        m_device.getGraphicsQueue().submit({ submitInfo }, *m_fences[m_frameIndex]);

        std::array<vk::SwapchainKHR, 1> swapChains { *m_swapChain };
        std::array<std::uint32_t, 1> imageIndices { imageIndex };

        vk::PresentInfoKHR presentInfo { signalSemaphores, swapChains, imageIndices };

        auto presentResult = m_device.getPresentQueue().presentKHR(presentInfo);

        if ((presentResult == vk::Result::eErrorOutOfDateKHR) || (presentResult == vk::Result::eSuboptimalKHR) || resize) {
            resize = false;
            OnResize();
        } else if (presentResult != vk::Result::eSuccess) {
            Si::Engine::Error("Failed to get next image!");
            return false;
        }

        ++m_frameIndex %= m_maxFrames;

        return true;
    }

    void OnResize() override
    {
        m_device->waitIdle();
        m_swapChain.create();
        m_renderPass.create();
    }

private:

    Si::Window &m_window;

    static Si::Vulkan::Instance s_instance;

    Si::Vulkan::Surface m_surface;
    Si::Vulkan::PhysicalDevice m_physicalDevice;
    Si::Vulkan::Device m_device;
    Si::Vulkan::SwapChain m_swapChain;
    Si::Vulkan::RenderPass m_renderPass;
    Si::Vulkan::Pipeline m_pipeline;
    Si::Vulkan::CommandPool m_commandPool;
    Si::Vulkan::Buffer m_vertexBuffer;
    
    Si::Vector<Si::Vulkan::Framebuffer> m_framebuffers;
    Si::Vector<Si::Vulkan::Fence> m_fences;
    Si::Vector<Si::Vulkan::Semaphore> m_imageAvailableSemaphores, m_renderFinishedSemaphores;
    Si::Vector<Si::Vulkan::Shader> m_defaultShaders;

    Si::Vector<vk::CommandBuffer> m_commandBuffers;

    Si::Sub<Si::Desktop::Event::WindowResize> m_resizeHandler;

    std::uint32_t m_frameIndex = 0;
    std::uint32_t m_maxFrames = 0;

    bool resize = false;
};

Si::Vulkan::Instance VulkanRendererImpl::s_instance {};

namespace Si::VulkanRenderer {

void Initialize()
{
}

void Deinitialize()
{
}

void Create(Window &window)
{
    Renderer::RegisterRenderer("Si::Vulkan", std::make_unique<VulkanRendererImpl>(window));
}

}