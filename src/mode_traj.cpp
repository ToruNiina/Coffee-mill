#include <mill/util/logger.hpp>
#include "mode_traj.hpp"
#include "mode_traj_help.hpp"
#include "mode_traj_convert.hpp"
#include "mode_traj_extract.hpp"
#include <iostream>

namespace mill
{

// argv := arrayof{ "traj", "command-name", {rests...} }
int mode_traj(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill traj mode: too few arguments");
        mode_traj_help({});
        return 1;
    }

    const auto command = args.front();
    args.pop_front();
    if(command == "convert")
    {
        return mode_traj_convert(std::move(args));
    }
    else if(command == "extract")
    {
        return mode_traj_extract(std::move(args));
    }
    else if(command == "help")
    {
        return mode_traj_help(std::move(args));
    }
    else
    {
        log::error("mill traj mode: unknown command: ", command);
        log::error(mode_traj_help_usage());
        return 1;
    }
}

} // mill
