project(Coffee-mill)
cmake_minimum_required(VERSION 2.8)

# to run doxygen, run `$make doc`.
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_CONF_FILE "doxyfile")
    set(DOXYGEN_CONF_PATH ${PROJECT_SOURCE_DIR}/${DOXYGEN_CONF_FILE})
    set(DOXYGEN_SOURCE_DIR ${PROJECT_SOURCE_DIR})
    set(DOXYGEN_TARGET "doc")
    set(DOXYGEN_PROJECT_NAME "Coffee-mill")
    set(DOXYGEN_PROJECT_BRIEF "command line tool for using cafemol")
    set(DOXYGEN_INPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/src")
    set(DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/doc")

    configure_file(${DOXYGEN_CONF_FILE}.in ${DOXYGEN_CONF_PATH})
    add_custom_target(${DOXYGEN_TARGET}
        ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONF_PATH}
        DEPENDS ${DOXYGEN_CONF_PATH})
else(DOXYGEN_FOUND)
    message(WARNING "doxygen binary could not be found.")
endif(DOXYGEN_FOUND)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory(src)
