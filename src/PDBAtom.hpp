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
#include "math/LinearAlgebra.hpp"
#include <iostream>
#include <string>

namespace coffeemill
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
 *  |79-80    | charge              | 15.93   |
 *
 */

/*
 * ATOM      1  N_  MET A   1_     12.969  18.506  30.954  1.00 15.93           N  _
 *    ^      ^  ^^    ^ ^   ^^          ^       ^       ^     ^     ^           ^  ^
 *    +------+--++----+-+---++----------+-------+-------+-----+-----+-----------+--+--prefix
 *           +--++----+-+---++----------+-------+-------+-----+-----+-----------+--+--atom_id
 *              ++----+-+---++----------+-------+-------+-----+-----+-----------+--+--atom
 *               +----+-+---++----------+-------+-------+-----+-----+-----------+--+--altloc
 *                    +-+---++----------+-------+-------+-----+-----+-----------+--+--residue
 *                      +---++----------+-------+-------+-----+-----+-----------+--+--chain_id
 *                          ++----------+-------+-------+-----+-----+-----------+--+--residue_id
 *                           +----------+-------+-------+-----+-----+-----------+--+--iCode
 *                                      +-------+-------+-----+-----+-----------+--+--x
 *                                              +-------+-----+-----+-----------+--+--y
 *                                                      +-----+-----+-----------+--+--z
 *                                                            +-----+-----------+--+--occupancy
 *                                                                  +-----------+--+--T_factor
 *                                                                              +--+--element
 *                                                                                 +--charge
 */
class PDBAtom
{
  public:
    //! ctor.
    PDBAtom(){}
    //! dtor.
    ~PDBAtom(){}

    //! atom id.
    std::size_t& atom_id()       {return atom_id_;}
    //! atom id.
    std::size_t  atom_id() const {return atom_id_;}
    //! residue id.
    std::size_t& residue_id()       {return residue_id_;}
    //! residue id.
    std::size_t  residue_id() const {return residue_id_;}
    //! chain id.
          std::string& chain_id()       {return chain_id_;}
    //! chain id.
    const std::string& chain_id() const {return chain_id_;}

    //! coordinate x
    double  x() const {return position_[0];}
    //! coordinate x
    double& x()       {return position_[0];}
    //! coordinate y
    double  y() const {return position_[1];}
    //! coordinate y
    double& y()       {return position_[1];}
    //! coordinate z
    double  z() const {return position_[2];}
    //! coordinate z
    double& z()       {return position_[2];}
    //! 3D coordinate as ax::Vector3d
          ax::Vector3d& pos()       {return position_;}
    const ax::Vector3d& pos() const {return position_;}

    //! temperature_factor
    double& temperature_factor()       {return temperature_factor_;}
    //! temperature_factor
    double  temperature_factor() const {return temperature_factor_;}
    //! occupancy
    double& occupancy()       {return occupancy_;}
    //! occupancy
    double  occupancy() const {return occupancy_;}
    //! name of atom
          std::string& atom()       {return atom_name_;}
    //! name of atom
    const std::string& atom() const {return atom_name_;}
    //! name of residue
          std::string& residue()       {return residue_name_;}
    //! name of residue
    const std::string& residue() const {return residue_name_;}
    //! prefix (ATOM)
          std::string& prefix()       {return prefix_;}
    //! prefix (ATOM)
    const std::string& prefix() const {return prefix_;}
    //! code for insertion
          std::string& icode()       {return icode_;}
    //! code for insertion
    const std::string& icode() const {return icode_;}
    //! element symbol
          std::string& element()       {return element_;}
    //! element symbol
    const std::string& element() const {return element_;}
    //! charge on the atom
          std::string& charge()       {return charge_;}
    //! charge on the atom
    const std::string& charge() const {return charge_;}
    //! altanative location indicator
    char& altloc()       {return altloc_;}
    //! altanative location indicator
    char  altloc() const {return altloc_;}

  private:

    char         altloc_;
    std::size_t  atom_id_;
    std::size_t  residue_id_;
    double       occupancy_;
    double       temperature_factor_;
    std::string  atom_name_;
    std::string  residue_name_;
    std::string  chain_id_;
    std::string  prefix_;
    std::string  icode_;
    std::string  element_;
    std::string  charge_;
    ax::Vector3d position_;
};

/*!
 * @brief read PDBAtom from string that include only one line.
 * @param line corresponds to ATOM line. read data from this.
 * @param atom PDBAtom to store the data. normally be modified.
 * @return the string is ATOM line or not. if not, atom will not be modified.
 */
bool operator>>(const std::string& line, PDBAtom& atom);

//! output stream operator. output PDB Atom line as pdb atom format.
std::ostream& operator<<(std::ostream& os, const PDBAtom& a);

/*!
 * @brief input stream operator.
 * use std::getline to read one line from is.
 * And then call std::string >> PDBAtom.
 * @param is input stream
 * @param atom PDBatom to store the data
 */
std::istream& operator>>(std::istream& is, PDBAtom& atom);

}
#endif // COFFEE_MILL_PDB_ATOM
