cmake_minimum_required(VERSION 3.16)

if (NOT SI_PLATFORM)
    set(SI_PLATFORM "Desktop")
endif()

if (SI_PLATFORM STREQUAL "Desktop")
elseif(SI_PLATFORM STREQUAL "Web")
    if (NOT DEFINED ENV{EMSDK})
        message(FATAL_ERROR "Could not find Emscripten! Please set your EMSDK environment variable")
    endif()

    set(CMAKE_TOOLCHAIN_FILE "$ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")

    if (NOT EXISTS ${CMAKE_TOOLCHAIN_FILE})
        message(FATAL_ERROR "Emscripten toolchain file not found!")
    endif()

    set(THREADS_PREFER_PTHREAD_FLAG TRUE)
    set(Threads_FOUND TRUE)
    set(CMAKE_USE_PTHREADS_INIT TRUE)
    set(CMAKE_THREAD_LIBS_INIT "-pthread")
else()
    message(FATAL_ERROR "Invalid platform specified at SI_PLATFORM! Must either be \"Desktop\" or \"Web\".")
endif()

project(Silicon VERSION 0.0.1)

message(STATUS "Building Silicon Engine ${PROJECT_VERSION} for ${SI_PLATFORM}")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(libs/spdlog)
add_subdirectory(libs/GSL)

include(Desktop/ResolveDependencies)
include(Web/ResolveDependencies)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(include/)

set(SI_DEPENDENCIES
        SiliconHeaders
        Microsoft.GSL::GSL
        spdlog::spdlog)

include(Desktop/AmendDependenciesList)
include(Web/AmendDependenciesList)

include(CTest)
include(AddSiliconTest)

include(AddModule)

# AddModule(Extension)
AddModule(Desktop)

configure_file(src/Modules.cpp.in Modules.cpp)

set(SRC ${CMAKE_CURRENT_BINARY_DIR}/Modules.cpp src/Silicon.cpp src/Modules.hpp src/Log.cpp src/Event.cpp src/Node.cpp)

include(Desktop/AmendSourcesList)
include(Web/AmendSourcesList)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRC})

add_library(${PROJECT_NAME} ${SRC})

add_library("Silicon::${PROJECT_NAME}" ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE ${ENGINE_MODULES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${SI_DEPENDENCIES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/modules")

include(Desktop/BuildOptions)
include(Web/BuildOptions)

add_subdirectory(editor)

if (SI_PLATFORM STREQUAL "Web")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

AddSiliconTest(EngineInit)
AddSiliconTest(PubSub)
AddSiliconTest(SimpleNodes)