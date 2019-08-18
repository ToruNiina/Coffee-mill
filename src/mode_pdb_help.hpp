#ifndef COFFEE_MILL_PDB_HELP_MODE
#define COFFEE_MILL_PDB_HELP_MODE
#include <iostream>
#include <mill/math/Vector.hpp>
#include <src/mode_pdb_seq.hpp>
// #include <src/mode_pdb_mutate.hpp>

namespace mill
{

inline const char* mode_pdb_help_usage() noexcept
{
    return "usage: mill pdb [command] [parameters...]\n\n"
           "    avaiable commands\n"
           "    - seq\n"
           "        print a sequence of each chain\n"
           "    - mutate\n"
           "        mutate coarse-grained chain by renaming residues\n"
           "    - help\n"
           "        print detailed explanation of each command\n";
}

//! this function forwards the arguments to different modes.
// argv := { "help" or "pdb", {args...} }
inline int mode_pdb_help(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        std::cerr << mode_pdb_help_usage() << std::endl;
        return 0;
    }

    const std::string command(argument_v[1]);

    if(command == "seq")
    {
        const char *cmds[2] = {"seq", "help"};
        return mode_pdb_seq<mill::Vector<double, 3>>(2, cmds);
    }
//     else if(command == "mutate")
//     {
//         const char *cmds[2] = {"mutate", "help"};
//         return mode_pdb_mutate<mill::Vector<double, 3>>(2, cmds);
//     }
    else
    {
        log(log_level::error, "mill pdb help: unknown command : ", command, '\n');
        std::cerr << mode_pdb_help_usage() << std::endl;
        return 1;
    }
}

} // mill
#endif// COFFEE_MILL_pdb_HELP_MODE
