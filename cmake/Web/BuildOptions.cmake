if (SI_PLATFORM STREQUAL "Web")
    target_compile_options(${PROJECT_NAME} PRIVATE "-sUSE_SDL=2" "-sUSE_BOOST_HEADERS=1")
endif()