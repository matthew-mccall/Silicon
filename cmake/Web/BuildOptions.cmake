if (SI_PLATFORM STREQUAL "Web")
    target_compile_options(${PROJECT_NAME} PUBLIC "-sUSE_SDL=2" "-sUSE_BOOST_HEADERS=1")
    target_link_options(${PROJECT_NAME} PUBLIC "-sUSE_SDL=2" "-sUSE_BOOST_HEADERS=1")
endif()