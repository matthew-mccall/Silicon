project(VulkanRenderer)

include(../add_module.cmake)

set(${PROJECT_NAME}_DEPENDENCIES Vulkan::Vulkan Vulkan::shaderc_combined Desktop)

add_module(${PROJECT_NAME}
        VulkanRenderer.cpp
        include/Silicon/VulkanRenderer/VulkanRenderer.hpp
        Buffer.hpp
        Buffer.cpp
        CommandPool.hpp
        CommandPool.cpp
        Device.hpp
        Device.cpp
        Fence.hpp
        Fence.cpp
        Framebuffer.hpp
        Framebuffer.cpp
        FrameData.hpp
        FrameData.cpp
        Handle.hpp
        Handle.cpp
        ImageView.hpp
        ImageView.cpp
        Instance.hpp
        Instance.cpp
        PhysicalDevice.hpp
        PhysicalDevice.cpp
        Pipeline.hpp
        Pipeline.cpp
        PipelineLayout.hpp
        PipelineLayout.cpp
        RenderPass.hpp
        RenderPass.cpp
        RequestableItem.hpp
        Semaphore.hpp
        Semaphore.cpp
        Shader.hpp
        Shader.cpp
        Surface.cpp
        Surface.hpp
        SwapChain.cpp
        SwapChain.hpp
        VMA.cpp)