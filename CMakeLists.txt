cmake_minimum_required(VERSION 3.16)

project(Silicon)

add_subdirectory(libs/spdlog)
add_subdirectory(libs/boost-cmake)
add_subdirectory(libs/GSL)
add_subdirectory(libs/SDL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(include/)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(SI_DEPENDENCIES
        SiliconHeaders
        Boost::boost
        Microsoft.GSL::GSL
        spdlog::spdlog
        SDL2::SDL2)

include(CTest)
include(AddModule)
include(AddSiliconTest)

AddModule(Extension)

configure_file(src/Modules.cpp.in Modules.cpp)

add_library(${PROJECT_NAME} src/Silicon.cpp ${CMAKE_CURRENT_BINARY_DIR}/Modules.cpp src/Modules.hpp src/Log.cpp src/Event.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${ENGINE_MODULES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SI_DEPENDENCIES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/modules")

AddSiliconTest(EngineInit)

add_subdirectory(editor)