project(Extension)

include(../add_module.cmake)
add_module(${PROJECT_NAME} Extension.cpp Extension.hpp)

target_link_libraries(${PROJECT_NAME} PUBLIC SDL2::SDL2)