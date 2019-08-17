#ifndef COFFEE_MILL_NINFO_MODE
#define COFFEE_MILL_NINFO_MODE
#include <src/mode_ninfo_help.hpp>
#include <src/mode_ninfo_split.hpp>
#include <iostream>

namespace mill
{

template<typename vectorT>
int mode_ninfo(int argument_c, const char** argument_v)
{
    if(argument_c < 2)
    {
        log(log_level::error, "mill ninfo mode: too few arguments\n");
        mode_ninfo_help(--argument_c, ++argument_v);
        return 1;
    }

    const std::string command(argument_v[1]);
    if(command == "split")
    {
        return mode_ninfo_split<vectorT>(--argument_c, ++argument_v);
    }
    else
    {
        log(log_level::error, "mill ninfo mode: unknown command", command, '\n');
        std::cerr << mode_ninfo_help_usage() << std::endl;
        return 1;
    }
}

} // mill
#endif /* COFFEE_MILL_NINFO_MODE */
