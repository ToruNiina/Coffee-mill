#ifndef COFFEE_MILL_SEQUENCE_EXTRACTOR
#define COFFEE_MILL_SEQUENCE_EXTRACTOR
#include "PDBChain.hpp"
#include "utility.hpp"
#include <string>
#include <map>

namespace mill
{

template<typename vectorT>
class SequenceExtractor
{
  public:
    using vector_type = vectorT;
    using chain_type = PDBChain<vector_type>;

  public:
    SequenceExtractor(){}
    ~SequenceExtractor() = default;

    std::string extract(const chain_type& chain) const;

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

template<typename vectorT>
std::string SequenceExtractor<vectorT>::extract(const chain_type& chain) const
{
    std::string sequence;
    for(auto residue = chain.cbegin(); residue != chain.cend(); ++residue)
    {
        std::string res = residue->residue_name();
        res = remove_all(' ', res);
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


} // mill

#endif /* COFFEE_MILL_SEQUENCE_EXTRACTOR */
