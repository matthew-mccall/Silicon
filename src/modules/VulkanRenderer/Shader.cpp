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

#include "shaderc/shaderc.hpp"
#include "vulkan/vulkan.hpp"

#include "Silicon/Log.hpp"
#include "Silicon/Types.hpp"

#include "Shader.hpp"

namespace Si::Vulkan {

Shader::Shader(const std::string &string, Type type)
    : Si::Shader(string, type)
{
    shaderc::Compiler compiler;
    shaderc::CompilationResult<std::uint32_t> result;

    shaderc_shader_kind kind;

    switch (m_type) {
    case Type::Vertex:
        kind = shaderc_shader_kind::shaderc_glsl_vertex_shader;
        break;

    case Type::Fragment:
        kind = shaderc_shader_kind::shaderc_glsl_fragment_shader;
        break;

    default:
        kind = shaderc_shader_kind::shaderc_glsl_infer_from_source;
        break;
    }

    result = compiler.CompileGlslToSpv(string, kind, "");

    if (result.GetCompilationStatus() == shaderc_compilation_status_success) {
        Vector<std::uint32_t> spirv { result.begin(), result.end() };
        vk::ShaderModuleCreateInfo createInfo = { {}, spirv };
//        m_handle = m_device.get()->createShaderModule(createInfo);
        Si::Engine::Debug("Compiled shader!");
//        return true;
    }

    Si::Engine::Error(result.GetErrorMessage());
}

} // Si::Vulkan