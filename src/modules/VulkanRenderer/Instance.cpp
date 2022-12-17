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

#include <sstream>

#include <SDL_vulkan.h>

#include "Silicon/Config.hpp"
#include "Silicon/Log.hpp"

#include "Instance.hpp"

namespace {

Si::Vector<vk::LayerProperties> s_availableLayers;
Si::Vector<vk::ExtensionProperties> s_availableExtensions;

VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageFunc(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData, void * /*pUserData*/)
{

    switch (messageSeverity) {

    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        Si::Engine::Trace(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        Si::Engine::Info(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        Si::Engine::Warn(pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        Si::Engine::Error(pCallbackData->pMessage);
        break;
    default:
        Si::Engine::Debug(pCallbackData->pMessage);
        break;
    }

    return false;
}

}

namespace Si::Vulkan {

bool Instance::createImpl()
{
    vk::ApplicationInfo appInfo {"York Engine Client", VK_MAKE_VERSION(1, 0, 0), "York Engine", VK_MAKE_VERSION(1, 0, 0), VK_API_VERSION_1_1};

    Vector<const char *> enabledLayers;
    Vector<const char *> enabledExtensions;
    Vector<InstanceLayer> unavailableLayers;
    Vector<InstanceExtension> unavailableExtensions;

    if constexpr (SI_BUILD_CONFIG == BuildConfig::Debug) {
        requestLayer({"VK_LAYER_KHRONOS_validation"});
        requestExtension({VK_EXT_DEBUG_UTILS_EXTENSION_NAME});
    }

    requestExtension({VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME});

    if (s_availableLayers.empty()) {
        s_availableLayers = vk::enumerateInstanceLayerProperties<Allocator<vk::LayerProperties>>();
    }

    for (InstanceLayer &requestedLayer : m_requestedLayers) {

        bool layerFound = false;

        for (vk::LayerProperties &availableLayer : s_availableLayers) {
            if (requestedLayer.name == availableLayer.layerName) {
                enabledLayers.push_back(availableLayer.layerName);
                layerFound = true;
            }
        }

        if (layerFound)
            continue;

        unavailableLayers.push_back(requestedLayer);
    }

    if (s_availableExtensions.empty()) {
        s_availableExtensions = vk::enumerateInstanceExtensionProperties<Allocator<vk::ExtensionProperties>>();
    }

    Vector<const char *> sdlExtensions;

    {
        // TODO: Remove when SDL updates their API
        SDL_Window *window = SDL_CreateWindow("York Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
        unsigned extensionCount;

        if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr)) {
            Si::Engine::Error(SDL_GetError());
            return false;
        }

        sdlExtensions.resize(extensionCount);

        if (!SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, sdlExtensions.data())) {
            Si::Engine::Error(SDL_GetError());
            return false;
        }

        SDL_DestroyWindow(window);
    }

    for (const char *sdlExtension : sdlExtensions) {
        requestExtension({sdlExtension});
    }

    for (InstanceExtension &requestedExtension : m_requestedExtensions) {

        bool extensionFound = false;

        for (vk::ExtensionProperties &availableExtension : s_availableExtensions) {
            if (requestedExtension.name == availableExtension.extensionName) {
                enabledExtensions.push_back(availableExtension.extensionName);
                extensionFound = true;
            }
        }

        if (extensionFound)
            continue;

        unavailableExtensions.push_back(requestedExtension);
    }

    if (!unavailableLayers.empty()) {
        std::stringstream unavailableRequiredLayersList;
        std::stringstream unavailableOptionalLayersList;

        for (InstanceLayer &unavailableLayer : unavailableLayers) {
            if (unavailableLayer.required) {
                unavailableRequiredLayersList << '\n'
                                              << unavailableLayer.name;
                continue;
            }

            unavailableOptionalLayersList << '\n'
                                          << unavailableLayer.name;
        }

        Si::Engine::Error("The following REQUIRED instance layers were requested but not found!{}", unavailableRequiredLayersList.str());
        Si::Engine::Warn("The following OPTIONAL instance layers were requested but not found!{}", unavailableOptionalLayersList.str());
    }

    if (!unavailableExtensions.empty()) {
        std::stringstream unavailableRequiredExtensionList;
        std::stringstream unavailableOptionalExtensionList;

        for (InstanceExtension &unavailableExtension : unavailableExtensions) {
            if (unavailableExtension.required) {
                unavailableRequiredExtensionList << '\n'
                                                 << unavailableExtension.name;
                continue;
            }

            unavailableOptionalExtensionList << '\n'
                                             << unavailableExtension.name;
        }

        Si::Engine::Error("The following REQUIRED instance extensions were requested but not found!\n{}", unavailableRequiredExtensionList.str());
        Si::Engine::Warn("The following OPTIONAL instance extensions were requested but not found!\n{}", unavailableOptionalExtensionList.str());
    }

    vk::InstanceCreateInfo createInfo {vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR, &appInfo, enabledLayers, enabledExtensions};

    vk::DebugUtilsMessengerCreateInfoEXT messengerCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT()
                                                                   .setMessageSeverity(/* vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | */ vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
                                                                   .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
                                                                   .setPfnUserCallback(debugMessageFunc);

    if constexpr (SI_BUILD_CONFIG == BuildConfig::Debug) {
        createInfo.setPNext(reinterpret_cast<VkDebugUtilsMessengerCreateInfoEXT *>(&messengerCreateInfo));
    }

    m_handle = vk::createInstance(createInfo);

    if constexpr (SI_BUILD_CONFIG == BuildConfig::Debug) {
        debugUtilsMessenger = m_handle.createDebugUtilsMessengerEXT(messengerCreateInfo, nullptr, vk::DispatchLoaderDynamic(static_cast<VkInstance>(m_handle), vkGetInstanceProcAddr));
    }

    return true;
}

void Instance::requestLayer(const InstanceLayer &layer)
{
    m_requestedLayers.push_back(layer);
}

void Instance::requestExtension(const InstanceExtension &extension)
{
    m_requestedExtensions.push_back(extension);
}

void Instance::destroyImpl()
{
    if constexpr (SI_BUILD_CONFIG == BuildConfig::Debug) {
        m_handle.destroy(debugUtilsMessenger, nullptr, vk::DispatchLoaderDynamic(static_cast<VkInstance>(m_handle), vkGetInstanceProcAddr));
    }

    m_handle.destroy();
}

} // namespace Si::graphics
