#include "DCDReader.hpp"
#include "PDBReader.hpp"
#include "DCDtoMovie.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>

namespace coffeemill
{
void DCDtoMovie::convert() const
{
    DCDReader dcdreader(this->dcdfile_);
    dcdreader.read();
    auto traj = dcdreader.data();

    PDBReader pdbreader(this->pdbfile_);
    pdbreader.read();
    auto structure = pdbreader.atoms();

    if(traj.empty())
        throw std::runtime_error("dcdfile contain no structure");
    if(traj.front().size() != structure.size())
        throw std::invalid_argument("pdb and dcd has different structure");

    std::ofstream output(this->output_);
    if(!output.good())
        throw std::runtime_error("file open error: " + this->output_);

    std::size_t index = 0;
    for(auto snapshot = traj.cbegin(); snapshot != traj.cend(); ++snapshot)
    {
        if(this->pymol_format_)
        {
            output << "MODEL" << std::setw(9) << std::right << index << std::endl;
        }
        else
        {
            output << "<<<<" << std::setw(12) << std::right << index << std::endl;
        }

        for(std::size_t i = 0; i < structure.size(); ++i)
        {
            structure.at(i)->pos() = snapshot->at(i);
            output << *(structure.at(i)) << std::endl;
        }

        if(this->pymol_format_)
        {
            output << "TER" << std::endl;
            output << "ENDMDL" << std::endl;
        }
        else
        {
            output << ">>>>" << std::endl;
            output << "END" << std::endl;
            output << std::endl;
        }
        ++index;
    }

    output.close();
    return;
}

}
