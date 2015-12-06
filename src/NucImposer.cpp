#include "../includes/dcd/DCDManager.hpp"
#include "../includes/pdb/CGReader.hpp"
#include "../includes/SuperImposer.hpp"
using namespace coffeemill;

SnapShot pickupHistoneCore(const SnapShot& ss, const int DNA_length);
SnapShot truncate(const SnapShot& ss);

int main(int argc, char *argv[])
{
    if(argc != 2 && argc != 3)
    {
        std::cout << "Usage: ./nucanalysis <filename> [DNA length]" << std::endl;
        std::cout << "     : this considers chain AB is DNA and"
                  << "CDEFGHIJ is histone 8-mer" << std::endl;
        std::cout << "     : DNA length is 147 by default. " << std::endl;
        std::cout << "     : DNA particle numbers are calculated as "
                  << "3 * DNA length - 1. the case of 147, it is 440."
                  << std::endl;
        std::cout << "     : this considers histone as 1kx5."
                  << "histones has " << std::endl;
        std::cout << "135, 102, 128, 125 amino acid, respectively."
                  << " And the order is also this"
                  << std::endl;
        return 1;
    }
    std::string infile(argv[1]);
    DCDReader reader(infile);
    reader.read_file();

    int DNA_length(147);
    if(argc == 3)
    {
        std::string arg_DNA_len(argv[2]);
        DNA_length = (std::stoi(arg_DNA_len) * 3) - 1;
    }

    std::vector<SnapShot> data(reader.get_all_data());
    std::vector<SnapShot> imposed(data.size());
    SuperImposer simposer;

    SnapShot data0(data.at(0));
    SnapShot data1(data.at(1));

    SnapShot data0_histone(pickupHistoneCore(data0, DNA_length));
    SnapShot data1_histone(pickupHistoneCore(data1, DNA_length));

    simposer.set_data(data1_histone, data0_histone);
    Matrix3 R(simposer.get_R());

    Realvec sum0(0e0);
    for(auto iter = data0.begin(); iter != data0.end(); ++iter)
        sum0 += (*iter);
    sum0 /= static_cast<double>(data0.size());
    for(auto iter = data0.begin(); iter != data0.end(); ++iter)
        *iter -= sum0;

    Realvec sum1(0e0);
    for(auto iter = data1.begin(); iter != data1.end(); ++iter)
        sum1 += (*iter);
    sum1 /= static_cast<double>(data1.size());
    for(auto iter = data1.begin(); iter != data1.end(); ++iter)
        *iter -= sum1;

    for(auto iter = data1.begin(); iter != data1.end(); ++iter)
    {
        Realvec temp(R * (*iter));
        *iter = temp;
    }
    imposed.at(0) = data0;
    imposed.at(1) = data1;

    for(size_t i(2); i<data.size(); ++i)
    {
        SnapShot datai(data.at(i));
        SnapShot datai_histone(pickupHistoneCore(datai, DNA_length));
        simposer.set_data_push(datai);
        Matrix3 R(simposer.get_R());

        Realvec sum(0e0, 0e0, 0e0);
        for(auto iter = datai.begin(); iter != datai.end(); ++iter)
        {
            sum += *iter;
        }
        sum /= static_cast<double>(datai.size());

        for(auto iter = datai.begin(); iter != datai.end(); ++iter)
        {
            *iter -= sum;
        }

        for(auto iter = datai.begin(); iter != datai.end(); ++iter)
        {
            Realvec temp(R * (*iter));
            *iter = temp;
        }
        imposed.at(i) = datai;
    }

    std::string outfile(infile.substr(0, infile.size() - 4) + "_NucImp.dcd");
    DCDWriter writer(outfile);
    set_header_from_reader(&writer, &reader);
    writer.set_all_data(imposed);

    writer.write_file();

    return 0;
}

SnapShot pickupHistoneCore(const SnapShot& ss, const int DNA_length)
{
    SnapShot retval(ss);
    SnapShot::iterator iter = retval.begin();
    retval.erase(iter, iter + (2 * DNA_length));

    for(int i(0); i < 2; ++i)
    {
        retval.erase(iter, iter + 44); //size of each tail.
        iter = iter + 91;              //size of histone. 135 - tail.
        retval.erase(iter, iter + 24);
        iter = iter + 78;              //size of histone. 102 - tail.
        retval.erase(iter, iter + 16);
        iter = iter + 99;              //size of histone. 128 - tail.
        retval.erase(iter, iter + 13);
        retval.erase(iter, iter + 34);
        iter = iter + 91;              //size of histone. 125 - tail.
    }
    return retval;
}
