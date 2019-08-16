set(TEST_NAMES
    test_matrix
    test_vector
    test_matrix_vector
    test_eigen
#     test_dcd
#     test_dcd_extract
    )

foreach(TEST_NAME ${TEST_NAMES})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach(TEST_NAME)

add_subdirectory(util)
