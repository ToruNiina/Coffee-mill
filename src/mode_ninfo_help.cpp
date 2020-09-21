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
    if(args.empty())
    {
        log::info(mode_ninfo_help_usage());
        return 0;
    }

    const auto command = args.front();
    if(command == "split")
    {
        using namespace std::literals::string_view_literals;
        return mode_ninfo_split({"split"sv, "help"sv});
    }
    else
    {
        log::error("mill ninfo help: unknown command : ", command);
        log::error(mode_ninfo_help_usage());
        return 1;
    }
}

} // mill
