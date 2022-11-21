project(Desktop)

include(../add_module.cmake)
add_module(${PROJECT_NAME} Desktop.cpp include/Silicon/Desktop/Desktop.hpp Window.cpp include/Silicon/Desktop/Window.hpp include/Silicon/Desktop/Event.hpp Event.cpp)