project(SiliconVulkan)

add_library(${PROJECT_NAME} OBJECT
            VulkanRenderer.cpp
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

add_library(Silicon::Vulkan ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC
                      Silicon::Headers
                      SDL2::SDL2
                      Vulkan::Vulkan
                      VulkanMemoryAllocator)