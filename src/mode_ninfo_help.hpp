#ifndef COFFEE_MILL_NINFO_HELP_MODE
#define COFFEE_MILL_NINFO_HELP_MODE
#include <iostream>
#include <mill/math/Vector.hpp>
#include <src/mode_ninfo_split.hpp>

namespace mill
{

inline const char* mode_ninfo_help_usage() noexcept
{
    return "usage: mill ninfo [command] [parameters...]\n\n"
           "    avaiable commands\n"
           "    - split\n"
           "      : split all-in-one .ninfo files into one-by-one files\n"
           "    - help\n"
           "      : print detailed explanation of each command\n";
}

//! this function forwards the arguments to different modes.
inline int mode_ninfo_help(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        std::cerr << mode_ninfo_help_usage() << std::endl;
        return 0;
    }

    const std::string command(argument_v[1]);

    if(command == "split")
    {
        const char *cmds[2] = {"split", "help"};
        return mode_ninfo_split<mill::Vector<double, 3>>(2, cmds);
    }
    else
    {
        log::error("mill ninfo help: unknown command : ", command, '\n');
        std::cerr << mode_ninfo_help_usage() << std::endl;
        return 1;
    }
}

} // mill
#endif// COFFEE_MILL_NINFO_HELP_MODE
