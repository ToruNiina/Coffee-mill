#ifndef COFFEE_MILL_UTIL_AMINO_ACID_CODE_HPP
#define COFFEE_MILL_UTIL_AMINO_ACID_CODE_HPP
#include <algorithm>
#include <stdexcept>
#include <string>
#include <map>

namespace mill
{

inline const std::map<std::string, char> amino_acid_3to1{
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

inline const std::map<char, std::string> amino_acid_1to3{
    {'A', "ALA"},
    {'B', "ASX"},
    {'C', "CYS"},
    {'D', "ASP"},
    {'E', "GLU"},
    {'F', "PHE"},
    {'G', "GLY"},
    {'H', "HIS"},
    {'I', "ILE"},
    {'K', "LYS"},
    {'L', "LEU"},
    {'M', "MET"},
    {'N', "ASN"},
    {'P', "PRO"},
    {'Q', "GLN"},
    {'R', "ARG"},
    {'S', "SER"},
    {'T', "THR"},
    {'U', "SEC"},
    {'V', "VAL"},
    {'W', "TRP"},
    {'X', "XAA"},
    {'Y', "TYR"},
    {'Z', "GLX"}
};

} // mill
#endif /* COFFEE_MILL_AMINO_ACID_CODE */
