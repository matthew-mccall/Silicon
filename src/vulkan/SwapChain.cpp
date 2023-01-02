/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022 Matthew McCall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//
// Created by Matthew McCall on 12/23/21.
//

#include <SDL_vulkan.h>
#include <array>
#include <cstdint>

#include "SwapChain.hpp"

namespace Si::Vulkan {

SwapChain::SwapChain(Device &device, Window &window, Surface &surface)
    : m_device(device)
    , m_window(window)
    , m_surface(surface)
    , m_presentMode(m_device.getPhysicalDevice().getBestPresentMode())
{
    addDependency(m_device);
    addDependency(m_surface);
}

bool SwapChain::createImpl()
{
    PhysicalDevice &physicalDevice = m_device.getPhysicalDevice();

    m_surfaceFormat = physicalDevice.getBestFormat();
    m_presentMode = physicalDevice.getBestPresentMode();
    m_capabilities = physicalDevice.getSurfaceCapabilities();

    if (m_capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) {
        m_extent = m_capabilities.currentExtent;
    } else {
        int width, height;
        SDL_Vulkan_GetDrawableSize(m_window.GetSDLWindow(), &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, m_capabilities.minImageExtent.width, m_capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, m_capabilities.minImageExtent.height, m_capabilities.maxImageExtent.height);

        m_extent = actualExtent;
    }

    std::uint32_t imageCount = m_capabilities.minImageCount + 1;
    std::uint32_t maxImageCount = m_capabilities.maxImageCount;

    if ((maxImageCount > 0) && (imageCount > maxImageCount)) {
        imageCount = maxImageCount;
    }

    vk::SwapchainCreateInfoKHR createInfo {
        {},
        *m_surface,
        imageCount,
        m_surfaceFormat.format,
        m_surfaceFormat.colorSpace,
        m_extent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment,
        vk::SharingMode::eExclusive,
        0,
        {},
        m_capabilities.currentTransform,
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        m_presentMode,
        VK_TRUE,
        nullptr};

    std::array<std::uint32_t, 2> queueFamilyIndices = {physicalDevice.getGraphicsFamilyQueueIndex(), physicalDevice.getPresentFamilyQueueIndex()};

    if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
        createInfo.setQueueFamilyIndices(queueFamilyIndices);
    }

    m_handle = m_device->createSwapchainKHR(createInfo);
    m_swapChainImages = m_device->getSwapchainImagesKHR<Allocator<vk::Image>>(m_handle);

    m_swapChainImageViews.reserve(m_swapChainImages.size());

    for (vk::Image image : m_swapChainImages) {
        m_swapChainImageViews.emplace_back(m_device, m_surfaceFormat.format, image);
    }

    return true;
}

void SwapChain::destroyImpl()
{
    for (ImageView &imageView : m_swapChainImageViews) {
        imageView.destroy();
    }
    m_swapChainImageViews.clear();

    m_device->destroy(m_handle);
}
const vk::Extent2D &SwapChain::getExtent() const
{
    return m_extent;
}

Device &SwapChain::getDevice() const
{
    return m_device;
}

vk::SurfaceFormatKHR SwapChain::getFormat() const
{
    return m_surfaceFormat;
}

Vector<ImageView> &SwapChain::getImageViews()
{
    return m_swapChainImageViews;
}

}
