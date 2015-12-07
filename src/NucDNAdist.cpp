#include "../includes/dcd/DCDManager.hpp"
#include "../includes/SuperImposer.hpp"
using namespace coffeemill;

SnapShot pickupHistoneCore(const SnapShot& ss, const int DNA_length);
std::vector<Realvec> extractDNAcenter(const SnapShot& ss, const int DNA_length);

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: ./nucdnadist <filename>.dcd [DNA length]" << std::endl;
        return 1;
    }
    std::string infile(argv[1]);
    DCDReader reader(infile);
    reader.read_file();
    std::vector<SnapShot> data(reader.get_all_data());

    int DNA_length(440);
    if(argc == 3)
    {
        std::string arg_DNA_len(argv[2]);
        DNA_length = (std::stoi(arg_DNA_len) * 3) - 1;
    }

    std::vector<std::vector<double> > output;

    for(auto iter = data.begin(); iter != data.end(); ++iter)
    {
        SnapShot histone_core(pickupHistoneCore(*iter, DNA_length));
        std::vector<Realvec> DNAcenter(extractDNAcenter(*iter, DNA_length));
        std::vector<double> out_dna;
        for(auto Diter = DNAcenter.begin(); Diter != DNAcenter.end(); ++Diter)
        {
            double mindist(length(*Diter - *(histone_core.begin())));
            for(auto Hiter = histone_core.begin()+1; Hiter != histone_core.end();
                    ++Hiter)
            {
                double dist = length(*Diter - *Hiter);
                if(mindist > dist)
                    mindist = dist;
            }
            out_dna.push_back(mindist);
        }

        if(out_dna.size() != static_cast<size_t>((DNA_length + 1)/3))
        {
            std::cout << "output DNA_length: " << out_dna.size() << std::endl;
        }
        output.push_back(out_dna);
    }

    std::ofstream ofs(infile + ".DNAdist.ts");
    for(auto Oiter = output.begin(); Oiter != output.end(); ++Oiter)
    {
        for(auto Iiter = (*Oiter).begin(); Iiter != (*Oiter).end(); ++Iiter)
        {
            ofs << *Iiter << " ";
        }
        ofs << std::endl;
    }

    return 0;
}

SnapShot pickupHistoneCore(const SnapShot& ss, const int DNA_length)
{
    SnapShot retval(ss);
    SnapShot::iterator iter = retval.begin();
    retval.erase(iter, iter + (2 * DNA_length));

    for(int i(0); i < 2; ++i)
    {
        retval.erase(iter, iter + 45); //size of each tail.
        iter = iter + 90;              //size of histone. 135 - tail.
        retval.erase(iter, iter + 25);
        iter = iter + 77;              //size of histone. 102 - tail.
        retval.erase(iter, iter + 16);
        iter = iter + 99;              //size of histone. 128 - tail.
        retval.erase(iter, iter + 13);
        retval.erase(iter, iter + 34);
        iter = iter + 91;              //size of histone. 125 - tail.
    }
//     std::cout << "debug  : iter == ss.end(-1) : " << (iter == ss.end()-1) << std::endl;
    return retval;
}

std::vector<Realvec> extractDNAcenter(const SnapShot& ss, const int DNA_length)
{
    size_t dna_base((DNA_length + 1)/3);
    std::vector<Realvec> retval(dna_base);

    SnapShot dna_only(ss);
    dna_only.erase(dna_only.begin()+(DNA_length*2), dna_only.end());

    size_t dnaonlysize(dna_only.size()-1);
    for(size_t i(0); i<147; ++i)
    {
        Realvec fwbase_pos = dna_only.at(1 + 3*i);
        Realvec rvbase_pos = dna_only.at(dnaonlysize - 3*i);

        retval.at(i) = (fwbase_pos + rvbase_pos) / 2e0;
    }

    return retval;
}
