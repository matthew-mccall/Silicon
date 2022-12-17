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
// Created by Matthew McCall on 12/9/21.
//

#ifndef YORK_VULKAN_PHYSICALDEVICE_HPP
#define YORK_VULKAN_PHYSICALDEVICE_HPP

#include <optional>

#include <vulkan/vulkan.hpp>

#include "Silicon/Types.hpp"

#include "RequestableItem.hpp"
#include "Surface.hpp"

namespace Si::Vulkan {

/**
 * @brief A class representing properties about a physical GPU.
 *
 * Not to be confused with Device. Device actually initializes the GPU and interacts with it. PhysicalDevice gets properties about it.
 */
class PhysicalDevice
{
public:
    /**
     * Creates a class representing information about a specific GPU.
     *
     * @param device The Vulkan handle for the physical device to get the information about.
     * @param requestedExtensions The extensions to check against this device's support for.
     */
    PhysicalDevice(vk::PhysicalDevice device, Surface &surface, Vector<RequestableItem> &requestedExtensions);

    /**
     * Gets the number of requested required extensions supported.
     *
     * @return The number of requested required extensions supported.
     */
    [[nodiscard]] unsigned int getRequiredExtensionsSupported() const;

    /**
     * Gets the number of requested optional extensions supported.
     *
     * @return The number of requested optional extensions supported.
     */
    [[nodiscard]] unsigned int getOptionalExtensionsSupported() const;

    /**
     * Gets the Vulkan handle for the device.
     *
     * @return The Vulkan handle for the device.
     */
    [[nodiscard]] const vk::PhysicalDevice &getPhysicalDevice() const;

    /**
     * Gets the index of the graphics queue of the device.
     *
     * @return The index of the graphics queue of the device.
     */
    [[nodiscard]] uint32_t getGraphicsFamilyQueueIndex() const;

    [[nodiscard]] uint32_t getPresentFamilyQueueIndex();

    /**
     * Gets a list of the requested extensions that are supported by the device
     *
     * @return A list of the requested extensions that are supported by the device
     */
    [[nodiscard]] const Vector<std::string> &getEnabledExtensions() const;

    /**
     * Gets the highest resolution supported by the device.
     *
     * @return The highest resolution supported by the device.
     */
    [[nodiscard]] uint32_t getMaximumImageResolution() const;

    vk::SurfaceCapabilitiesKHR getSurfaceCapabilities();
    Vector<vk::SurfaceFormatKHR> &getFormats();

    [[nodiscard]] vk::SurfaceFormatKHR getBestFormat();
    [[nodiscard]] vk::PresentModeKHR getBestPresentMode() const;

    [[nodiscard]] Surface &getSurface() const;

    /**
     * @brief Returns the best physical device.
     *
     * It gets a list of all physical devices supported by the instance. Then it checks them against
     * requestedExtensions and sorts them by whether it is a discrete GPU, extension compatibility, and supported
     * resolutions.
     *
     * @param instance The instance to get the physical devices from.
     * @param requestedExtensions The extensions to check the devices against.
     * @return
     */
    static std::optional<PhysicalDevice> getBest(Instance &instance, Surface &surface, Vector<RequestableItem> requestedExtensions);

    vk::PhysicalDevice *operator->();
    vk::PhysicalDevice &operator*();
    PhysicalDevice &operator=(const PhysicalDevice &rhs);

private:
    unsigned m_requiredExtensionsSupported = 0;
    unsigned m_optionalExtensionsSupported = 0;

    Surface &m_surface;

    vk::PhysicalDevice m_physicalDevice;
    vk::PresentModeKHR m_presentBestMode;

    std::uint32_t m_graphicsFamilyQueueIndex = -1;
    std::uint32_t m_presentFamilyQueueIndex = -1;
    std::uint32_t m_maximumImageResolution;

    Vector<std::string> m_enabledExtensions;
    Vector<vk::SurfaceFormatKHR> m_formats;
    Vector<vk::QueueFamilyProperties> m_queueFamilyProperties;
};

}

#endif // YORK_VULKAN_PHYSICALDEVICE_HPP
