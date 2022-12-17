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
// Created by Matthew McCall on 1/6/22.
//

#include "RenderPass.hpp"

namespace Si::Vulkan {

RenderPass::RenderPass(Device &device)
    : m_device(device)
{
    addDependency(m_device);
}

bool RenderPass::createImpl()
{
    vk::AttachmentDescription attachmentDescription {
        {},
        m_device.getPhysicalDevice().getBestFormat().format,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR};

    vk::AttachmentReference attachmentReference {
        0,
        vk::ImageLayout::eColorAttachmentOptimal};

    std::array<vk::AttachmentReference, 1> attachmentReferences {attachmentReference};

    vk::SubpassDescription subpassDescription {
        {},
        vk::PipelineBindPoint::eGraphics,
        {},
        attachmentReferences};

    std::array<vk::AttachmentDescription, 1> colorAttachments {attachmentDescription};
    std::array<vk::SubpassDescription, 1> subpasses {subpassDescription};

    vk::SubpassDependency dependency {
        VK_SUBPASS_EXTERNAL,
        0,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::AccessFlagBits::eNoneKHR,
        vk::AccessFlagBits::eColorAttachmentWrite};

    std::array<vk::SubpassDependency, 1> dependencies {dependency};

    vk::RenderPassCreateInfo renderPassCreateInfo {
        {},
        colorAttachments,
        subpassDescription,
        dependencies};

    m_handle = m_device->createRenderPass(renderPassCreateInfo);

    return true;
}

void RenderPass::destroyImpl()
{
    m_device->destroy(m_handle);
}
Device &RenderPass::getDevice()
{
    return m_device;
}

}