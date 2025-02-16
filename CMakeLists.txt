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
check_cxx_compiler_flag("-O2"           COMPILER_SUPPORTS_O2)
check_cxx_compiler_flag("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
check_cxx_compiler_flag("-mtune=native" COMPILER_SUPPORTS_MTUNE_NATIVE)
check_cxx_compiler_flag("-Wall"         COMPILER_SUPPORTS_Wall)
check_cxx_compiler_flag("-Wextra"       COMPILER_SUPPORTS_Wextra)
check_cxx_compiler_flag("-Wpedantic"    COMPILER_SUPPORTS_Wpedantic)

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
if(COMPILER_SUPPORTS_MARCH_NATIVE)
    add_definitions(-march=native)
endif()
if(COMPILER_SUPPORTS_MTUNE_NATIVE)
    add_definitions(-mtune=native)
endif()

include(cmake/CPM.cmake)
CPMAddPackage(
    NAME toml11
    GITHUB_REPOSITORY "ToruNiina/toml11"
    VERSION 4.3.0
    OPTIONS "TOML11_PRECOMPILE ON" "CMAKE_CXX_STANDARD 17"
    )

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src)

# tests --------------------------------------------------------------------

find_package(Boost)
if(Boost_FOUND)
    message(STATUS "Boost ${Boost_VERSION} found at ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    add_subdirectory(tests)
endif()
