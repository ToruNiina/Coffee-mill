set(TEST_NAMES
    test_string
    test_file_extension
    )

add_definitions("-O3 -std=c++17")

foreach(TEST_NAME ${TEST_NAMES})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach(TEST_NAME)
