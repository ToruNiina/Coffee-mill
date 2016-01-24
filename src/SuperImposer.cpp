#include "SuperImposer.hpp"
#include "dcd/DCDManager.hpp"
#include "pdb/CGReader.hpp"
using namespace coffeemill;

/* @brief> this code generates superimposed dcd file. reference snapshot *
 *         to fit each snapshot is the previous snapshot. you can select *
 *         chains to use as reference. if you specify chains, rotation   *
 *         matrixes are calculated only using the chains.                *
 *    XXX> NOT FULLY TUNED                                               */

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

    std::string inputfilename(argv[1]);
    DCDReader reader(inputfilename);
    reader.read_file();

    // if chain IDs are specified, extract structures to use as references.
    // else, this reference is same as whole data.
    std::vector<SnapShot> rotation_reference(reader.get_size());
    if(argc == 3)
    {
        std::size_t filename_length = inputfilename.size();
        std::string filename(inputfilename.substr(0, filename_length - 4));
        std::string cgpdb(filename + ".pdb");
        CGReader cgreader(cgpdb);
        cgreader.read_file();
        CGMdlSptr model(cgreader.get_model(0));

        std::string IDwhitelist(argv[2]);
        std::transform(IDwhitelist.cbegin(),
                       IDwhitelist.cend(),
                       IDwhitelist.begin(),
                       toupper);
        //this contains how many mass points the chain has
        std::vector<int> chain_sizes;
        for(int i(0); i<model->size(); ++i)
        {
            chain_sizes.push_back(model->at(i)->size());
        }

        //this contains what is the ID of the chain to calculate RMSD
        std::vector<int> chain_IDs;
        for(auto iter = IDwhitelist.begin(); iter != IDwhitelist.end(); ++iter)
        {
            chain_IDs.push_back(model->find_id(*iter));
        }

        auto ref_iter = rotation_reference.begin();
        for(auto iter = reader.cbegin(); iter < reader.cend(); ++iter)
        {
            *ref_iter = pickup_chain(*iter, chain_IDs, chain_sizes);
            ++ref_iter;
        }
    }
    else
    {
        // simply copy.
        auto ref_iter = rotation_reference.begin();
        for(auto iter = reader.cbegin(); iter < reader.cend(); ++iter)
        {
            *ref_iter = *iter;
            ++ref_iter;
        }
    }

    // obtain rotation matrix from reference structures
    std::vector<Matrix3> rotation_matrixes(rotation_reference.size());
    SuperImposer supimposer(rotation_reference.at(0), rotation_reference.at(1));
    rotation_matrixes.at(0) = Matrix3(1e0); // reference structure itself
    rotation_matrixes.at(1) = supimposer.calc_and_get_R();
    for(std::size_t i = 2; i < rotation_reference.size(); ++i)
    {
        auto discard = supimposer.push_datas(rotation_reference.at(i));
        rotation_matrixes.at(i) = supimposer.calc_and_get_R();
    }

    // rotate subject structures.
    // (snapshot[t] * rotation_matrix[t]) -> imposed_snapshot[t]
    std::vector<SnapShot> imposed_output(reader.get_size());
    for(std::size_t timestep = 0; timestep < reader.get_size(); ++timestep)
    {
        SnapShot imposed_snapshot(reader[timestep].size());

        Realvec mean(0e0);
        for(auto iter = reader[timestep].cbegin();
                iter != reader[timestep].cend(); ++iter)
        {
            mean += *iter;
        }
        mean = mean / static_cast<double>(reader[timestep].size());

        for(std::size_t particleindex = 0;
            particleindex < reader[timestep].size();
            ++particleindex)
        {
            imposed_snapshot[particleindex]
                = rotation_matrixes[timestep] *
                    (reader[timestep][particleindex] - mean);
        }
        imposed_output[timestep] = imposed_snapshot;
    }

    // output superimposed dcd
    std::string outputfilename(inputfilename.substr(0, inputfilename.size()-4)
                               + "_SupImp.dcd");
    DCDWriter writer(outputfilename);
    set_header_from_reader(&writer, &reader);
    writer.set_all_data(imposed_output);
    writer.write_file();

    return EXIT_SUCCESS;
}

SnapShot pickup_chain(const SnapShot& ss, 
                      const std::vector<int>& chain_ids,
                      const std::vector<int>& chain_sizes)
{
    SnapShot retval(ss);
    auto ssiter = retval.begin();

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
