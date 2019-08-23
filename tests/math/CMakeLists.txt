set(TEST_NAMES
    test_eigen
    test_matrix
    test_matrix_vector
    test_vector
    )

add_definitions("-O3 -std=c++11")

foreach(TEST_NAME ${TEST_NAMES})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach(TEST_NAME)
