#ifndef COFFEE_MILL_PROTEIN_SEQUENCE
#define COFFEE_MILL_PROTEIN_SEQUENCE
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

namespace coffeemill
{
    class ProteinSeq
    {
        public:

            ProteinSeq();
            ~ProteinSeq(){}

            char pseq_3to1(const std::string& aacode) const;
            std::string pseq_1to3(const char aacode);//it tooks some time
            bool find(const std::string& aacode);

        private:

            std::map<std::string, char> sequence_map;
    };

    ProteinSeq::ProteinSeq()
    {
        sequence_map["ALA"] = 'A';
        sequence_map["ASX"] = 'B';
        sequence_map["CYS"] = 'C';
        sequence_map["ASP"] = 'D';
        sequence_map["GLU"] = 'E';
        sequence_map["PHE"] = 'F';
        sequence_map["GLY"] = 'G';
        sequence_map["HIS"] = 'H';
        sequence_map["ILE"] = 'I';
        sequence_map["LYS"] = 'K';
        sequence_map["LEU"] = 'L';
        sequence_map["MET"] = 'M';
        sequence_map["ASN"] = 'N';
        sequence_map["PRO"] = 'P';
        sequence_map["GLN"] = 'Q';
        sequence_map["ARG"] = 'R';
        sequence_map["SER"] = 'S';
        sequence_map["THR"] = 'T';
        sequence_map["SEC"] = 'U';
        sequence_map["VAL"] = 'V';
        sequence_map["TRP"] = 'W';
        sequence_map["XAA"] = 'X';
        sequence_map["TYR"] = 'Y';
        sequence_map["GLX"] = 'Z';
    }

    char ProteinSeq::pseq_3to1(const std::string& aacode) const
    {
        std::string capital;
        capital.resize(aacode.size());
        std::transform(aacode.cbegin(), aacode.cend(),
                       capital.begin(), toupper);
        return sequence_map.at(capital);
    }//return 1 letter code in capital

    std::string ProteinSeq::pseq_1to3(const char aacode)
    {
        char capital(toupper(aacode));
        for(std::map<std::string, char>::iterator iter = sequence_map.begin();
            iter != sequence_map.end(); ++iter)
        {
            if((*iter).second == capital) return (*iter).first;
        }
        std::cout << "unknown code: " << aacode << std::endl;
        throw std::invalid_argument("unknown amino acid 1 letter code");
    }

    bool ProteinSeq::find(const std::string& aacode)
    {
        std::string capital;
        capital.resize(aacode.size());
        std::transform(aacode.cbegin(), aacode.cend(),
                       capital.begin(), toupper);

        std::map<std::string, char>::iterator iter = sequence_map.find(capital);
        return (iter != sequence_map.end());
    }

} //end namespace coffeemill
#endif // COFFEE_MILL_PROTEIN_SEQUENCE
