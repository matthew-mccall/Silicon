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

#include <cstdint>

#include <SDL_vulkan.h>

#include "Silicon/Log.hpp"

#include "PhysicalDevice.hpp"

namespace Si::Vulkan {

PhysicalDevice::PhysicalDevice(vk::PhysicalDevice device, Surface &surface, Vector<RequestableItem> &requestedExtensions)
    : m_physicalDevice(device)
    , m_surface(surface)
{
    Vector<vk::ExtensionProperties> availableDeviceExtensions = m_physicalDevice.enumerateDeviceExtensionProperties<Allocator<vk::ExtensionProperties>>();

    for (const Si::RequestableItem &requestedExtension : requestedExtensions) {
        for (vk::ExtensionProperties availableExtension : availableDeviceExtensions) {
            if (requestedExtension.name == availableExtension.extensionName) {
                if (requestedExtension.required) {
                    m_requiredExtensionsSupported++;
                    m_enabledExtensions.emplace_back(availableExtension.extensionName.begin(), availableExtension.extensionName.end());
                    break;
                }
                m_optionalExtensionsSupported++;
                m_enabledExtensions.emplace_back(availableExtension.extensionName.begin(), availableExtension.extensionName.end());
                break;
            }
        }
    }

    m_queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties<Allocator<vk::QueueFamilyProperties>>();

    unsigned int graphicsQueueFamilyIndex = 0;
    for (const vk::QueueFamilyProperties &queueFamily : m_queueFamilyProperties) {
        if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
            m_graphicsFamilyQueueIndex = graphicsQueueFamilyIndex;
        }
        graphicsQueueFamilyIndex++;
    }

    m_maximumImageResolution = device.getProperties().limits.maxImageDimension2D;

    m_formats = m_physicalDevice.getSurfaceFormatsKHR<Allocator<vk::SurfaceFormatKHR>>(*surface);
    Vector<vk::PresentModeKHR> presentModes = m_physicalDevice.getSurfacePresentModesKHR<Allocator<vk::PresentModeKHR>>(*surface);

    m_presentBestMode = vk::PresentModeKHR::eFifo;

    for (auto &availablePresentMode : presentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            m_presentBestMode = availablePresentMode;
        }
    }
}

unsigned int PhysicalDevice::getRequiredExtensionsSupported() const
{
    return m_requiredExtensionsSupported;
}

unsigned int PhysicalDevice::getOptionalExtensionsSupported() const
{
    return m_optionalExtensionsSupported;
}

const vk::PhysicalDevice &PhysicalDevice::getPhysicalDevice() const
{
    return m_physicalDevice;
}

uint32_t PhysicalDevice::getGraphicsFamilyQueueIndex() const
{
    return m_graphicsFamilyQueueIndex;
}

uint32_t PhysicalDevice::getPresentFamilyQueueIndex()
{
    unsigned int presentQueueFamilyIndex = 0;
    for (const vk::QueueFamilyProperties &queueFamily : m_queueFamilyProperties) {
        if (m_physicalDevice.getSurfaceSupportKHR(presentQueueFamilyIndex, *m_surface) == VK_TRUE) {
            m_presentFamilyQueueIndex = presentQueueFamilyIndex;
            break;
        }
        presentQueueFamilyIndex++;
    }

    return m_presentFamilyQueueIndex;
}

const Vector<std::string> &PhysicalDevice::getEnabledExtensions() const
{
    return m_enabledExtensions;
}

uint32_t PhysicalDevice::getMaximumImageResolution() const
{
    return m_maximumImageResolution;
}
std::optional<PhysicalDevice> PhysicalDevice::getBest(Instance &instance, Surface &surface, Vector<RequestableItem> requestedExtensions)
{
    Vector<vk::PhysicalDevice> physicalDevices = instance->enumeratePhysicalDevices<Allocator<vk::PhysicalDevice>>();

    if (physicalDevices.empty()) {
        Si::Engine::Error("Could not find any supported GPUs!");
        return std::nullopt;
    }

    Vector<PhysicalDevice> sortedPhysicalDevices;
    sortedPhysicalDevices.reserve(physicalDevices.size());

    for (vk::PhysicalDevice &physicalDevice : physicalDevices) {
        sortedPhysicalDevices.emplace_back(physicalDevice, surface, requestedExtensions);
    }

    std::sort(sortedPhysicalDevices.begin(), sortedPhysicalDevices.end(), [](PhysicalDevice &a, PhysicalDevice &b) -> bool {
        if ((a.getPhysicalDevice().getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu) && (b.getPhysicalDevice().getProperties().deviceType != vk::PhysicalDeviceType::eDiscreteGpu)) {
            return true;
        } else if ((a.getPhysicalDevice().getProperties().deviceType != vk::PhysicalDeviceType::eDiscreteGpu) && (b.getPhysicalDevice().getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu)) {
            return false;
        } else {
            if (a.getRequiredExtensionsSupported() > b.getRequiredExtensionsSupported()) {
                return true;
            } else if (a.getRequiredExtensionsSupported() < b.getRequiredExtensionsSupported()) {
                return false;
            } else {
                if (a.getGraphicsFamilyQueueIndex() != -1 && b.getGraphicsFamilyQueueIndex() == -1) {
                    return true;
                } else if (a.getGraphicsFamilyQueueIndex() == -1 && b.getGraphicsFamilyQueueIndex() != -1) {
                    return false;
                } else {
                    if (a.getPresentFamilyQueueIndex() != -1 && b.getPresentFamilyQueueIndex() == -1) {
                        return true;
                    } else if (a.getPresentFamilyQueueIndex() == -1 && b.getPresentFamilyQueueIndex() != -1) {
                        return false;
                    } else {
                        if (a.getOptionalExtensionsSupported() > b.getOptionalExtensionsSupported()) {
                            return true;
                        } else if (a.getOptionalExtensionsSupported() < b.getOptionalExtensionsSupported()) {
                            return false;
                        } else {
                            return a.getMaximumImageResolution() >= b.getMaximumImageResolution();
                        }
                    }
                }
            }
        }
    });

    return sortedPhysicalDevices.front();
}

vk::SurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities()
{
    return m_physicalDevice.getSurfaceCapabilitiesKHR(*m_surface);
    ;
}

Vector<vk::SurfaceFormatKHR> &PhysicalDevice::getFormats()
{
    m_formats = m_physicalDevice.getSurfaceFormatsKHR<Allocator<vk::SurfaceFormatKHR>>(*m_surface);
    return m_formats;
}

vk::SurfaceFormatKHR PhysicalDevice::getBestFormat()
{
    for (const auto &availableFormat : m_formats) {
        if (availableFormat.format == vk::Format::eB8G8R8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }

    return getFormats()[0];
}

vk::PresentModeKHR PhysicalDevice::getBestPresentMode() const
{
    return m_presentBestMode;
}

vk::PhysicalDevice *PhysicalDevice::operator->()
{
    return &m_physicalDevice;
}

PhysicalDevice &PhysicalDevice::operator=(const PhysicalDevice &rhs)
{
    return *this;
}

Surface &PhysicalDevice::getSurface() const
{
    return m_surface;
}

vk::PhysicalDevice &PhysicalDevice::operator*()
{
    return m_physicalDevice;
}

}
