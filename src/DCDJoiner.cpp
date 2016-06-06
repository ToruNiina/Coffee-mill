#include "DCDJoiner.hpp"

namespace coffeemill
{

void DCDJoiner::join() const
{
    std::size_t num_particle;
    std::size_t num_steps;
    DCDData data;

    {
    DCDReader reader(this->filenames_.front());
    reader.read();
    num_particle = reader.data().nparticle();
    num_steps    = reader.data().nstep();
    data = reader.data();
    }

    for(auto iter = this->filenames_.cbegin() + 1;
            iter != this->filenames_.cend(); ++iter)
    {
        DCDReader reader(*iter);
        reader.read_header();
        if(num_particle != reader.data().nparticle())
        {
            throw std::runtime_error(
                    "dcdfiles contain different number of atoms");
        }
        if(this->redundancy_)
            num_steps += reader.data().nstep();
        else
            num_steps += reader.data().nstep() - 2;
    }
    data.nstep() = num_steps;

    DCDWriter writer(this->output_);
    writer.data() = data;
    writer.write();

    for(auto iter = this->filenames_.cbegin() + 1;
            iter != this->filenames_.cend(); ++iter)
    {
        DCDReader reader(*iter);
        reader.read();

        if(this->redundancy_)
        {
            for(auto snapshot = reader.data().cbegin();
                    snapshot != reader.data().cend(); ++snapshot)
            {
                writer.write_snapshot(*snapshot);
            }
        }
        else
        {
            for(auto snapshot = reader.data().cbegin() + 2;
                    snapshot != reader.data().cend(); ++snapshot)
            {
                writer.write_snapshot(*snapshot);
            }
        }
    }


    return;
}

}
