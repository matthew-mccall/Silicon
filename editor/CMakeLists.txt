cmake_minimum_required(VERSION 3.16)

project(Editor)

add_subdirectory(.. Engine)

add_executable(${PROJECT_NAME} Editor.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)