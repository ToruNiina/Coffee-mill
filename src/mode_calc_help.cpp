#include <mill/math/Vector.hpp>
#include <mill/util/logger.hpp>
#include "mode_calc_help.hpp"
#include "mode_calc_rmsd.hpp"
#include "mode_calc_dist.hpp"

namespace mill
{

const char* mode_calc_help_usage() noexcept
{
    return "usage: mill calc [command] [parameters...]\n\n"
           "    avaiable commands\n"
           "    - rmsd\n"
           "      : calculate RMSD\n"
           "    - dist\n"
           "      : calculate distance from traj file\n"
           "    - wham\n"
           "      : reconstruct free energy surface by WHAM\n"
           "    - help\n"
           "      : print detailed explanation of each command\n";
}

//! this function forwards the arguments to different modes.
int mode_calc_help(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        log::info(mode_calc_help_usage());
        return 0;
    }

    const std::string command(argument_v[1]);

    if(command == "rmsd")
    {
        const char *cmds[2] = {"rmsd", "help"};
        return mode_calc_rmsd(2, cmds);
    }
    else if(command == "dist")
    {
        const char *cmds[2] = {"dist", "help"};
        return mode_calc_dist(2, cmds);
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
