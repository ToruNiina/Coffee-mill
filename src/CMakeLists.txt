add_executable(mill main.cpp
    mode_help.cpp
    mode_pdb.cpp   mode_pdb_help.cpp   mode_pdb_seq.cpp
    mode_ninfo.cpp mode_ninfo_help.cpp mode_ninfo_split.cpp
    mode_calc.cpp  mode_calc_help.cpp  mode_calc_rmsd.cpp   mode_calc_dist.cpp mode_calc_angle.cpp mode_calc_wham.cpp
    mode_dcd.cpp   mode_dcd_help.cpp
    mode_traj.cpp  mode_traj_help.cpp  mode_traj_convert.cpp mode_traj_extract.cpp mode_traj_join.cpp
    )
set_target_properties(mill PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
