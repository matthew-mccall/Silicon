cmake_minimum_required(VERSION 3.16)

project(Silicon)

add_subdirectory(libs/spdlog)
add_subdirectory(libs/boost-cmake)
add_subdirectory(libs/GSL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(include/)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(SI_DEPENDENCIES
        SiliconHeaders
        Boost::boost
        Microsoft.GSL::GSL
        spdlog::spdlog)

include(AddModule)

AddModule(Extension)

configure_file(src/Modules.cpp.in Modules.cpp)

add_library(${PROJECT_NAME} src/Silicon.cpp ${CMAKE_CURRENT_BINARY_DIR}/Modules.cpp src/Modules.hpp src/Log.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SI_DEPENDENCIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ENGINE_MODULES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src/modules")

add_subdirectory(editor)