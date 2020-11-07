#include "mode_calc_center.hpp"
#include <mill/traj.hpp>
#include <mill/math/Vector.hpp>
#include <mill/util/cmdarg.hpp>
#include <toml/toml.hpp>

namespace mill
{
const char* mode_calc_center_usage() noexcept
{
    return "usage: mill calc center [filename]\n"
           "calculate the geometric center (it does not consider the weight)";
}

int mode_calc_center(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc center: too few arguments.");
        log::error(mode_calc_center_usage());
        return 1;
    }

    const auto out = pop_argument<std::string>(args, "output")
        .value_or("mill_center.dat");

    const auto filename = args.front();
    if(filename == "help")
    {
        log::info(mode_calc_center_usage());
        return 0;
    }

    std::size_t idx = 0;
    std::cout << "# frame x y z\n";
    for(const auto& frame : reader(filename))
    {
        Vector<double, 3> center(0.0, 0.0, 0.0);
        for(const auto& p : frame)
        {
            center += p.position();
        }
        center /= static_cast<double>(frame.size());
        std::cout << idx++     << ' ' << center[0] << ' '
                  << center[1] << ' ' << center[2] << '\n';
    }

    return 0;
}


} // mill
