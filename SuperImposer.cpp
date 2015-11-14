#include "includes/SuperImposer.hpp"
#include "includes/dcd/DCDManager.hpp"
#include "includes/pdb/CGReader.hpp"
using namespace coffeemill;

SnapShot pickup_chain(const SnapShot& ss, 
                      const std::vector<int>& chain_ids,
                      const std::vector<int>& chain_sizes);

int main(int argc, char *argv[])
{
    if(!(argc == 2 || argc == 3))
    {
        std::cout << "Coffee-mill DCD SuperImposer" << std::endl;
        std::cout << "Usage: ./superimpose <filename>.dcd [ChainIDs]"
                  << std::endl;
        std::cout << "Usage: option [chainIDs] enables if there are "
                  << "CG style pdb file having same name as dcd file"
                  << std::endl;
        std::cout << "Info : SuperImposer outputs dcd file "
                  << "containing superimposed structure" << std::endl;
        return 1;
    }

    std::string infile(argv[1]);
    DCDReader reader(infile);
    reader.read_file();

    std::vector<int> chain_nums;
    std::vector<int> chain_sizes;
    if(argc == 3)
    {
        std::string chainIDs = argv[2];
        std::transform(chainIDs.cbegin(), chainIDs.cend(),
                       chainIDs.begin(), toupper);
        std::string cgpdb(infile.substr(0, infile.size()-3) + "pdb");
        CGReader cgreader(cgpdb);
        cgreader.read_file();
        CGMdlSptr model(cgreader.get_model(0));

        for(int i(0); i < model->size(); ++i)
            chain_sizes.push_back(model->at(i)->get_size());

        for(auto iter = chainIDs.begin(); iter != chainIDs.end(); ++iter)
            chain_nums.push_back(model->find_id(*iter));
    }

    std::vector<SnapShot> data(reader.get_all_data());
    std::vector<SnapShot> imposed(data.size());

    SuperImposer simposer;

    if(argc == 2)
    {
        simposer.set_data(data.at(1), data.at(0));
        simposer.superimpose();
        imposed.at(0) = simposer.get_data2();
    }
    else if(argc == 3)
    {
        SnapShot data0(pickup_chain(data.at(0), chain_nums, chain_sizes));
        SnapShot data1(pickup_chain(data.at(1), chain_nums, chain_sizes));
        simposer.set_data(data1, data0);
        Matrix3 R(simposer.get_R());

        SnapShot data0_rot(data.at(0));
        SnapShot data1_rot(data.at(1));

        Realvec sum0(0e0, 0e0, 0e0);
        Realvec sum1(0e0, 0e0, 0e0);
        SnapShot::iterator iter0 = data0_rot.begin();
        for(SnapShot::iterator iter = data1_rot.begin();
            iter != data1_rot.end(); ++iter)
        {
            sum0 += (*iter0);
            sum1 += (*iter);
            ++iter0;
        }
        sum0 /= static_cast<double>(data0_rot.size());
        sum1 /= static_cast<double>(data1_rot.size());
        iter0 = data0_rot.begin();
        for(SnapShot::iterator iter = data1_rot.begin();
            iter != data1_rot.end(); ++iter)
        {
            *iter0 -= sum0;
            *iter -= sum1;
            ++iter0;
        }

        for(SnapShot::iterator iter = data1_rot.begin();
            iter != data1_rot.end(); ++iter)
        {
            Realvec temp(R * (*iter));
            *iter = temp;
        }
        imposed.at(0) = data0_rot;
        imposed.at(1) = data1_rot;
    }
    
    for(size_t i(2); i<data.size(); ++i)
    {
        if(argc == 2)
        {
            simposer.set_data_push(data.at(i));
            simposer.superimpose();
            imposed.at(i-1) = simposer.get_data2();
        }
        else if(argc == 3)
        {
            SnapShot datai(pickup_chain(data.at(i), chain_nums, chain_sizes));
            simposer.set_data_push(datai);
            Matrix3 R(simposer.get_R());

            SnapShot data_rot(data.at(i));

            Realvec sum(0e0, 0e0, 0e0);
            for(SnapShot::iterator iter = data_rot.begin();
                iter != data_rot.end(); ++iter)
            {
                sum += *iter;
            }
            sum /= static_cast<double>(data_rot.size());

            for(SnapShot::iterator iter = data_rot.begin();
                iter != data_rot.end(); ++iter)
            {
                *iter -= sum;
            }

            for(SnapShot::iterator iter = data_rot.begin();
                iter != data_rot.end(); ++iter)
            {
                Realvec temp(R * (*iter));
                *iter = temp;
            }
            imposed.at(i) = data_rot;
        }
    }
    if(argc == 2)
        imposed.at(data.size()-1) = simposer.get_data1();

    std::string outfile("SupImp"+infile);
    DCDWriter writer(outfile);

    set_header_from_reader(&writer, &reader);
    writer.set_all_data(imposed);

    writer.write_file();

    return 0;
}

SnapShot pickup_chain(const SnapShot& ss, 
                                 const std::vector<int>& chain_ids,
                                 const std::vector<int>& chain_sizes)
{
    SnapShot retval(ss);

    SnapShot::iterator ssiter = retval.begin();

    for(size_t i(0); i<chain_sizes.size(); ++i)
    {
        if(std::find(chain_ids.begin(), chain_ids.end(), i) != chain_ids.end())
        {
            ssiter = ssiter + chain_sizes.at(i);
        }
        else
        {
            retval.erase(ssiter, ssiter + chain_sizes.at(i));
        }
    }

    return retval;
}
//(c)Toru Niina 2015 all rights reserved.
