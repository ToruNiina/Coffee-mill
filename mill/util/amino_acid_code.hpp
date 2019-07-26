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

    char               at(const std::string& s) const {return codes.at(s);}
    std::string const& at(const char s) const
    {
        const auto found = std::find_if(codes.cbegin(), codes.cend(),
            [=](const std::pair<std::string, char>& p){return p.second==s;}
            );
        if(found != codes.cend())
        {
            return found->first;
        }
        else
        {
            throw std::out_of_range("invalid amino acid code");
        }
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
