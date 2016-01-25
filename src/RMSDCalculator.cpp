#include "RMSDCalculator.hpp"
#include "dcd/DCDReader.hpp"
#include "pdb/CGReader.hpp"
using namespace coffeemill;

SnapShot pickup_chain(const SnapShot& ss, 
                      const std::vector<int>& chain_ids,
                      const std::vector<int>& chain_sizes);

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "RMSDCalculator" << std::endl;
        std::cout << "USAGE: ./rmsdcalc <filename> <chainIDs>" << std::endl;
        std::cout << "       input the filename without extension" << std::endl;
        std::cout << "       input IDs(alphabet) of the chain you want to calculate RMSD"
                  << std::endl;
        std::cout << "       this reads pdb(cg style) and dcd file." << std::endl;
        std::cout << "       please confirm these files exist and have same name."
                  << std::endl;
        return -1;
    }

    std::string filename(argv[1]);

    std::string cgpdb(filename + ".pdb");
    CGReader cgreader(cgpdb);
    cgreader.read_file();
    CGMdlSptr model(cgreader.get_model(0));

    std::string IDs(argv[2]);
    std::transform(IDs.cbegin(), IDs.cend(), IDs.begin(), toupper);

    //this contains how many mass points the chain has
    std::vector<int> chain_sizes;
    for(int i(0); i<model->size(); ++i)
    {
        chain_sizes.push_back(model->at(i)->size());
    }

    //this contains what is the ID of the chain to calculate RMSD
    std::vector<int> chain_IDs;
    for(auto iter = IDs.begin(); iter != IDs.end(); ++iter)
    {
        chain_IDs.push_back(model->find_id(*iter));
    }

    std::string dcd(filename + ".dcd");
    DCDReader dcdreader;
    dcdreader.read_file(dcd);

    if(dcdreader.size() < 2)
    {
        std::cout << "there are only" << dcdreader.size()
                  << "snapshots." << std::endl;
        std::cout << "it is not enough to calculate RMSD." << std::endl;
        return EXIT_FAILURE;
    }

    SnapShot initial(pickup_chain(dcdreader.at(0), chain_IDs, chain_sizes));
    const int nstep_save = dcdreader.get_nstep_save();

    RMSDCalculator rmsdcalc;
    std::string output(filename + "_" + IDs + "_RMSD.ts");
    std::ofstream ofs(output);

    ofs << 0 << " " << rmsdcalc.get_RMSD(initial, initial) << std::endl;

    for(int i(1); i < dcdreader.size(); ++i)
    {
        ofs << nstep_save * i << " "
            << rmsdcalc.get_RMSD(initial,
                    pickup_chain(dcdreader.at(i), chain_IDs, chain_sizes))
            << std::endl;
    }

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
