project(SiliconHeaders)

configure_file(Silicon/Config.hpp.in Silicon/Config.hpp)

set(SRCS
        Silicon/Silicon.hpp Silicon/Log.hpp Silicon/Allocator.hpp Silicon/Types.hpp Silicon/Event.hpp Silicon/Node.hpp Silicon/Renderer/Renderer.hpp Silicon/Shader.hpp ${CMAKE_CURRENT_BINARY_DIR}/Silicon/Config.hpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

add_library(${PROJECT_NAME} INTERFACE ${SRC} Silicon/Renderer/Vertex.hpp)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})