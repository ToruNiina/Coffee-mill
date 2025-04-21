#include "mode_calc_angle.hpp"

#include <mill/traj.hpp>
#include <mill/math/Vector.hpp>
#include <mill/math/Constants.hpp>
#include <toml.hpp>

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
           "       - output will be printed on stdout.\n"
           "    3. mill calc angle [x y z] [x y z]\n"
           "       - It just calculate angle formed by the two vectors.\n"
           "    4. mill calc angle [input.toml]\n"
           "       ```toml\n"
           "       traj = \"traj1.dcd\"\n"
           "       angles = [\n"
           "           [ [i0: int, j0: int ], [k0: int, l0: int ] ], \n"
           "           [ [i1: int, j1: int ], [k1: int, l1: int ] ], \n"
           "           ... , \n"
           "       ]\n"
           "       ```\n";
}

int mode_calc_angle(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc angle: too few arguments.");
        log::error(mode_calc_angle_usage());
        return 1;
    }

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_calc_angle_usage());
        return 0;
    }

    constexpr double rad_to_deg = 180.0 / constants::pi;

    if(args.size() == 1) // input.toml
    {
        const std::string fname(args.front());
        args.pop_front();

        const auto input  = toml::parse(fname);
        const auto traj   = toml::find<std::string>(input, "traj");
        const auto vecs = toml::find<std::vector<
            std::pair<std::array<std::size_t, 2>, std::array<std::size_t, 2>>
            >>(input, "angles");

        for(const auto& frame : reader(traj))
        {
            for(const auto& [vec1, vec2] : vecs)
            {
                const auto [i, j] = vec1;
                const auto [k, l] = vec2;
                const auto v1 = frame[i].position() - frame[j].position();
                const auto v2 = frame[k].position() - frame[l].position();
                std::cout << std::setprecision(16) << angle(v1, v2) * rad_to_deg << '\n';
            }
        }
    }
    else if(args.size() == 4) // traj.dcd i j k
    {
        const auto i = std::stoi(std::string(args.at(1)));
        const auto j = std::stoi(std::string(args.at(2)));
        const auto k = std::stoi(std::string(args.at(3)));
        for(const auto& frame : reader(fname))
        {
            const auto v1 = frame[i].position() - frame[j].position();
            const auto v2 = frame[k].position() - frame[j].position();
            std::cout << std::setprecision(16) << angle(v1, v2) * rad_to_deg << '\n';
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
            const auto v1 = frame[j].position() - frame[i].position();
            const auto v2 = frame[l].position() - frame[k].position();
            std::cout << std::setprecision(16) << angle(v1, v2) * rad_to_deg << '\n';
        }
    }
    else if(args.size() == 6) // x y z x y z
    {
        Vector<double, 3> v1(std::stod(std::string(fname)),
                             std::stod(std::string(args.at(1))),
                             std::stod(std::string(args.at(2))));
        Vector<double, 3> v2(std::stod(std::string(args.at(3))),
                             std::stod(std::string(args.at(4))),
                             std::stod(std::string(args.at(5))));
        std::cout << std::setprecision(16) << angle(v1, v2) * rad_to_deg << '\n';
    }
    else
    {
        log::error("mill calc angle: too few arguments.");
        log::error(mode_calc_angle_usage());
        return 1;
    }
    return 0;
}


} // mill
