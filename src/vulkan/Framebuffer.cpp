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
// Created by Matthew McCall on 1/7/22.
//

#include "Framebuffer.hpp"

namespace Si::Vulkan {

Framebuffer::Framebuffer(RenderPass &renderPass, ImageView &imageView, SwapChain &swapChain)
    : m_renderPass(renderPass)
    , m_imageView(imageView)
    , m_device(m_renderPass.getDevice())
    , m_swapChain(swapChain)
{
    assert(&m_renderPass.getDevice() == &m_imageView.getDevice());

    addDependency(m_renderPass);
    addDependency(m_imageView);
    addDependency(m_swapChain);
}

bool Framebuffer::createImpl()
{
    std::array<vk::ImageView, 1> attachments {*m_imageView};

    vk::FramebufferCreateInfo framebufferCreateInfo {
        {},
        *m_renderPass,
        attachments,
        m_swapChain.getExtent().width,
        m_swapChain.getExtent().height,
        1};

    m_handle = m_device->createFramebuffer(framebufferCreateInfo);

    return true;
}
void Framebuffer::destroyImpl()
{
    m_device->destroy(m_handle);
}

}