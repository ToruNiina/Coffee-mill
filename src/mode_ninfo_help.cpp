#include "mode_ninfo_help.hpp"
#include "mode_ninfo_split.hpp"
#include <mill/util/logger.hpp>

namespace mill
{

const char* mode_ninfo_help_usage() noexcept
{
    return "usage: mill ninfo [command] [parameters...]\n\n"
           "    avaiable commands\n"
           "    - split\n"
           "      : split all-in-one .ninfo files into one-by-one files\n"
           "    - help\n"
           "      : print detailed explanation of each command\n";
}

//! this function forwards the arguments to different modes.
int mode_ninfo_help(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::info(mode_ninfo_help_usage());
        return 0;
    }

    const auto command = args.at(1);

    if(command == "split")
    {
        const char *cmds[2] = {"split", "help"};
        return mode_ninfo_split(2, cmds);
    }
    else
    {
        log::error("mill ninfo help: unknown command : ", command);
        log::error(mode_ninfo_help_usage());
        return 1;
    }
}

} // mill
