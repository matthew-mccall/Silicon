project(SiliconHeaders)

configure_file(Silicon/Config.hpp.in Silicon/Config.hpp)

add_library(${PROJECT_NAME} INTERFACE
            Silicon/Silicon.hpp
            Silicon/Log.hpp
            Silicon/Allocator.hpp
            Silicon/Types.hpp
            Silicon/Event.hpp
            Silicon/Localization.hpp
            Silicon/Node.hpp
            Silicon/Renderer/Renderer.hpp
            Silicon/Shader.hpp
            Silicon/Renderer/Vertex.hpp)
add_library(Silicon::Headers ALIAS ${PROJECT_NAME})

get_target_property(SOURCES ${PROJECT_NAME} SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

target_sources(${PROJECT_NAME} INTERFACE
               ${CMAKE_CURRENT_BINARY_DIR}/Silicon/Config.hpp)

target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME} INTERFACE
                      glm::glm
                      Microsoft.GSL::GSL
                      spdlog::spdlog)