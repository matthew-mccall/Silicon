if (SI_PLATFORM STREQUAL "Web")
#    include(src/web/web.cmake)

    add_subdirectory(src/web)
    list(APPEND SI_DEPENDENCIES
         Silicon::Web
    )
endif()
