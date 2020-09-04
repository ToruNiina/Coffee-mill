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
int mode_dcd(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        log::error("mill dcd mode: too few arguments");
        mode_dcd_help({});
        return 1;
    }

    const std::string command(argument_v[1]);
    if(command == "join")
    {
        // {"join", {"args"...}}
        return mode_dcd_join(--argument_c, ++argument_v);
    }
    else if(command == "extract")
    {
        return mode_dcd_extract(--argument_c, ++argument_v);
    }
    else if(command == "split")
    {
        return mode_dcd_split(--argument_c, ++argument_v);
    }
    else if(command == "impose")
    {
        return mode_dcd_impose(--argument_c, ++argument_v);
    }
    else if(command == "convert")
    {
        return mode_dcd_convert(--argument_c, ++argument_v);
    }
    else if(command == "calc_theta")
    {
        return mode_dcd_calc_theta(--argument_c, ++argument_v);
    }
    else if(command == "info")
    {
        return mode_dcd_info(--argument_c, ++argument_v);
    }
    else if(command == "help")
    {
        std::deque<std::string_view> args;
        for(int i=0; i<argument_c; ++i)
        {
            args.emplace_back(argument_v[i]);
        }
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
