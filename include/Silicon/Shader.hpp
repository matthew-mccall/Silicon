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

#ifndef SILICON_SHADER_HPP
#define SILICON_SHADER_HPP

#include <cstdint>
#include <string>

#include "Types.hpp"

namespace Si {
class Shader
{
public:
    /**
     * Represents different types of shaders
     */
    enum class Type {
        Vertex,
        TessellationControl, // Future support planned.
        TessellationEvaluation, // Future support planned.
        Geometry, // Future support planned.
        Fragment,
        Compute, // Future support planned.
        RTRayGen, // Future support planned.
        RTAnyHit, // Future support planned.
        RTClosestHit, // Future support planned.
        RTMiss, // Future support planned.
        RTIntersection, // Future support planned.
        RTCallable // Future support planned.
    };

    Shader(const std::string &, Type type)
        : m_type(type)
    {
    }

    Shader(const Vector<std::uint32_t> &, Type type)
        : m_type(type)
    {
    }

    Type getType() const
    {
        return m_type;
    }

    virtual ~Shader() = default;

protected:
    Type m_type;
};
}

#endif // SILICON_SHADER_HPP
