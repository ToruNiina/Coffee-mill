#include <mill/math/Vector.hpp>
#include <mill/util/logger.hpp>
#include "mode_calc_help.hpp"
#include "mode_calc_rmsd.hpp"
#include "mode_calc_wham.hpp"
#include "mode_calc_dist.hpp"
#include "mode_calc_angle.hpp"
#include "mode_calc_aabb.hpp"
#include "mode_calc_obb.hpp"
#include "mode_calc_center.hpp"
#include "mode_calc_autocorrelation.hpp"

namespace mill
{

const char* mode_calc_help_usage() noexcept
{
    return "usage: mill calc [command] [parameters...]\n\n"
           "    avaiable commands\n"
           "    - rmsd\n"
           "      : calculate RMSD between a reference and frames in trajectory\n"
           "    - wham\n"
           "      : reconstruct free energy surface by WHAM\n"
           "    - dist\n"
           "      : calculate distance from traj file\n"
           "    - angle\n"
           "      : calculate angle from traj file\n"
           "    - aabb\n"
           "      : construct AABB\n"
           "    - obb\n"
           "      : construct OBB using covariances\n"
           "    - center\n"
           "      : calculate geometric center\n"
           "    - autocorrelation\n"
           "      : calculate autocorrelation of data\n"
           "    - help\n"
           "      : print detailed explanation of each command\n";
}

//! this function forwards the arguments to different modes.
int mode_calc_help(std::deque<std::string_view> args)
{
    using namespace std::literals::string_view_literals;
    if(args.empty())
    {
        log::info(mode_calc_help_usage());
        return 0;
    }

    const auto command = args.front();

    if(command == "rmsd")
    {
        return mode_calc_rmsd({"help"sv});
    }
    else if(command == "dist")
    {
        return mode_calc_dist({"help"sv});
    }
    else if(command == "angle")
    {
        return mode_calc_angle({"help"sv});
    }
    else if(command == "wham")
    {
        return mode_calc_wham({"help"sv});
    }
    else if(command == "obb")
    {
        return mode_calc_obb({"help"sv});
    }
    else if(command == "aabb")
    {
        return mode_calc_aabb({"help"sv});
    }
    else if(command == "center")
    {
        return mode_calc_center({"help"sv});
    }
    else if(command == "autocorrelation")
    {
        return mode_calc_autocorrelation({"help"sv});
    }
    else if(command == "help")
    {
        log::info(mode_calc_help_usage());
        return 0;
    }
    else
    {
        log::error("mill calc help: unknown command : ", command);
        log::error(mode_calc_help_usage());
        return 1;
    }
}

} // mill
