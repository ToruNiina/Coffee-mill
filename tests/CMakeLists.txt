find_package(Boost)
if(Boost_FOUND)
    message(STATUS "Boost ${Boost_VERSION} found at ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})

    add_subdirectory(util)
    add_subdirectory(math)
    add_subdirectory(common)
    add_subdirectory(xyz)
else()
    message(FATAL_ERROR "Boost is not found")
endif()


