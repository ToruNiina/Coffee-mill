cmake_minimum_required(VERSION 3.8)
project(Coffee-mill)
enable_testing()

if (POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS       OFF)
set(CMAKE_CXX_STANDARD          17)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-O2"        COMPILER_SUPPORTS_O2)
check_cxx_compiler_flag("-Wall"      COMPILER_SUPPORTS_Wall)
check_cxx_compiler_flag("-Wextra"    COMPILER_SUPPORTS_Wextra)
check_cxx_compiler_flag("-Wpedantic" COMPILER_SUPPORTS_Wpedantic)

if(COMPILER_SUPPORTS_O2)
    add_definitions(-O2)
endif()
if(COMPILER_SUPPORTS_Wall)
    add_definitions(-Wall)
endif()
if(COMPILER_SUPPORTS_Wextra)
    add_definitions(-Wextra)
endif()
if(COMPILER_SUPPORTS_Wpedantic)
    add_definitions(-Wpedantic)
endif()

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/extlib)
include_directories(${PROJECT_SOURCE_DIR}/mill)

find_package(Boost)
if(Boost_FOUND)
    message(STATUS "Boost ${Boost_VERSION} found at ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

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
        ${PROJECT_SOURCE_DIR}/src       ${PROJECT_SOURCE_DIR}/mill/dcd
        ${PROJECT_SOURCE_DIR}/mill/pdb  ${PROJECT_SOURCE_DIR}/mill/ninfo
        ${PROJECT_SOURCE_DIR}/mill/xyz  ${PROJECT_SOURCE_DIR}/mill/math
        ${PROJECT_SOURCE_DIR}/mill/util)
    set(DOXYGEN_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/doc")

    configure_file(${DOXYGEN_CONF_FILE}.in ${DOXYGEN_CONF_PATH})
    add_custom_target(${DOXYGEN_TARGET}
        ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONF_PATH}
        DEPENDS ${DOXYGEN_CONF_PATH})
else(DOXYGEN_FOUND)
    message(WARNING "doxygen binary could not be found.")
endif(DOXYGEN_FOUND)

add_subdirectory(src)

# test codes depend on Boost.test
if(Boost_FOUND)
    add_subdirectory(tests)
endif()
