#include "mode_calc_angle.hpp"

#include <mill/traj.hpp>
#include <mill/math/Vector.hpp>
#include <toml/toml.hpp>

namespace mill
{
const char* mode_calc_angle_usage() noexcept
{
    return "usage: mill calc angle [options...]\n"
           "    1. mill calc angle traj.dcd [i:size_t] [j:size_t] [k:size_t]\n"
           "       - angle formed by j->i vector and j->k vector will be calculated.\n"
           "       - output will be printed on stdout.\n"
           "    2. mill calc angle traj.dcd [i:size_t] [j:size_t] [k:size_t] [l:size_t]\n"
           "       - angle formed by i->j vector and k->l vector will be calculated.\n"
           "       - NOTE THAT HERE IT CALCULATES THE ANGLE FORMED BY i->j and k->l VECTORS.\n"
           "       - output will be printed on stdout.\n";
}

int mode_calc_angle(int argc, const char** argv)
{
    if(argc < 2)
    {
        log::error("mill calc angle: too few arguments.");
        log::error(mode_calc_angle_usage());
        return 1;
    }

    const std::string fname(argv[1]);
    if(fname == "help")
    {
        log::info(mode_calc_angle_usage());
        return 0;
    }

    if(fname.size() < 5)
    {
        log::error("mill calc angle: unknown file format: ", fname);
        log::error(mode_calc_angle_usage());
        return 1;
    }
    constexpr double rad_to_deg = 180.0 / 3.14159265358979;

    if(argc == 5) // angle traj.dcd i j k
    {
        const auto i = std::atoi(argv[2]);
        const auto j = std::atoi(argv[3]);
        const auto k = std::atoi(argv[4]);
        for(const auto frame : read(fname))
        {
            const auto v1 = frame[i].position() - frame[j].position();
            const auto v2 = frame[k].position() - frame[j].position();
            std::cout << std::setprecision(16) << angle(v1, v2) * rad_to_deg << '\n';
        }
    }
    else if(argc == 6) // angle traj.dcd i j k l
    {
        const auto i = std::atoi(argv[2]);
        const auto j = std::atoi(argv[3]);
        const auto k = std::atoi(argv[4]);
        const auto l = std::atoi(argv[5]);

        for(const auto frame : read(fname))
        {
            const auto v1 = frame[j].position() - frame[i].position();
            const auto v2 = frame[l].position() - frame[k].position();
            std::cout << std::setprecision(16) << angle(v1, v2) * rad_to_deg << '\n';
        }
    }
    return 0;
}


} // mill
