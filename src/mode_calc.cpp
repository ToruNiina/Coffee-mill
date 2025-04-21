#include "mode_calc.hpp"
#include "mode_calc_help.hpp"
#include "mode_calc_rmsd.hpp"
#include "mode_calc_dist.hpp"
#include "mode_calc_angle.hpp"
#include "mode_calc_dihd.hpp"
#include "mode_calc_wham.hpp"
#include "mode_calc_aabb.hpp"
#include "mode_calc_obb.hpp"
#include "mode_calc_center.hpp"
#include "mode_calc_autocorrelation.hpp"
#include "mode_calc_mean.hpp"
#include "mode_calc_BAR.hpp"
#include "mode_calc_neighbor.hpp"
#include "mode_calc_local.hpp"

#ifdef MILL_WITH_EIGEN
#include "mode_calc_pca.hpp"
#endif

#include <mill/util/logger.hpp>

namespace mill
{

int mode_calc(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc: too few arguments");
        mode_calc_help({});
        return 1;
    }

    const auto command = args.front();
    args.pop_front();
    if(command == "rmsd")
    {
        return mode_calc_rmsd(std::move(args));
    }
    else if(command == "dist")
    {
        return mode_calc_dist(std::move(args));
    }
    else if(command == "angle")
    {
        return mode_calc_angle(std::move(args));
    }
    else if(command == "dihd")
    {
        return mode_calc_dihd(std::move(args));
    }
    else if(command == "wham")
    {
        return mode_calc_wham(std::move(args));
    }
    else if(command == "aabb")
    {
        return mode_calc_aabb(std::move(args));
    }
    else if(command == "obb")
    {
        return mode_calc_obb(std::move(args));
    }
    else if(command == "center")
    {
        return mode_calc_center(std::move(args));
    }
    else if(command == "autocorrelation")
    {
        return mode_calc_autocorrelation(std::move(args));
    }
    else if(command == "mean")
    {
        return mode_calc_mean(std::move(args));
    }
    else if(command == "BAR" || command == "bar")
    {
        return mode_calc_BAR(std::move(args));
    }
#ifdef MILL_WITH_EIGEN
    else if(command == "pca")
    {
        return mode_calc_pca(std::move(args));
    }
#endif// MILL_WITH_EIGEN
    else if(command == "neighbor")
    {
        return mode_calc_neighbor(std::move(args));
    }
    else if(command == "local")
    {
        return mode_calc_local(std::move(args));
    }
    else if(command == "help")
    {
        return mode_calc_help(std::move(args));
    }
    else
    {
        log::error("mill calc mode: unknown command: ", command);
        log::error(mode_calc_help_usage());
        return 1;
    }
}
} // mill
