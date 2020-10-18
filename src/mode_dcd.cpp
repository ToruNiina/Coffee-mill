#include "mode_dcd.hpp"
#include "mode_dcd_help.hpp"
#include "mode_dcd_info.hpp"
#include <iostream>

namespace mill
{

// argv := arrayof{ "dcd", "command-name", {rests...} }
int mode_dcd(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill dcd mode: too few arguments");
        mode_dcd_help({});
        return 1;
    }

    const auto command = args.front();
    if(command == "impose")
    {
        return mode_dcd_impose(std::move(args));
    }
    else if(command == "info")
    {
        return mode_dcd_info(std::move(args));
    }
    else if(command == "help")
    {
        return mode_dcd_help(std::move(args));
    }
    else
    {
        log::error("mill dcd mode: unknown command: ", command);
        log::error(mode_dcd_help_usage());
        return 1;
    }
}

} // mill
