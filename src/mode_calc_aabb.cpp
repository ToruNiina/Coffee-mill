#include "mode_calc_aabb.hpp"

#include <mill/math/Vector.hpp>
#include <mill/traj.hpp>
#include <toml.hpp>
#include <numeric>

namespace mill
{

const char* mode_calc_aabb_usage() noexcept
{
    return "usage: mill calc aabb <trajfile>\n"
           "       calculates axis aligned bounding box (aabb) from a set of points.";
}

int mode_calc_aabb(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc aabb: too few arguments.");
        log::error(mode_calc_aabb_usage());
        return 1;
    }

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_calc_aabb_usage());
        return 0;
    }

    for(const auto& frame : reader(fname))
    {
        const auto dmax = std::numeric_limits<double>::max();
        Vector<double, 3> lower( dmax,  dmax,  dmax);
        Vector<double, 3> upper(-dmax, -dmax, -dmax);

        for(const auto& p : frame)
        {
            lower[0] = std::min(lower[0], p.position()[0]);
            lower[1] = std::min(lower[1], p.position()[1]);
            lower[2] = std::min(lower[2], p.position()[2]);

            upper[0] = std::max(upper[0], p.position()[0]);
            upper[1] = std::max(upper[1], p.position()[1]);
            upper[2] = std::max(upper[2], p.position()[2]);
        }
        std::cout << "lower x = " << lower[0]  << " upper x = " << upper[0] << '\n';
        std::cout << "lower y = " << lower[1]  << " upper y = " << upper[1] << '\n';
        std::cout << "lower z = " << lower[2]  << " upper z = " << upper[2] << '\n';
        std::cout << "-------------------------------------------------------\n";
    }
    return 0;
}

} // mill
