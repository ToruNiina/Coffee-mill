#ifndef COFFEE_MILL_AMINO_ACID_CODE
#define COFFEE_MILL_AMINO_ACID_CODE
#include <algorithm>
#include <string>
#include <map>

namespace mill
{

struct amino_acid_code
{
    char operator()(std::string const& s) const {return codes.at(s);}
    std::string const& operator()(char s) const
    {
        return std::find_if(codes.cbegin(), codes.cend(),
            [=](const std::pair<std::string, char>& p){return p.second==s;}
            )->first;
    }

    std::map<std::string, char> codes = {
        {"ALA", 'A'},
        {"ASX", 'B'},
        {"CYS", 'C'},
        {"ASP", 'D'},
        {"GLU", 'E'},
        {"PHE", 'F'},
        {"GLY", 'G'},
        {"HIS", 'H'},
        {"ILE", 'I'},
        {"LYS", 'K'},
        {"LEU", 'L'},
        {"MET", 'M'},
        {"ASN", 'N'},
        {"PRO", 'P'},
        {"GLN", 'Q'},
        {"ARG", 'R'},
        {"SER", 'S'},
        {"THR", 'T'},
        {"SEC", 'U'},
        {"VAL", 'V'},
        {"TRP", 'W'},
        {"XAA", 'X'},
        {"TYR", 'Y'},
        {"GLX", 'Z'}
    };
};

} // mill
#endif /* COFFEE_MILL_AMINO_ACID_CODE */
