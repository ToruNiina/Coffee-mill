#include "mode_calc.hpp"
#include "mode_calc_help.hpp"
#include "mode_calc_rmsd.hpp"
#include "mode_calc_dist.hpp"
#include "mode_calc_wham.hpp"
#include <mill/util/logger.hpp>

namespace mill
{

int mode_calc(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        log::error("mill calc: too few arguments");
        mode_calc_help(--argument_c, ++argument_v); // {}
        return 1;
    }

    const std::string command(argument_v[1]);
    if(command == "rmsd")
    {
        return mode_calc_rmsd(--argument_c, ++argument_v);
    }
    else if(command == "dist")
    {
        return mode_calc_dist(--argument_c, ++argument_v);
    }
    else if(command == "wham")
    {
        return mode_calc_wham(--argument_c, ++argument_v);
    }
    else if(command == "help")
    {
        return mode_calc_help(--argument_c, ++argument_v);
    }
    else
    {
        log::error("mill calc mode: unknown command: ", command);
        log::error(mode_calc_help_usage());
        return 1;
    }
}
} // mill
