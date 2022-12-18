// BSD 2-Clause License
//
// Copyright (c) 2022, Matthew McCall
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//
// Created by Matthew McCall on 11/21/22.
//

#ifndef SILICON_RENDERER_HPP
#define SILICON_RENDERER_HPP

#include <memory>
#include <functional>

#include "Silicon/Types.hpp"
#include "Vertex.hpp"

namespace Si {

/**
 * The Renderer class is the base class for all Renderer implementations. It provides a common interface for all Renderer implementations.
 */
class Renderer
{
public:

    /**
     * @brief Registers a renderer implementation with the renderer. This will be called by the modules that provide a renderer implementation.
     *
     * @param renderer The renderer implementation to register.
     */
    static void RegisterRenderer(const std::string &name, std::unique_ptr<Renderer> renderer);

    virtual bool Draw() = 0;

    virtual ~Renderer() = default;

protected:

    virtual void OnResize() = 0;

    Si::Vector<Si::Vertex> m_vertices;


};

}

#endif // SILICON_RENDERER_HPP
