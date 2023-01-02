function(AddSiliconTest TEST_NAME)
    add_executable(${TEST_NAME} test/${TEST_NAME}/${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} Silicon)

    set(SI_TEST_COMMAND ${TEST_NAME})

    if (SI_PLATFORM STREQUAL "Web")
        target_link_options(${TEST_NAME} PRIVATE "--emrun")
        set(SI_TEST_COMMAND emrun --kill_start --kill_exit --verbose --timeout 15 ${TEST_NAME}.html)
    endif()

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${SI_TEST_COMMAND})
endfunction()