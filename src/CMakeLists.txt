set(FILE_OPERATIONS
    InputFileReader.cpp
    PDBAtom.cpp
    PDBReader.cpp
    DCDReader.cpp
    DCDWriter.cpp
    NinfoData.cpp
    NinfoReader.cpp
    NinfoWriter.cpp
    )

set(CPP_FILES
    DCDJoiner.cpp
    DCDtoMovie.cpp
    NinfoSplitter.cpp
    )

include_directories("${PROJECT_SOURCE_DIR}/includes/")
add_definitions("-std=c++11 -O3 -Wall")

add_library(coffeemill STATIC
    ${FILE_OPERATIONS}
    )

add_executable(mill
    coffee_mill.cpp
    ${CPP_FILES}
    )

target_link_libraries(mill
    coffeemill)
