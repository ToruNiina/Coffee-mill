#include "DCDJoiner.hpp"

namespace coffeemill
{

void DCDJoiner::join() const
{
    std::size_t num_particle;
    std::size_t num_set;
    DCDData data;

    {
    DCDReader reader(this->filenames_.front());
    reader.read();
    num_particle = reader.data().nparticle();
    num_set    = reader.data().nset();
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
            num_set += reader.data().nset();
        else
            num_set += reader.data().nset() - 1;
    }
    data.nset() = num_set;

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
            for(auto snapshot = reader.data().cbegin() + 1;
                    snapshot != reader.data().cend(); ++snapshot)
            {
                writer.write_snapshot(*snapshot);
            }
        }
    }


    return;
}

}
