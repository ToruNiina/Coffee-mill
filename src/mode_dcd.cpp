#include "mode_dcd.hpp"
#include "mode_dcd_help.hpp"
#include "mode_dcd_join.hpp"
#include "mode_dcd_split.hpp"
#include "mode_dcd_extract.hpp"
#include "mode_dcd_impose.hpp"
#include "mode_dcd_convert.hpp"
#include "mode_dcd_calc_theta.hpp"
#include "mode_dcd_info.hpp"
#include <iostream>

namespace mill
{

// argv := arrayof{ "dcd", "command-name", {rests...} }
int mode_dcd(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::error("mill dcd mode: too few arguments");
        mode_dcd_help({});
        return 1;
    }

    const auto command(args.at(1));
    args.pop_front();
    if(command == "join")
    {
        // {"join", {"args"...}}
        return mode_dcd_join(std::move(args));
    }
    else if(command == "extract")
    {
        return mode_dcd_extract(std::move(args));
    }
    else if(command == "split")
    {
        return mode_dcd_split(std::move(args));
    }
    else if(command == "impose")
    {
        return mode_dcd_impose(std::move(args));
    }
    else if(command == "convert")
    {
        return mode_dcd_convert(std::move(args));
    }
    else if(command == "calc_theta")
    {
        return mode_dcd_calc_theta(std::move(args));
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
