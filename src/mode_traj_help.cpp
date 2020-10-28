#include <mill/util/logger.hpp>
#include "mode_traj_help.hpp"
#include "mode_traj_convert.hpp"
#include "mode_traj_extract.hpp"
#include "mode_traj_join.hpp"
#include "mode_traj_split.hpp"
#include "mode_traj_impose.hpp"

namespace mill
{

const char* mode_traj_help_usage() noexcept
{
    return "usage: mill traj [command] [parameters...]\n\n"
           "    avaiable commands\n"
           "    - convert\n"
           "      : convert file format\n"
           "    - extract\n"
           "      : extract a part of trajectory\n"
           "    - join\n"
           "      : concatenate several traj files\n"
           "    - split\n"
           "      : split traj files into several fragments\n"
           "    - impose\n"
           "      : superimpose all the frames onto the initial frame\n"
           "    - help\n"
           "      : prints detailed explanation of each command\n";
}

//! this function forwards the arguments to different modes.
// argv := { "help" or "traj", {args...} }
int mode_traj_help(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::info(mode_traj_help_usage());
        return 0;
    }
    const auto command = args.front();

    using namespace std::literals::string_view_literals; // ""sv
    if(command == "convert")
    {
        return mode_traj_convert({"help"sv});
    }
    if(command == "extract")
    {
        return mode_traj_extract({"help"sv});
    }
    else if(command == "join")
    {
        return mode_traj_join({"help"sv});
    }
    else if(command == "split")
    {
        return mode_traj_split({"help"sv});
    }
    else if(command == "impose")
    {
        return mode_traj_impose({"help"sv});
    }
    if(command == "help")
    {
        log::info(mode_traj_help_usage());
        return 0;
    }
    else
    {
        log::error("mill traj help: unknown command : ", command);
        log::error(mode_traj_help_usage());
        return 1;
    }
}
} // mill
