if (SI_PLATFORM STREQUAL "Desktop")

    execute_process(
            COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/setup.sh"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libs/boost"
    )
#
#    set(FETCHCONTENT_SOURCE_DIR_BOOST "${CMAKE_CURRENT_SOURCE_DIR}/libs/boost")
#    add_subdirectory(libs/boost-cmake)

    set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs/boost")
    set(Boost_NO_SYSTEM_PATHS ON)
    find_package(Boost REQUIRED)

    if(Boost_FOUND)
        include_directories(${Boost_INCLUDE_DIRS})
    endif()

    add_subdirectory(libs/SDL)
endif()