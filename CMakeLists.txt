project(Coffee-mill)
cmake_minimum_required(VERSION 2.8)
enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
include_directories(${PROJECT_SOURCE_DIR}/mill)

# to run doxygen, run `$make doc`.
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_CONF_FILE "doxyfile")
    set(DOXYGEN_CONF_PATH ${PROJECT_SOURCE_DIR}/${DOXYGEN_CONF_FILE})
    set(DOXYGEN_SOURCE_DIR ${PROJECT_SOURCE_DIR})
    set(DOXYGEN_TARGET "doc")
    set(DOXYGEN_PROJECT_NAME "\"Coffee-mill\"")
    set(DOXYGEN_PROJECT_BRIEF "\"header-only library to analyze the data of CafeMol\"")
    set(DOXYGEN_INPUT_DIRECTORY
        ${PROJECT_SOURCE_DIR}/src       ${PROJECT_SOURCE_DIR}/mill/data
        ${PROJECT_SOURCE_DIR}/mill/math ${PROJECT_SOURCE_DIR}/mill/util)
    set(DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/doc")

    configure_file(${DOXYGEN_CONF_FILE}.in ${DOXYGEN_CONF_PATH})
    add_custom_target(${DOXYGEN_TARGET}
        ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONF_PATH}
        DEPENDS ${DOXYGEN_CONF_PATH})
else(DOXYGEN_FOUND)
    message(WARNING "doxygen binary could not be found.")
endif(DOXYGEN_FOUND)

add_subdirectory(src)
add_subdirectory(tests)
