#include "includes/pdb/PDBChain.hpp"
#include "includes/pdb/CGChain.hpp"

using namespace coffeemill;

int main(int argc, char* argv[])
{
    if(argc != 2 && argc != 3)
    {
        std::cout << "Usage  : ./seqextr [OPTION] [filename].pdb" << std::endl;
        std::cout << "Option : -cg : read cg style" << std::endl;
        return -1;
    }

    std::string argv1(argv[1]);
    if(argv1 == "-cg")
    {//read CG style
        std::ifstream pdbfile(argv[2]);

        while(!pdbfile.eof())
        {
            CGChain chain;
            chain.read_block(pdbfile);

            if(chain.chain_exist())
            {
                std::cout << "Chain " << chain.get_chainID() << ": ";
                std::cout << chain.get_sequence() << std::endl;
            }
        }
    }
    else
    {//read plane PDB
        std::ifstream pdbfile(argv[1]);

        while(!pdbfile.eof())
        {
            PDBChain chain;
            chain.read_block(pdbfile);

            if(chain.chain_exist())
            {
                std::cout << "Chain " << chain.get_chainID() << ": ";
                std::cout << chain.get_sequence() << std::endl;
            }
        }
    }
    return 0;
}



