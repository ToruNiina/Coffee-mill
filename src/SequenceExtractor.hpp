#ifndef COFFEE_MILL_SEQUENCE_EXTRACTOR
#define COFFEE_MILL_SEQUENCE_EXTRACTOR
#include <string>
#include <map>
#include "PDBChain.hpp"
#include "utility.hpp"

namespace coffeemill
{

class SequenceExtractor
{
  public:
    SequenceExtractor(){}
    ~SequenceExtractor() = default;

    std::string operator()(const PDBChain& chain) const
    {
        std::string sequence;
        for(auto residue = chain.cbegin(); residue != chain.cend(); ++residue)
        {
            std::string res = (*residue)->residue();
            res = remove_all<' '>(res);
            if(res[0] == 'D')
            {
                sequence += res[1];
            }
            else if(res.size() == 1)
            {
                sequence += res;
            }
            else 
            {
                try{sequence += amino_acid.at(res);}
                catch(std::out_of_range& except)
                {
                    std::cerr << "Exception caught: residue " << res << std::endl; 
                    std::cerr << "what: " << except.what() << std::endl;
                    throw;
                }
            }
        }
        return sequence;
    }

  private:

    std::map<std::string, char> amino_acid = {
        {"ALA", 'A'},
        {"ASX", 'B'},
        {"CYS", 'C'},
        {"ASP", 'D'},
        {"GLU", 'E'},
        {"PHE", 'F'},
        {"GLY", 'G'},
        {"HIS", 'H'},
        {"ILE", 'I'},
        {"LYS", 'K'},
        {"LEU", 'L'},
        {"MET", 'M'},
        {"ASN", 'N'},
        {"PRO", 'P'},
        {"GLN", 'Q'},
        {"ARG", 'R'},
        {"SER", 'S'},
        {"THR", 'T'},
        {"SEC", 'U'},
        {"VAL", 'V'},
        {"TRP", 'W'},
        {"XAA", 'X'},
        {"TYR", 'Y'},
        {"GLX", 'Z'}
    };
};

}

#endif /* COFFEE_MILL_SEQUENCE_EXTRACTOR */
