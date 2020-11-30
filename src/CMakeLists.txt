add_executable(mill main.cpp
    mode_help.cpp
    mode_pdb.cpp   mode_pdb_help.cpp   mode_pdb_seq.cpp
    mode_ninfo.cpp mode_ninfo_help.cpp mode_ninfo_split.cpp
    mode_dcd.cpp   mode_dcd_help.cpp   mode_dcd_info.cpp
    mode_calc.cpp  mode_calc_help.cpp  mode_calc_rmsd.cpp    mode_calc_dist.cpp    mode_calc_angle.cpp mode_calc_wham.cpp mode_calc_obb.cpp    mode_calc_center.cpp mode_calc_aabb.cpp
    mode_traj.cpp  mode_traj_help.cpp  mode_traj_convert.cpp mode_traj_extract.cpp mode_traj_split.cpp mode_traj_join.cpp mode_traj_impose.cpp mode_traj_rotate.cpp mode_traj_translate.cpp
    mode_psf.cpp   mode_psf_help.cpp   mode_psf_gen.cpp
    )
set_target_properties(mill PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
