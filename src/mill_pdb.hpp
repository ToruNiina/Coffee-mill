#ifndef COFFEE_MILL_PDB_MODE
#define COFFEE_MILL_PDB_MODE
#include "PDBReader.hpp"
#include "PDBWriter.hpp"
#include "SequenceExtractor.hpp"

namespace mill
{

template<typename vectorT>
int mode_pdb(int argument_c, char **argument_v)
{
    if(argument_c < 2) throw std::invalid_argument("too few commands");
    const std::string command(argument_v[1]);
    if(command == "seq")
    {
        if(argument_c < 3) throw std::invalid_argument("too few commands");

        const std::string fname(argument_v[2]);
        PDBReader<vectorT> reader;
        const auto atoms = reader.read(fname);
        const auto chains = reader.parse(atoms);
        SequenceExtractor<vectorT> extr;
        for(auto iter = chains.cbegin(); iter != chains.cend(); ++iter)
            std::cout << "chain " << iter->chain_id() << ": "
                      << extr.extract(*iter) << std::endl;
        return 0;
    }
    else
    {
        throw std::invalid_argument("command not found");
    }
}

} // mill

#endif /* COFFEE_MILL_PDB_MODE */
