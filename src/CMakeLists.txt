include_directories("${CMAKE_SOURCE_DIR}")
add_definitions("-std=c++11 -O3 -Wall")

find_library(BOOST_REGEX boost_regex)

add_executable(nsplit
    NinfoSplitter.cpp)

add_executable(seqextr
    SequenceExtractor.cpp)

target_link_libraries(seqextr
    ${BOOST_REGEX})

add_executable(mutator
    CGMutator.cpp)

target_link_libraries(mutator
    ${BOOST_REGEX})

add_executable(rmsdcalc
    RMSDCalculator.cpp)

target_link_libraries(rmsdcalc
    ${BOOST_REGEX})

add_executable(superimpose
    SuperImposer.cpp)

target_link_libraries(superimpose
    ${BOOST_REGEX})

add_executable(nucimpose
    NucImposer.cpp)

target_link_libraries(nucimpose
    ${BOOST_REGEX})

add_executable(nucdnadist
    NucDNAdist.cpp)

add_executable(inertia
    CalcInertia.cpp)
target_link_libraries(inertia
    ${BOOST_REGEX})

add_executable(cginertia
    CGInertia.cpp)
target_link_libraries(cginertia
    ${BOOST_REGEX})

add_executable(symaxis
    CalcAxisSym.cpp)
target_link_libraries(symaxis
    ${BOOST_REGEX})

add_executable(reinterpret
    CylinderReinterpreter.cpp)
target_link_libraries(reinterpret
    ${BOOST_REGEX})

