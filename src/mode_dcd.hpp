#ifndef COFFEE_MILL_DCD_MODE
#define COFFEE_MILL_DCD_MODE
// #include "DCDtoMovie.hpp"
#include <src/mode_dcd_join.hpp>
#include <src/mode_dcd_split.hpp>
#include <src/mode_dcd_extract.hpp>
#include <src/mode_dcd_impose.hpp>
// #include "mill_dcd_msd.hpp"
// #include "mode_dcd_help.hpp"

namespace mill
{

// argv := arrayof{ "dcd", "command-name", {rests...} }
template<typename vectorT>
int mode_dcd(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        // no commands provided: {"dcd"}
        std::cerr << "error: mill dcd-mode: too few arguments\n";
        mode_dcd_help(--argument_c, ++argument_v); // {}
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
//     else if(command == "make-movie")
//     {
//         const std::string fname(argument_v[2]);
//         return dcd_to_movie<vectorT>(fname);
//     }
    else if(command == "help")
    {
        return mode_dcd_help(--argument_c, ++argument_v);
    }
    else
    {
        std::cerr << "error: mill dcd-mode: unknown command : " << command << '\n';
        return 1;
    }
}

} // mill

#endif /* COFFEE_MILL_PDB_MODE */
