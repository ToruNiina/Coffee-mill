#ifndef COFFEE_MILL_DCD_MODE
#define COFFEE_MILL_DCD_MODE
#include <src/mode_dcd_help.hpp>
#include <src/mode_dcd_join.hpp>
#include <src/mode_dcd_split.hpp>
#include <src/mode_dcd_extract.hpp>
#include <src/mode_dcd_impose.hpp>
#include <src/mode_dcd_convert.hpp>
#include <src/mode_dcd_calc_dist.hpp>
#include <src/mode_dcd_calc_theta.hpp>
#include <src/mode_dcd_info.hpp>
// #include "mill_dcd_msd.hpp"
#include <iostream>

namespace mill
{

// argv := arrayof{ "dcd", "command-name", {rests...} }
template<typename vectorT>
int mode_dcd(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        log::error("mill dcd mode: too few arguments");
        mode_dcd_help(--argument_c, ++argument_v);
        return 1;
    }

    const std::string command(argument_v[1]);
    if(command == "join")
    {
        // {"join", {"args"...}}
        return mode_dcd_join<vectorT>(--argument_c, ++argument_v);
    }
    else if(command == "extract")
    {
        return mode_dcd_extract<vectorT>(--argument_c, ++argument_v);
    }
    else if(command == "split")
    {
        return mode_dcd_split<vectorT>(--argument_c, ++argument_v);
    }
    else if(command == "impose")
    {
        return mode_dcd_impose<vectorT>(--argument_c, ++argument_v);
    }
    else if(command == "convert")
    {
        return mode_dcd_convert<vectorT>(--argument_c, ++argument_v);
    }
    else if(command == "calc_theta")
    {
        return mode_dcd_calc_theta<vectorT>(--argument_c, ++argument_v);
    }
    else if(command == "calc_dist")
    {
        return mode_dcd_calc_dist<vectorT>(--argument_c, ++argument_v);
    }
    else if(command == "info")
    {
        return mode_dcd_info<vectorT>(--argument_c, ++argument_v);
    }
    else if(command == "help")
    {
        return mode_dcd_help(--argument_c, ++argument_v);
    }
    else
    {
        log::error("mill dcd mode: unknown command: ", command);
        std::cerr << mode_dcd_help_usage() << std::endl;
        return 1;
    }
}

} // mill
#endif /* COFFEE_MILL_PDB_MODE */
