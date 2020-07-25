add_executable(mill main.cpp mode_help.cpp
    mode_pdb.cpp mode_pdb_help.cpp mode_pdb_seq.cpp
    mode_ninfo.cpp mode_ninfo_help.cpp mode_ninfo_split.cpp
    )
set_target_properties(mill PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
