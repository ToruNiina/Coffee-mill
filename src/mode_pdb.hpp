#ifndef COFFEE_MILL_PDB_MODE
#define COFFEE_MILL_PDB_MODE
#include <mill/pdb/PDBReader.hpp>
#include <mill/pdb/PDBWriter.hpp>
#include <src/mode_pdb_help.hpp>
#include <src/mode_pdb_seq.hpp>
// #include <src/mode_pdb_mutate.hpp>
#include <iostream>

namespace mill
{

// argv := arrayof{ "pdb", "command-name", {rests...} }
template<typename vectorT>
int mode_pdb(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        log::error("mill pdb mode: too few arguments\n");
        mode_pdb_help(--argument_c, ++argument_v); // {}
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
        log::error("mill pdb mode: unknown command: ", command, '\n');
        std::cerr << mode_pdb_help_usage() << std::endl;
        return 1;
    }
}

} // mill

#endif /* COFFEE_MILL_PDB_MODE */
