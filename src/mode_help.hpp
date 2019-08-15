#ifndef COFFEE_MILL_HELP_MODE
#define COFFEE_MILL_HELP_MODE
#include <iostream>
#include <src/mode_dcd_help.hpp>
#include <src/mode_pdb_help.hpp>
#include <src/mode_ninfo_help.hpp>
#include <src/mode_calc_help.hpp>

namespace mill
{

inline const char* main_usage() noexcept
{
    return "Usage: mill [mode] [parameters...]\n"
           "list of modes\n"
           "- pdb\n"
           "- dcd\n"
           "- ninfo\n"
           "- calc\n"
           "- help\n"
           "for more information, try `mill help [mode]`.\n"
           ;
}

//! this function forwards the arguments to different modes.
// argv := { "help", {args...} }
inline int mode_help(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        std::cerr << main_usage() << std::endl;
        return 0;
    }

    const std::string command(argument_v[1]);

    if(command == "pdb")
    {
        return mode_pdb_help(--argument_c, ++argument_v);
    }
    else if(command == "ninfo")
    {
        return mode_ninfo_help(--argument_c, ++argument_v);
    }
    else if(command == "dcd")
    {
        return mode_dcd_help(--argument_c, ++argument_v);
    }
    else if(command == "calc")
    {
        return mode_calc_help(--argument_c, ++argument_v);
    }
    else
    {
        std::cerr << "mill help: unknown command : " << command << "\n\n";
        std::cerr << main_usage() << std::endl;
        return 1;
    }
}

} // mill
#endif// COFFEE_MILL_HELP_MODE
