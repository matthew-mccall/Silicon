if (SI_PLATFORM STREQUAL "Desktop")

    add_subdirectory(libs/SDL)
    add_subdirectory(src/desktop)

    # ================================ Boost ================================ #

    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        execute_process(
            COMMAND "pwsh" "${CMAKE_CURRENT_SOURCE_DIR}/scripts/setup.ps1"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libs/boost"
        )
    else()
        execute_process(
            COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/setup.sh"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libs/boost"
        )
    endif()

    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/boost")
    set(Boost_NO_SYSTEM_PATHS ON)
    find_package(Boost REQUIRED)

    if (Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
    endif ()

    list(APPEND SI_DEPENDENCIES Boost::boost)

    # ================================ Vulkan ================================ #

    find_package(Vulkan REQUIRED COMPONENTS shaderc_combined glslc)

    add_subdirectory(src/vulkan)
    add_subdirectory(libs/VulkanMemoryAllocator)

    CompileShader(shaders/simple.vert)
    CompileShader(shaders/simple.frag)

    list(APPEND SI_DEPENDENCIES
         Silicon::Desktop
         Silicon::Vulkan)
endif()