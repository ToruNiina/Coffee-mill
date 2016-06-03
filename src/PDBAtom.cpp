#include "PDBAtom.hpp"
#include "includes/utility.hpp"
#include <iomanip>

namespace coffeemill
{

bool operator>>(const std::string& line, PDBAtom& atom)
{
    const std::string pref = line.substr(0,6);
    if(pref != "ATOM  ")
        return false;
    atom.prefix()  = pref;
    atom.atom_id() = std::stoi(line.substr(6, 5));
    atom.atom()    = line.substr(12, 4);
    atom.altloc()  = line[16];
    atom.residue() = line.substr(17,3);
    atom.chain_id() = line[21];
    atom.residue_id() = stoi(line.substr(22, 4));
    atom.icode() = line[26];
    double x, y, z;
    x = stod(line.substr(30, 8));
    y = stod(line.substr(38, 8));
    z = stod(line.substr(46, 8));
    atom.pos() = ax::Vector3d(x,y,z);

    try{atom.occupancy() = stod(line.substr(54, 6));}
    catch(std::exception& excpt){atom.occupancy() = 0e0;}

    try{atom.temperature_factor() = stod(line.substr(60, 6));}
    catch(std::exception& excpt){atom.temperature_factor() = 0e0;}

    try{atom.element() = line.substr(76,2);}
    catch(std::exception& excpt){atom.element() = "";}

    try{atom.charge() = line.substr(78,2);}
    catch(std::exception& excpt){atom.charge() = "";}

    return true;
}

std::ostream& operator<<(std::ostream& os, const PDBAtom& a)
{
    os << std::setw(6) << a.prefix();
    os << std::setw(5) << a.atom_id();
    os << " ";
    if(remove_all<' '>(a.atom()) == "CA")
    {
        os << " CA ";
    }
    else if(remove_all<' '>(a.atom()) == "DB" ||
            remove_all<' '>(a.atom()) == "DS" ||
            remove_all<' '>(a.atom()) == "DP")
    {
        os << std::setw(4) << std::left << remove_all<' '>(a.atom());
    }
    else
    {
        os << std::setw(4) << a.atom();
    }
    os << std::setw(1) << a.altloc();
    os << std::setw(3) << a.residue();
    os << " ";
    os << std::setw(1) << a.chain_id();
    os << std::setw(4) << a.residue_id();
    os << std::setw(1) << a.icode();
    os << "   ";
    os << std::setw(8) << std::fixed << std::setprecision(3)
       << a.x();
    os << std::setw(8) << std::fixed << std::setprecision(3)
       << a.y();
    os << std::setw(8) << std::fixed << std::setprecision(3)
       << a.z();
    os << std::setw(6) << std::fixed << std::setprecision(2)
       << a.occupancy();
    os << std::setw(6) << std::fixed << std::setprecision(2)
       << a.temperature_factor();
    os << "          ";
    os << std::setw(2) << a.element();
    os << std::setw(2) << a.charge();
    return os;
}

//ATOM      1  N   ARG A   2     735.862 773.197 574.291  1.00  0.00           N 
std::istream& operator>>(std::istream& is, PDBAtom& atom)
{
    while(!is.eof())
    {
        std::string line;
        std::getline(is, line);
        if(line >> atom)
            return is;
        else
            continue;
    }
    return is;
}

}
