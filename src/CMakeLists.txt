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

# find_library(BOOST_REGEX boost_regex)

add_executable(mill
    coffee_mill.cpp)

target_link_libraries(mill
    coffeemill)


# add_executable(seqextr
#     SequenceExtractor.cpp)
#
# target_link_libraries(seqextr
#     coffeemill)
#

# add_executable(nsplit
#     NinfoSplitter.cpp)
# add_executable(mutator
#     CGMutator.cpp)
#
# target_link_libraries(mutator
#     ${BOOST_REGEX})
#
# add_executable(rmsdcalc
#     RMSDCalculator.cpp)
#
# target_link_libraries(rmsdcalc
#     ${BOOST_REGEX})
#
# add_executable(superimpose
#     SuperImposer.cpp)
#
# target_link_libraries(superimpose
#     ${BOOST_REGEX})
#
# add_executable(dcdtomovie
#     DCDtoMovie.cpp)
#
# target_link_libraries(dcdtomovie
#     ${BOOST_REGEX})
#
# add_executable(dcdjoin
#     DCDJoiner.cpp)
#
# target_link_libraries(dcdjoin
#     ${BOOST_REGEX})
