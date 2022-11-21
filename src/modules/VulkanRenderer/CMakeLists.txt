project(VulkanRenderer)

find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

include(../add_module.cmake)

set(${PROJECT_NAME}_DEPENDENCIES Vulkan::Vulkan Vulkan::shaderc_combined)

add_module(${PROJECT_NAME} VulkanRenderer.cpp include/Silicon/VulkanRenderer/VulkanRenderer.hpp Shader.hpp Shader.cpp)