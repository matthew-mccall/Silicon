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
// Created by Matthew McCall on 12/3/21.
//

#ifndef YORK_VULKAN_DEVICE_HPP
#define YORK_VULKAN_DEVICE_HPP

#include <utility>

#include <vulkan/vulkan.hpp>

#include "Silicon/Types.hpp"

#include "Handle.hpp"
#include "Instance.hpp"
#include "PhysicalDevice.hpp"
#include "RequestableItem.hpp"
#include "Surface.hpp"

namespace Si::Vulkan {

/**
 * @brief A convenience type alias for storing a queue and it's index.
 */
using IndexQueuePair = std::pair<std::uint32_t, vk::Queue>;

/**
 * @brief A convenience type alias for storing a device extension name and whether its required.
 */
using DeviceExtension = RequestableItem;

/**
 * @brief A class representing a GPU device. You can request extensions and it will automatically pick an appropriate device accordingly upon create().
 */
class Device : public Handle<vk::Device>
{
public:
    /**
     * @brief Initializes a GPU device
     *
     * @param physicalDevice The Vulkan instance to register the device with;
     */
    explicit Device(PhysicalDevice &physicalDevice);

    /**
     * @brief Gets the native Vulkan handle for the physical device.
     *
     * @return The native Vulkan handle for the physical device.
     */
    [[nodiscard]] PhysicalDevice &getPhysicalDevice();

    /**
     * @brief Gets the index for the graphics queue of the device.
     *
     * @return The index for the graphics queue of the device.
     */
    [[nodiscard]] std::uint32_t getGraphicsQueueIndex() const;

    /**
     * @brief Gets the graphics queue of the device.
     *
     * @return The graphics queue of the device.
     */
    [[nodiscard]] vk::Queue getGraphicsQueue() const;

    /**
     * @brief Gets the index for the present queue of the device.
     *
     * @return The index for the present queue of the device.
     */
    [[nodiscard]] std::uint32_t getPresentQueueIndex() const;

    /**
     * @brief Gets the present queue of the device.
     *
     * @return The present queue of the device.
     */
    [[nodiscard]] vk::Queue getPresentQueue() const;

protected:
    bool createImpl() override;
    void destroyImpl() override;

private:
    PhysicalDevice &m_physicalDevice;

    IndexQueuePair m_graphicsQueue;
    IndexQueuePair m_presentQueue;
};

}

#endif // YORK_VULKAN_DEVICE_HPP
