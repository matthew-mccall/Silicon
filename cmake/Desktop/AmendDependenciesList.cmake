if (SI_PLATFORM STREQUAL "Desktop")
    list(APPEND SI_DEPENDENCIES Boost::boost)
    list(APPEND SI_DEPENDENCIES SDL2::SDL2-static)
endif()