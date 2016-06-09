set(CPP_FILES
    InputFileReader.cpp
    PDBAtom.cpp
    PDBReader.cpp
    DCDReader.cpp
    DCDWriter.cpp
    DCDJoiner.cpp
    DCDtoMovie.cpp
    NinfoData.cpp
    NinfoReader.cpp
    NinfoWriter.cpp
    NinfoSplitter.cpp
    )

include_directories("${CMAKE_SOURCE_DIR}/includes/")
add_definitions("-std=c++11 -O3 -Wall")

add_library(coffeemill
    ${CPP_FILES}
    )

add_executable(mill
    coffee_mill.cpp)

target_link_libraries(mill
    coffeemill)
