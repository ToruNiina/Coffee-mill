#include "../includes/NucTracker.hpp"
using namespace coffeemill;

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
        Realvec center(0e0);
        for(auto Hiter = histone_core.begin(); Hiter != histone_core.end(); ++Hiter)
        {
            center += *Hiter;
        }
        center /= static_cast<double>(histone_core.size());
        for(auto Diter = DNAcenter.begin(); Diter != DNAcenter.end(); ++Diter)
        {
            out_dna.push_back(length(*Diter - center));
        }
//         for(auto Diter = DNAcenter.begin(); Diter != DNAcenter.end(); ++Diter)
//         {
//             double mindist(length(*Diter - *(histone_core.begin())));
//             for(auto Hiter = histone_core.begin()+1; Hiter != histone_core.end();
//                     ++Hiter)
//             {
//                 double dist = length(*Diter - *Hiter);
//                 if(mindist > dist)
//                     mindist = dist;
//             }
//             out_dna.push_back(mindist);
//         }

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
