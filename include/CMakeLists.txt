project(SiliconHeaders)

add_library(${PROJECT_NAME} INTERFACE Silicon/Silicon.hpp Silicon/Log.hpp Silicon/Allocator.hpp Silicon/Types.hpp Silicon/Event.hpp)
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})