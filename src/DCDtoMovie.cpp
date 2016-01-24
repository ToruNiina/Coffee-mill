#include "dcd/DCDReader.hpp"
#include "pdb/CGReader.hpp"
using namespace coffeemill;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cerr << "Usage : ./dcdtomovie <filename>.pdb <filename>.dcd" << std::endl;
        return EXIT_FAILURE;
    }

    std::string pdbfilename(argv[1]);
    CGReader cgreader(pdbfilename);
    cgreader.read_file();
    CGMdlSptr model(cgreader.get_model(0));
    std::size_t particle_number(0);
    for(auto iter = model->begin(); iter != model->end(); ++iter)
    {
        particle_number += (*iter)->size();
    }

    std::string dcdfilename(argv[2]);
    std::shared_ptr<DCDReader> dcdreader(new DCDReader(dcdfilename));
    dcdreader->read_file();
    std::vector<SnapShot> data(dcdreader->get_all_data());
    dcdreader.reset();

    if(data.at(0).size() != particle_number)
    {
        std::cerr << "Error : " << pdbfilename << " and " << dcdfilename
                  << " has different number of beads!" << std::endl;
        std::cerr << pdbfilename << " has " << particle_number
                  << " and " << dcdfilename << " has " << data.at(0).size()
                  << std::endl;
        return EXIT_FAILURE;
    }else{
        std::cerr << "same data" << std::endl;
    }

    std::size_t filename_length = pdbfilename.size() - 4;
    std::string filename(pdbfilename.substr(0, filename_length));

    std::ofstream outputfile(filename + ".movie");
    for(auto snapshotIter = data.cbegin(); snapshotIter != data.cend();
            ++snapshotIter)
    {
        std::size_t index(0);
        std::size_t chain_index(1);
        outputfile << "<<<<" << std::setw(12) << std::right << index << std::endl;
        for(auto chainIter = model->cbegin(); chainIter != model->cend();
                ++chainIter)
        {
            CGChain output;
            for(auto beadIter = (*chainIter)->cbegin();
                    beadIter != (*chainIter)->cend();
                    ++beadIter)
            {
                BeadSptr bead(new CGBead(**beadIter));
                Realvec pos = (*snapshotIter)[index];
                bead->set_coordx(pos[0]);
                bead->set_coordy(pos[1]);
                bead->set_coordz(pos[2]);
                output.push_back(bead);
                ++index;
            }
            output.write_block(outputfile, chain_index);
            ++chain_index;
        }
        outputfile << ">>>>" << std::endl;
        outputfile << "END" << std::endl;
        outputfile << std::endl;
    }

    return EXIT_SUCCESS;
} 
