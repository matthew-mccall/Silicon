if (SI_PLATFORM STREQUAL "Desktop")
    add_subdirectory(libs/boost-cmake)
    add_subdirectory(libs/SDL)
endif()