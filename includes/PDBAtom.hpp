/*!
  @file PDBAtom.hpp
  @brief definition of a class that contains information of pdb atom line.

  PDBAtom class contains information of atom line of pdb file.
  This file contains a definition of io stream operator<< and >>.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 13:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_PDB_ATOM
#define COFFEE_MILL_PDB_ATOM
#include "detail/scalar_type_extractor.hpp"
#include "utility.hpp"
#include <iostream>
#include <iomanip>
#include <string>

namespace mill
{

//! PDBAtom class.
/*!
 *  PDBAtom contains information of one atom line in pdb file.
 *  the name of data is listed below. @n
 *  @n
 *  example
 *  ATOM      1  N_  MET A   1_     12.969  18.506  30.954  1.00 15.93           N  _
 *
 *  | columns | name                | example |
 *  |:--------|:--------------------|:--------|
 *  |1-6      | prefix              | ATOM    |
 *  |7-11     | atom_id             | 1       |
 *  |13-16    | atom                | N       |
 *  |17       | altloc              |         |
 *  |18-20    | residue             | MET     |
 *  |22       | chain_id            | A       |
 *  |23-26    | redidue_id          | 1       |
 *  |27       | icode               |         |
 *  |31-38    | x                   | 12.969  |
 *  |39-46    | y                   | 18.506  |
 *  |47-54    | z                   | 30.954  |
 *  |55-60    | occupancy           | 1.00    |
 *  |61-66    | temperature_factor  | 15.93   |
 *  |77-78    | element             | N       |
 *  |79-80    | charge              |         |
 */

template<typename vectorT>
struct PDBAtom
{
    using vector_type = vectorT;
    using real_type   = typename scalar_type_extractor<vector_type>::type;
    using position_type = vector_type;

    char          altloc;
    char          icode;
    int           atom_id;
    int           residue_id;
    real_type     occupancy;
    real_type     temperature_factor;
    std::string   prefix;
    std::string   atom_name;
    std::string   residue_name;
    std::string   chain_id;
    std::string   element;
    std::string   charge;
    position_type position;
};

template<typename vectorT>
PDBAtom<vectorT> make_default_atom(const int id, const vectorT& pos)
{
    PDBAtom<vectorT> atom;
    atom.altloc = ' ';
    atom.icode = ' ';
    atom.atom_id = id;
    atom.residue_id = id;
    atom.occupancy = 0.0;
    atom.temperature_factor = 0.0;
    atom.prefix = "ATOM";
    atom.atom_name = "CA";
    atom.residue_name = "XXX";
    atom.chain_id = "A";
    atom.element = "C";
    atom.charge = "";
    atom.position = pos;
    return atom;
}

//! output stream operator. output PDB Atom line as pdb atom format.
template<typename vector_type>
std::basic_ostream<char>&
operator<<(std::basic_ostream<char>& os, const PDBAtom<vector_type>& a)
{
    os << std::setw(6) << std::left << a.prefix;
    os << std::setw(5) << std::right << a.atom_id;
    os << " ";
    std::string atom_name = remove_all(' ', a.atom_name);
    if(atom_name == "CA")
        os << std::setw(4) << " CA ";
    else if(atom_name == "DB" || atom_name == "DS" || atom_name == "DP")
        os << std::setw(4) << std::left << atom_name;
    else
        os << std::setw(4) << a.atom_name;

    os << std::setw(1) << a.altloc;
    os << std::setw(3) << std::right << a.residue_name;
    os << " ";
    os << std::setw(1) << a.chain_id;
    os << std::setw(4) << std::right << a.residue_id;
    os << std::setw(1) << a.icode;
    os << "   ";
    os << std::setw(8) << std::fixed << std::setprecision(3) << std::right
       << a.position[0];
    os << std::setw(8) << std::fixed << std::setprecision(3) << std::right
       << a.position[1];
    os << std::setw(8) << std::fixed << std::setprecision(3) << std::right
       << a.position[2];
    os << std::setw(6) << std::fixed << std::setprecision(2) << std::right
       << a.occupancy;
    os << std::setw(6) << std::fixed << std::setprecision(2) << std::right
       << a.temperature_factor;
    os << "          ";
    os << std::setw(2) << a.element;
    os << std::setw(2) << a.charge;
    return os;
}

/*!
 * @brief input stream operator.
 * use std::getline to read one line from is.
 * And then call std::string >> PDBAtom.
 * @param is input stream
 * @param atom PDBatom to store the data
 */
template<typename vector_type>
std::basic_istream<char>&
operator>>(std::basic_istream<char>& is, PDBAtom<vector_type>& atom)
{
    while(!is.eof())
    {
        std::string line;
        std::getline(is, line);
        if(line >> atom) return is;
        else continue;
    }
    return is;
}

/*!
 * @brief read PDBAtom from string that include only one line.
 * @param line corresponds to ATOM line. read data from this.
 * @param atom PDBAtom to store the data. normally be modified.
 * @return the string is ATOM line or not. if not, atom will not be modified.
 */
template<typename vector_type>
bool operator>>(const std::string& line, PDBAtom<vector_type>& atom)
{
    const std::string pref = line.substr(0,6);
    if(pref != "ATOM  " && pref != "HETATM") return false;
    atom.prefix       = pref;
    atom.atom_id      = std::stoi(line.substr(6, 5));
    atom.atom_name    = line.substr(12, 4);
    atom.altloc       = line[16];
    atom.residue_name = line.substr(17,3);
    atom.chain_id     = line[21];
    atom.residue_id   = stoi(line.substr(22, 4));
    atom.icode        = line[26];
    typename PDBAtom<vector_type>::real_type x, y, z;
    x = stod(line.substr(30, 8));
    y = stod(line.substr(38, 8));
    z = stod(line.substr(46, 8));
    const typename PDBAtom<vector_type>::position_type pos(x, y, z);
    atom.position = pos;

    try{atom.occupancy = stod(line.substr(54, 6));}
    catch(std::exception& excpt){atom.occupancy = 0e0;}

    try{atom.temperature_factor = stod(line.substr(60, 6));}
    catch(std::exception& excpt){atom.temperature_factor = 0e0;}

    try{atom.element = line.substr(76,2);}
    catch(std::exception& excpt){atom.element = "";}

    try{atom.charge = line.substr(78,2);}
    catch(std::exception& excpt){atom.charge = "";}

    return true;
}

}//coffeemill
#endif // COFFEE_MILL_PDB_ATOM
