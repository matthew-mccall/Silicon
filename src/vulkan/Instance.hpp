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

#if !defined(YORK_VULKAN_INSTANCE_HPP)
#define YORK_VULKAN_INSTANCE_HPP

#include <string>

#include <vulkan/vulkan.hpp>

#include "Silicon/Window.hpp"
#include "Silicon/Types.hpp"

#include "Handle.hpp"
#include "RequestableItem.hpp"

/**
 * The namespace for everything Vulkan related. Engine only. (Except Window)
 */
namespace Si::Vulkan {

using InstanceLayer = RequestableItem;
using InstanceExtension = RequestableItem;

/**
 * A handle for Vulkan instances
 */
class Instance : public Handle<vk::Instance>
{
public:
    /**
     * Creates a Vulkan instance.
     *
     * @param window The window which we will be drawing to.
     */
    explicit Instance() = default;

protected:
    bool createImpl() override;

    /**
     * @brief Requests an instance layer to be used in the creation of the instance.
     *
     * If supported, the instance will be created with the layer.
     *
     * @param layer The layer to be requested and whether it is required.
     */
    void requestLayer(const InstanceLayer &layer);

    /**
     * @brief Requests an instance extension to be used in the creation of the instance.
     *
     * If supported, the instance will be created with the extension.
     *
     * @param extension The extension to be requested and whether it is required.
     */
    void requestExtension(const InstanceExtension &extension);

    void destroyImpl() override;

private:
    vk::DebugUtilsMessengerEXT debugUtilsMessenger = {};
    Vector<InstanceLayer> m_requestedLayers;
    Vector<InstanceExtension> m_requestedExtensions;
};

}

#endif
