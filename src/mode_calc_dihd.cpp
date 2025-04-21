#include "mode_calc_dihd.hpp"

#include <mill/traj.hpp>
#include <mill/math/Vector.hpp>
#include <mill/math/Constants.hpp>
#include <toml.hpp>

namespace mill
{
const char* mode_calc_dihd_usage() noexcept
{
    return "usage: mill calc dihd [options...]\n"
           "    1. mill calc dihd traj.dcd [i:size_t] [j:size_t] [k:size_t] [l:size_t]\n"
           "    2. mill calc dihd [input.toml]\n"
           "       ```toml\n"
           "       traj = \"traj.dcd\"\n"
           "       dihds = [\n"
           "           [ i0, j0, k0, l0 ], \n"
           "           [ i1, j1, k1, l1 ], \n"
           "           ... , \n"
           "       ]\n"
           "       ```\n";
}

int mode_calc_dihd(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc dihd: too few arguments.");
        log::error(mode_calc_dihd_usage());
        return 1;
    }

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_calc_dihd_usage());
        return 0;
    }

    constexpr double rad_to_deg = 180.0 / constants::pi;

    if(args.size() == 1) // input.toml
    {
        const std::string fname(args.front());
        args.pop_front();

        const auto input  = toml::parse(fname);
        const auto traj   = toml::find<std::string>(input, "traj");
        const auto vecs = toml::find<std::vector<std::array<std::size_t, 4>>>(input, "dihds");

        for(const auto& frame : reader(traj))
        {
            for(const auto& ijkl : vecs)
            {
                const auto [i, j, k, l] = ijkl;
                const auto ri = frame[i].position();
                const auto rj = frame[j].position();
                const auto rk = frame[k].position();
                const auto rl = frame[l].position();

                std::cout << std::setprecision(16) << dihedral(ri, rj, rk, rl) * rad_to_deg << '\n';
            }
        }
    }
    else if(args.size() == 5) // traj.dcd i j k l
    {
        const auto i = std::stoi(std::string(args.at(1)));
        const auto j = std::stoi(std::string(args.at(2)));
        const auto k = std::stoi(std::string(args.at(3)));
        const auto l = std::stoi(std::string(args.at(4)));

        for(const auto& frame : reader(fname))
        {
            const auto ri = frame[i].position();
            const auto rj = frame[j].position();
            const auto rk = frame[k].position();
            const auto rl = frame[l].position();
            std::cout << std::setprecision(16) << dihedral(ri, rj, rk, rl) * rad_to_deg << '\n';
        }
    }
    else
    {
        log::error("mill calc dihd: too few arguments.");
        log::error(mode_calc_dihd_usage());
        return 1;
    }
    return 0;
}


} // mill
