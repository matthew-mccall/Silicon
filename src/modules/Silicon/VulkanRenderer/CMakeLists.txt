project(VulkanRenderer)

include(../../add_module.cmake)
add_module(${PROJECT_NAME} VulkanRenderer.cpp VulkanRenderer.hpp)