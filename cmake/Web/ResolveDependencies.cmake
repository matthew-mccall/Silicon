if (SI_PLATFORM STREQUAL "Web")
    add_subdirectory(src/web)
    list(APPEND SI_DEPENDENCIES
         Silicon::Web
    )
endif()
