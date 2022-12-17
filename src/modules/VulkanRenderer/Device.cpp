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

#include <algorithm>
#include <set>

#include "Device.hpp"
#include "PhysicalDevice.hpp"

namespace Si::Vulkan {

Device::Device(PhysicalDevice &physicalDevice)
    : m_physicalDevice(physicalDevice)
{
}

bool Device::createImpl()
{
    std::array<float, 1> queuePriorities {1.0};

    std::set<uint32_t> uniqueQueueFamilies = {m_physicalDevice.getGraphicsFamilyQueueIndex(), m_physicalDevice.getPresentFamilyQueueIndex()};
    Vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo {{}, queueFamily, queuePriorities};
        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures physicalDeviceFeatures;

    Vector<const char *> enabledExtensions(m_physicalDevice.getEnabledExtensions().size());

    for (int i = 0; i < enabledExtensions.size(); i++) {
        enabledExtensions[i] = m_physicalDevice.getEnabledExtensions()[i].c_str();
    }

    vk::DeviceCreateInfo createInfo {{}, queueCreateInfos, {}, enabledExtensions, &physicalDeviceFeatures};

    m_handle = m_physicalDevice->createDevice(createInfo);
    m_graphicsQueue = std::make_pair(m_physicalDevice.getGraphicsFamilyQueueIndex(), m_handle.getQueue(m_physicalDevice.getGraphicsFamilyQueueIndex(), 0));
    m_presentQueue = std::make_pair(m_physicalDevice.getPresentFamilyQueueIndex(), m_handle.getQueue(m_physicalDevice.getPresentFamilyQueueIndex(), 0));

    return true;
}

PhysicalDevice &Device::getPhysicalDevice()
{
    return m_physicalDevice;
}

void Device::destroyImpl()
{
    m_handle.destroy();
}

std::uint32_t Device::getGraphicsQueueIndex() const
{
    return m_graphicsQueue.first;
}

vk::Queue Device::getGraphicsQueue() const
{
    return m_graphicsQueue.second;
}

std::uint32_t Device::getPresentQueueIndex() const
{
    return m_presentQueue.first;
}

vk::Queue Device::getPresentQueue() const
{
    return m_presentQueue.second;
}

}
