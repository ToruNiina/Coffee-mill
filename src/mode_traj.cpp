#include <mill/util/logger.hpp>
#include "mode_traj.hpp"
#include "mode_traj_help.hpp"
#include "mode_traj_convert.hpp"
#include "mode_traj_extract.hpp"
#include "mode_traj_join.hpp"
#include "mode_traj_split.hpp"
#include "mode_traj_impose.hpp"
#include "mode_traj_rotate.hpp"
#include "mode_traj_translate.hpp"
#include "mode_traj_running_average.hpp"
#include "mode_traj_mean.hpp"
#include "mode_traj_projection.hpp"

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
    else if(command == "join")
    {
        return mode_traj_join(std::move(args));
    }
    else if(command == "split")
    {
        return mode_traj_split(std::move(args));
    }
    else if(command == "impose")
    {
        return mode_traj_impose(std::move(args));
    }
    else if(command == "rotate")
    {
        return mode_traj_rotate(std::move(args));
    }
    else if(command == "translate")
    {
        return mode_traj_translate(std::move(args));
    }
    else if(command == "running_average" || command == "running-average")
    {
        return mode_traj_running_average(std::move(args));
    }
    else if(command == "mean")
    {
        return mode_traj_mean(std::move(args));
    }
    else if(command == "projection")
    {
        return mode_traj_projection(std::move(args));
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
