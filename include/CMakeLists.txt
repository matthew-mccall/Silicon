project(SiliconHeaders)

configure_file(Silicon/Config.hpp.in Silicon/Config.hpp)

set(SRCS
    Silicon/Silicon.hpp
    Silicon/Log.hpp
    Silicon/Allocator.hpp
    Silicon/Types.hpp
    Silicon/Event.hpp
    Silicon/Localization.hpp
    Silicon/Node.hpp
    Silicon/Renderer/Renderer.hpp
    Silicon/Shader.hpp
    Silicon/Renderer/Vertex.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/Silicon/Config.hpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

add_library(${PROJECT_NAME} INTERFACE ${SRC})
add_library(Silicon::Headers ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} INTERFACE
                      Microsoft.GSL::GSL
                      spdlog::spdlog)