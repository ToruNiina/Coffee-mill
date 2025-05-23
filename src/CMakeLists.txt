set(MILL_SOURCE_FILES
    "mode_help.cpp"
    "mode_pdb.cpp"   "mode_pdb_help.cpp"   "mode_pdb_seq.cpp"
    "mode_ninfo.cpp" "mode_ninfo_help.cpp" "mode_ninfo_split.cpp"
    "mode_dcd.cpp"   "mode_dcd_help.cpp"   "mode_dcd_info.cpp"
    "mode_calc.cpp"        "mode_calc_help.cpp"  "mode_calc_rmsd.cpp"
    "mode_calc_dist.cpp"   "mode_calc_angle.cpp" "mode_calc_wham.cpp" "mode_calc_obb.cpp"
    "mode_calc_center.cpp" "mode_calc_aabb.cpp"  "mode_calc_autocorrelation.cpp"
    "mode_calc_mean.cpp"   "mode_calc_BAR.cpp"   "mode_calc_neighbor.cpp"
    "mode_calc_local.cpp"  "mode_calc_dihd.cpp"  "mode_calc_pca.cpp"
    "mode_traj.cpp"  "mode_traj_help.cpp"  "mode_traj_convert.cpp"
    "mode_traj_extract.cpp" "mode_traj_split.cpp" "mode_traj_join.cpp"
    "mode_traj_impose.cpp" "mode_traj_rotate.cpp" "mode_traj_translate.cpp"
    "mode_traj_running_average.cpp" "mode_traj_projection.cpp"
    "mode_psf.cpp"   "mode_psf_help.cpp"   "mode_psf_gen.cpp"
    )

# Compilation -----------------------------------------------------------------

add_executable(mill main.cpp ${MILL_SOURCE_FILES})
set_target_properties(mill PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

target_link_libraries(mill PRIVATE toml11::toml11)

# OpenMP ----------------------------------------------------------------------

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP ${OpenMP_CXX_VERSION} found")
    target_link_libraries(mill PRIVATE OpenMP::OpenMP_CXX)
else()
    message(WARNING "OpenMP not found. OpenMP support is disabled.")
endif()

# Eigen dependent modules -----------------------------------------------------

find_package (Eigen3 3.3 NO_MODULE)
if(TARGET Eigen3::Eigen)
    target_compile_definitions(mill PRIVATE "-DMILL_WITH_EIGEN")
    target_link_libraries(mill Eigen3::Eigen)
    message(STATUS "Eigen found. calc pca mode is turned on.")
else()
    message(WARNING "Eigen3 not found. PCA mode is disabled.")
endif()
