#include "PDBReader.hpp"
#include <iostream>
#include <fstream>

namespace coffeemill
{

void PDBReader::read_file()
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


}
