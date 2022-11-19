project(Desktop)

include(../../add_module.cmake)
add_module(${PROJECT_NAME} Desktop.cpp Desktop.hpp Window.cpp Window.hpp Event.hpp Event.cpp)