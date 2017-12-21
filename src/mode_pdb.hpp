#ifndef COFFEE_MILL_PDB_MODE
#define COFFEE_MILL_PDB_MODE
#include <mill/data/PDBReader.hpp>
#include <mill/data/PDBWriter.hpp>
#include <src/mode_pdb_help.hpp>
#include <src/mode_pdb_seq.hpp>
#include <src/mode_pdb_mutate.hpp>
#include <iostream>

namespace mill
{

// argv := arrayof{ "pdb", "command-name", {rests...} }
template<typename vectorT>
int mode_pdb(int argument_c, char **argument_v)
{
    if(argument_c < 2)
    {
        std::cerr << "error: mill dcd-mode: too few arguments\n";
        mode_dcd_help(--argument_c, ++argument_v); // {}
        return 1;
    }

    const std::string command(argument_v[1]);
    if(command == "seq")
    {
        return mode_pdb_seq<vectorT>(--argument_c, ++argument_v);
    }
//     else if(command == "mutate")
//     {
//         return mode_pdb_mutate<vectorT>(--argument_c, ++argument_v);
//     }
    else
    {
        throw std::invalid_argument("command not found");
    }
}

} // mill

#endif /* COFFEE_MILL_PDB_MODE */
