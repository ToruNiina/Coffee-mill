#include "PDBChain.hpp"
#include "PDBReader.hpp"
#include "SequenceExtractor.hpp"
using namespace coffeemill;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage  : ./seqextr [filename].pdb" << std::endl;
        return EXIT_FAILURE;
    }

    PDBReader reader(argv[1]);
    reader.read();
    auto structure = reader.parse();

    SequenceExtractor seqextr;
    for(auto chain = structure.cbegin(); chain != structure.cend(); ++chain)
    {
        std::cout << "chain " << chain->chain_id() << ": "
                  << seqextr(*chain) << std::endl;
    }

    return EXIT_SUCCESS;
}



