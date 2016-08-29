#ifndef COFFEE_MILL_DNA_MUTATOR
#define COFFEE_MILL_DNA_MUTATOR
#include "PDBChain.hpp"
#include <map>

namespace coffeemill
{

class DNAMutator
{
  public:
    DNAMutator(){}
    ~DNAMutator() = default;

    PDBChain mutate(const std::string& seq, const PDBChain& chain) const;

  private:

    const std::map<char, double> SBlength = {
        {'A', 4.863},
        {'T', 4.379},
        {'C', 4.299},
        {'G', 4.973}
    };
};

PDBChain DNAMutator::mutate(const std::string& seq, const PDBChain& chain) const
{
    if(seq.size() != chain.size())
        throw std::invalid_argument("sequence and chain have different size");

    std::size_t index = 0;
    PDBChain mutated;

    for(auto residue = chain.cbegin(); residue != chain.cend(); ++residue)
    {
        std::shared_ptr<PDBResidue> res(new PDBResidue);
        for(auto iter = (*residue)->cbegin();
                iter != (*residue)->cend(); ++iter)
        {
            // deep copy
            std::shared_ptr<PDBAtom> atom(new PDBAtom(**iter));
            res->push_back(atom);
        }

        ax::Vector3d B(0e0);
        ax::Vector3d S(0e0);

        for(auto atom = res->cbegin(); atom != res->cend(); ++atom)
        {
            if((*atom)->atom() == "DB  ")
                B = (*atom)->pos();
            else if((*atom)->atom() == "DS  ")
                S = (*atom)->pos();
        }

        const ax::Vector3d direction = (B - S) / length(B - S);
        const ax::Vector3d B_newpos = S + 
            direction * this->SBlength.at(seq.at(index));

        for(auto atom = res->cbegin(); atom != res->cend(); ++atom)
        {
            (*atom)->residue() = std::string("D") + seq.at(index) + std::string(" ");
                 if((*atom)->atom() == "DB  ") (*atom)->pos() = B_newpos;
            else if((*atom)->atom() == "DS  ") S = (*atom)->pos();
        }

        mutated.push_back(res);
        index++;
    }

    return mutated;
}


}

#endif /*COFFEE_MILL_DNA_MUTATOR*/
