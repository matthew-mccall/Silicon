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
// Created by Matthew McCall on 1/3/22.
//

#include <array>
#include <utility>

#include "Silicon/Renderer/Vertex.hpp"

#include "Pipeline.hpp"

namespace Si {

template <>
vk::VertexInputBindingDescription Vertex::getBindingDescription()
{
    return {0, sizeof(Vertex), vk::VertexInputRate::eVertex};
}

template <>
std::array<vk::VertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions()
{
    vk::VertexInputAttributeDescription vertexPosition {0, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, position)};
    vk::VertexInputAttributeDescription vertexColor {1, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, color)};

    return {vertexPosition, vertexColor};
}

namespace Vulkan {

    Pipeline::Pipeline(RenderPass &renderPass, Vector<Shader> shaders)
        : m_renderPass(renderPass)
        , m_device(m_renderPass.getDevice())
        , m_pipelineLayout(m_device)
        , m_shaders(std::move(shaders))
    {
        addDependency(m_pipelineLayout);
        addDependency(m_renderPass);

        for (Shader &shader : m_shaders) {
            addDependency(shader);
        }
    }

    void Pipeline::setShaders(Vector<Shader> shaders)
    {
        for (Shader &shader : m_shaders) {
            removeDependency(shader);
        }

        m_shaders = std::move(shaders);

        for (Shader &shader : m_shaders) {
            addDependency(shader);
        }
    }

    bool Pipeline::createImpl()
    {
        Vector<vk::PipelineShaderStageCreateInfo> shaderStages;
        shaderStages.reserve(m_shaders.size());

        vk::ShaderStageFlagBits stage;

        for (Shader &shader : m_shaders) {

            switch (shader.getType()) {
            case Shader::Type::Vertex:
                stage = vk::ShaderStageFlagBits::eVertex;
                break;
            case Shader::Type::Fragment:
                stage = vk::ShaderStageFlagBits::eFragment;
                break;
            case Shader::Type::TessellationControl:
                stage = vk::ShaderStageFlagBits::eTessellationControl;
                break;
            case Shader::Type::TessellationEvaluation:
                stage = vk::ShaderStageFlagBits::eTessellationEvaluation;
                break;
            case Shader::Type::Geometry:
                stage = vk::ShaderStageFlagBits::eGeometry;
                break;
            case Shader::Type::Compute:
                stage = vk::ShaderStageFlagBits::eCompute;
                break;
            case Shader::Type::RTRayGen:
                stage = vk::ShaderStageFlagBits::eRaygenKHR;
                break;
            case Shader::Type::RTAnyHit:
                stage = vk::ShaderStageFlagBits::eAnyHitKHR;
                break;
            case Shader::Type::RTClosestHit:
                stage = vk::ShaderStageFlagBits::eClosestHitKHR;
                break;
            case Shader::Type::RTMiss:
                stage = vk::ShaderStageFlagBits::eMissKHR;
                break;
            case Shader::Type::RTIntersection:
                stage = vk::ShaderStageFlagBits::eIntersectionKHR;
                break;
            case Shader::Type::RTCallable:
                stage = vk::ShaderStageFlagBits::eCallableKHR;

                break;
            }

            shaderStages.push_back({{}, stage, *shader, "main"});
        }

        std::array<vk::VertexInputBindingDescription, 1> vertexBindingDescriptions {Vertex::getBindingDescription<vk::VertexInputBindingDescription>()};
        auto vertexAttributeDescriptions = Vertex::getAttributeDescriptions<vk::VertexInputAttributeDescription>();

        vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo {
            {},
            vertexBindingDescriptions,
            vertexAttributeDescriptions};

        vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo {{}, vk::PrimitiveTopology::eTriangleList, VK_FALSE};

        vk::PipelineViewportStateCreateInfo viewportStateCreateInfo {{}, 1, nullptr, 1, nullptr};

        vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo {
            {},
            VK_FALSE,
            VK_FALSE,
            vk::PolygonMode::eFill,
            vk::CullModeFlagBits::eBack,
            vk::FrontFace::eClockwise,
            VK_FALSE,
            0,
            0,
            0,
            1};

        vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo {{}, vk::SampleCountFlagBits::e1, VK_FALSE, 1};

        vk::PipelineColorBlendAttachmentState colorBlendAttachmentState {
            VK_TRUE,
            vk::BlendFactor::eSrcAlpha,
            vk::BlendFactor::eOneMinusSrcAlpha,
            vk::BlendOp::eAdd,
            vk::BlendFactor::eOne,
            vk::BlendFactor::eZero,
            vk::BlendOp::eAdd,
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA};

        std::array<vk::PipelineColorBlendAttachmentState, 1> colorBlendAttachmentStates {colorBlendAttachmentState};

        vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo {
            {},
            VK_FALSE,
            vk::LogicOp::eCopy,
            colorBlendAttachmentStates,
            {0, 0, 0, 0}};

        std::array<vk::DynamicState, 2> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor /*, vk::DynamicState::eLineWidth */};
        vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo {{}, dynamicStates};

        vk::GraphicsPipelineCreateInfo graphicsPipelineCreateInfo {
            {},
            shaderStages,
            &vertexInputStateCreateInfo,
            &inputAssemblyStateCreateInfo,
            nullptr,
            &viewportStateCreateInfo,
            &rasterizationStateCreateInfo,
            &multisampleStateCreateInfo,
            nullptr,
            &colorBlendStateCreateInfo,
            &dynamicStateCreateInfo,
            *m_pipelineLayout,
            *m_renderPass,
            0};

        m_handle = m_device->createGraphicsPipeline(VK_NULL_HANDLE, graphicsPipelineCreateInfo).value;

        return true;
    }

    void Pipeline::destroyImpl()
    {
        m_device->destroy(m_handle);
    }

}
}