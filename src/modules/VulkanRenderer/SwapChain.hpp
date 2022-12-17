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

#ifndef YORK_VULKAN_SWAPCHAIN_HPP
#define YORK_VULKAN_SWAPCHAIN_HPP

#include <vulkan/vulkan.hpp>

#include "Silicon/Desktop/Window.hpp"

#include "Device.hpp"
#include "Handle.hpp"
#include "ImageView.hpp"

namespace Si::Vulkan {

/**
 * Handle wrapper for Vulkan Swapchain
 */
class SwapChain : public Handle<vk::SwapchainKHR>
{
public:
    SwapChain(Device &device, Window &window, Surface &surface);
    [[nodiscard]] const vk::Extent2D &getExtent() const;
    [[nodiscard]] Device &getDevice() const;
    [[nodiscard]] vk::SurfaceFormatKHR getFormat() const;
    Vector<ImageView> &getImageViews();

private:
    bool createImpl() override;
    void destroyImpl() override;

    Device &m_device;
    Window &m_window;
    Surface &m_surface;
    vk::SurfaceFormatKHR m_surfaceFormat;
    vk::PresentModeKHR m_presentMode;
    vk::Extent2D m_extent;
    vk::SurfaceCapabilitiesKHR m_capabilities;
    Vector<vk::Image> m_swapChainImages;
    Vector<ImageView> m_swapChainImageViews;
};

}

#endif // YORK_VULKAN_SWAPCHAIN_HPP
