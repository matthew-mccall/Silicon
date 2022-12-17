if (SI_PLATFORM STREQUAL "Desktop")

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
#
#    set(FETCHCONTENT_SOURCE_DIR_BOOST "${CMAKE_CURRENT_SOURCE_DIR}/libs/boost")
#    add_subdirectory(libs/boost-cmake)

    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/boost")
    set(Boost_NO_SYSTEM_PATHS ON)
    find_package(Boost REQUIRED)

    if (Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
    endif ()

    find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

    add_subdirectory(libs/SDL)
    add_subdirectory(libs/VulkanMemoryAllocator)
endif()