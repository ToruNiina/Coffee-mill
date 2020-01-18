#ifndef COFFEE_MILL_CALC_MODE
#define COFFEE_MILL_CALC_MODE
#include <src/mode_calc_help.hpp>
#include <src/mode_calc_rmsd.hpp>
#include <iostream>

namespace mill
{

// argv := arrayof{ "dcd", "command-name", {rests...} }
template<typename vectorT>
int mode_calc(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        log::error("mill calc: too few arguments\n");
        mode_calc_help(--argument_c, ++argument_v); // {}
        return 1;
    }

    const std::string command(argument_v[1]);
    if(command == "rmsd")
    {
        return mode_calc_rmsd<vectorT>(--argument_c, ++argument_v);
    }
    else if(command == "help")
    {
        return mode_calc_help(--argument_c, ++argument_v);
    }
    else
    {
        log::error("mill calc mode: unknown command: ", command, '\n');
        std::cerr << mode_calc_help_usage() << std::endl;
        return 1;
    }
}

} // mill

#endif /* COFFEE_MILL_PDB_MODE */
