if (SI_PLATFORM STREQUAL "Web")
    target_compile_options(${PROJECT_NAME} PUBLIC "-sUSE_SDL=2" "-sUSE_BOOST_HEADERS=1")
    target_link_options(${PROJECT_NAME} PUBLIC "-sUSE_SDL=2" "-sUSE_BOOST_HEADERS=1")

    foreach(MODULE IN LISTS ENGINE_MODULES)
        target_compile_options(${MODULE} PRIVATE "-sUSE_SDL=2" "-sUSE_BOOST_HEADERS=1")
    endforeach()
endif()