#include "PDBChain.hpp"
#include "PDBReader.hpp"
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
    std::cout << "structure.size " << structure.size() << std::endl;
    for(auto chain = structure.cbegin(); chain != structure.cend(); ++chain)
    {
        std::cout << "chain.size " << chain->size() << std::endl;
        for(auto residue = chain->cbegin(); residue != chain->cend(); ++residue)
            std::cout << (*residue)->residue() << std::endl;
    }

    return EXIT_SUCCESS;
}



