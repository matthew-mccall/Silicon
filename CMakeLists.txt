cmake_minimum_required(VERSION 3.16)

if (NOT SI_PLATFORM)
    set(SI_PLATFORM "Desktop")
endif ()

if (SI_PLATFORM STREQUAL "Desktop")
elseif (SI_PLATFORM STREQUAL "Web")
    if (NOT DEFINED ENV{EMSDK})
        message(FATAL_ERROR "Could not find Emscripten! Please set your EMSDK environment variable")
    endif ()

    set(CMAKE_TOOLCHAIN_FILE "$ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
else ()
    message(FATAL_ERROR "Invalid platform specified at SI_PLATFORM! Must either be \"Desktop\" or \"Web\".")
endif ()

project(Silicon VERSION 0.0.1)

message(STATUS "Building Silicon Engine ${PROJECT_VERSION} for ${SI_PLATFORM}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(libs/glm)
add_subdirectory(libs/GSL)
add_subdirectory(libs/spdlog)
add_subdirectory(libs/tinyxml2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(include/)

set(SI_DEPENDENCIES
    SiliconHeaders
    tinyxml2::tinyxml2)

configure_file(src/Modules.cpp.in Modules.cpp)
file(COPY localizations DESTINATION .)

list(APPEND SI_SRC
     ${CMAKE_CURRENT_BINARY_DIR}/Modules.cpp
     src/Silicon.cpp
     src/Modules.hpp
     src/Log.cpp
     src/Event.cpp
     src/Node.cpp
     src/Localization.cpp
     src/Renderer.cpp
     src/Window.cpp)

include(Desktop/ResolveDependencies)
include(Web/ResolveDependencies)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SI_SRC})

add_library(${PROJECT_NAME} ${SI_SRC})
add_library("Silicon::${PROJECT_NAME}" ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PUBLIC ${SI_DEPENDENCIES})
target_include_directories(${PROJECT_NAME} PRIVATE
                           "${CMAKE_CURRENT_SOURCE_DIR}/src"
                           "libs/taskflow")

include(Desktop/BuildOptions)
include(Web/BuildOptions)

add_subdirectory(editor)

if (SI_PLATFORM STREQUAL "Web")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

include(CTest)
include(AddSiliconTest)

AddSiliconTest(EngineInit)
AddSiliconTest(PubSub)
AddSiliconTest(SimpleNodes)