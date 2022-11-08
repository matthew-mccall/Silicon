project(SiliconHeaders)

set(SRCS Silicon/Silicon.hpp Silicon/Log.hpp Silicon/Allocator.hpp Silicon/Types.hpp Silicon/Event.hpp Silicon/Node.hpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

add_library(${PROJECT_NAME} INTERFACE ${SRC})
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})