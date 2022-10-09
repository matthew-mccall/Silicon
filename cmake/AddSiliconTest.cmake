function(AddSiliconTest TEST_NAME)
    add_executable(${TEST_NAME} test/${TEST_NAME}/${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} Silicon)
    add_test(
            NAME ${TEST_NAME}
            COMMAND ${TEST_NAME})
endfunction()