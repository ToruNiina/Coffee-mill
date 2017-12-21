#ifndef COFFEE_MILL_DCD_HELP_MODE
#define COFFEE_MILL_DCD_HELP_MODE
#include <iostream>
#include <mill/math/Vector.hpp>
#include "mode_dcd_join.hpp"
// #include "mode_dcd_split.hpp"
// #include "mode_dcd_extract.hpp"
// #include "mode_dcd_convert.hpp"
// #include "mode_dcd_impose.hpp"

namespace mill
{

inline const char* mode_dcd_help_usage() noexcept
{
    return "usage: mill dcd [command] [parameters...]\n\n"
           "    avaiable commands\n"
           "    - impose\n"
           "      : superimpose snapshots by minimizing RMSD\n"
           "    - join\n"
           "      : concatenate several .dcd files\n"
           "    - split\n"
           "      : split .dcd files into several files\n"
           "    - extract\n"
           "      : extract one snapshot from dcd file\n"
           "    - convert\n"
           "      : convert dcd file into another format\n"
           "    - help\n"
           "      : print detailed explanation of each command\n";
}

//! this function forwards the arguments to different modes.
// argv := { "help" or "dcd", {args...} }
inline int mode_dcd_help(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        std::cerr << mode_dcd_help_usage() << std::endl;
        return 0;
    }

    const std::string command(argument_v[1]);

    if(command == "join")
    {
        const char *cmds[2] = {"join", "help"};
        return mode_dcd_join<mill::Vector<double, 3>>(2, cmds);
    }
//     else if(command == "impose")
//     {
//         const char *cmds[2] = {"impose", "help"};
//         return mode_dcd_impose(2, cmds);
//     }
//     else if(command == "split")
//     {
//         const char *cmds[2] = {"split", "help"};
//         return mode_dcd_split(2, cmds);
//     }
//     else if(command == "extract")
//     {
//         const char *cmds[2] = {"extract", "help"};
//         return mode_dcd_extract(2, cmds);
//     }
//     else if(command == "convert")
//     {
//         const char *cmds[2] = {"convert", "help"};
//         return mode_dcd_convert(2, cmds);
//     }
    else
    {
        std::cerr << "error: mill dcd help: unknown command : "
                  << command << "\n\n";
        std::cerr << mode_dcd_help_usage() << std::endl;
        return 1;
    }
}

} // mill
#endif// COFFEE_MILL_HELP_MODE
