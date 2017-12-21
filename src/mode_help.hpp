#ifndef COFFEE_MILL_HELP_MODE
#define COFFEE_MILL_HELP_MODE
#include <iostream>
#include "mill_pdb.hpp"
#include "mill_dcd.hpp"
#include "mill_ninfo.hpp"

namespace mill
{

inline void print_main_help()
{
    std::cerr << "Usage: mill [mode] [parameters...]\n\n";
    std::cerr << "list of modes\n";
    std::cerr << "\tpdb\n";
    std::cerr << "\tdcd\n";
    std::cerr << "\tninfo\n";
    std::cerr << "\thelp\n";
    std::cerr << std::flush;
    return;
}

//! this function forwards the arguments to different modes.
// argv := { "help", {args...} }
inline int mode_help(int argument_c, char **argument_v)
{
    if(argument_c < 2)
    {
        print_main_help();
        return 0;
    }

    const std::string command(argument_v[1]);
    if(command == "pdb")
    {
        return mode_pdb(2, argument_v);
    }
    else if(command == "ninfo")
    {
        return mode_ninfo(2, argument_v);
    }
    else if(command == "dcd")
    {
        return mode_dcd(2, argument_v);
    }
    else
    {
        std::cerr << "mill help: unknown command \"" << command << "\"\n\n";
        print_main_help();
        return 1;
    }
    return 0;
}

} // mill
#endif// COFFEE_MILL_HELP_MODE
