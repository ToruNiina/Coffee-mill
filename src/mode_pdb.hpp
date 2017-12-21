#ifndef COFFEE_MILL_PDB_MODE
#define COFFEE_MILL_PDB_MODE
#include <mill/data/PDBReader.hpp>
#include <mill/data/PDBWriter.hpp>
#include "mill_pdb_seq.hpp"
#include "mill_pdb_mutate.hpp"

namespace mill
{

template<typename vectorT>
int mode_pdb(int argument_c, char **argument_v)
{
    if(argument_c < 2) throw std::invalid_argument("too few commands");
    const std::string command(argument_v[1]);
    if(command == "seq")
    {
        return mill_pdb_seq<vectorT>(--argument_c, ++argument_v);
    }
    else if(command == "mutate")
    {
        return mill_pdb_mutate<vectorT>(--argument_c, ++argument_v);
    }
    else
    {
        throw std::invalid_argument("command not found");
    }
}

} // mill

#endif /* COFFEE_MILL_PDB_MODE */
