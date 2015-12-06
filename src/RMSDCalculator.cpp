#include "../includes/RMSDCalculator.hpp"
#include "../includes/dcd/DCDReader.hpp"
#include "../includes/pdb/CGReader.hpp"
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

    std::string output(filename + "_" + IDs + "_RMSD.ts");
    std::ofstream ofs(output);

    RMSDCalculator rmsdcalc;

    if(dcdreader.get_size() < 2)
    {
        std::cout << "there is only" << dcdreader.get_size()
                  << "snapshots." << std::endl;
        std::cout << "it is not enough to calculate RMSD." << std::endl;
        return 1;
    }

    std::pair<SnapShot, double> initial(dcdreader.get_snapshot(0));
    std::pair<SnapShot, double> second_(dcdreader.get_snapshot(1));


    SnapShot init(pickup_chain(initial.first, chain_IDs, chain_sizes));
    SnapShot seco(pickup_chain(second_.first, chain_IDs, chain_sizes));

    rmsdcalc.set_data(init, init);
    ofs << 0e0 << " " << rmsdcalc.get_RMSD() << std::endl;

    rmsdcalc.set_data(init, seco);
    ofs << second_.second << " " << rmsdcalc.get_RMSD() << std::endl;

    for(int i(2); i < dcdreader.get_size(); ++i)
    {
        std::pair<SnapShot, double> snapshot
            (dcdreader.get_snapshot(i));
        SnapShot sshot(pickup_chain(snapshot.first, chain_IDs, chain_sizes));

        rmsdcalc.set_data2(sshot);
        ofs << snapshot.second << " " << rmsdcalc.get_RMSD() << std::endl;
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
