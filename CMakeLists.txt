cmake_minimum_required(VERSION 3.16)

project(Engine)

add_subdirectory(include/)

set(ENGINE_MODULE_INCLUDES "")
set(ENGINE_MODULE_INIT_FUNCS "")
set(ENGINE_MODULE_DEINIT_FUNCS "")
set(ENGINE_MODULES "")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(AddModule)

AddModule(Extension)

configure_file(src/Modules.cpp.in Modules.cpp)

add_library(${PROJECT_NAME} src/Core.cpp ${CMAKE_CURRENT_BINARY_DIR}/Modules.cpp src/Modules.hpp)
target_link_libraries(${PROJECT_NAME} PUBLIC EngineHeaders)
target_link_libraries(${PROJECT_NAME} PRIVATE ${ENGINE_MODULES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src/modules")