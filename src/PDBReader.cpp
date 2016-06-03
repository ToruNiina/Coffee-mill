#include "PDBReader.hpp"
#include <iostream>
#include <fstream>

namespace coffeemill
{

void PDBReader::read()
{
    std::ifstream ifs(this->filename_);
    if(!ifs.good())
        throw std::runtime_error("file open error: " + this->filename_);

    while(!ifs.eof())
    {
        std::string line;
        std::getline(ifs, line);
        if(line.empty()) continue;

        std::shared_ptr<PDBAtom> atom(new PDBAtom);
        if(line >> *atom)
            this->atoms_.push_back(atom);
    }

    ifs.close();
    return;
}

std::vector<PDBChain> PDBReader::parse()
{
    // find residues
    using PDBResidueSptr = std::shared_ptr<PDBResidue>;
    std::vector<PDBResidueSptr> residues;
    PDBResidueSptr residue(new PDBResidue);
    std::size_t residue_id = 0;
    for(auto iter = this->atoms_.cbegin(); iter != this->atoms_.cend(); ++iter)
    {
        if((*iter)->residue_id() != residue_id)
        {
            if(!residue->empty())
            {
                PDBResidueSptr temp(new PDBResidue(*residue));
                residues.push_back(temp);
            }
            residue->clear();
            residue_id = (*iter)->residue_id();
        }
        residue->push_back(*iter);
    }
    if(!residue->empty())
        residues.push_back(residue);

    // find chains
    std::vector<PDBChain> structure;
    std::string chain_id = "";
    PDBChain chain;
    for(auto iter = residues.cbegin(); iter != residues.cend(); ++iter)
    {
       if((*iter)->chain_id() != chain_id)
        {
            if(!chain.empty())
                structure.push_back(chain);
            chain.clear();
            chain_id = (*iter)->chain_id();
        }
        chain.push_back(*iter);
    }
    if(!chain.empty())
        structure.push_back(chain);

    return structure;
}


}
