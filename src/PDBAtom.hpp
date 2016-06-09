#ifndef COFFEE_MILL_PDB_ATOM
#define COFFEE_MILL_PDB_ATOM
#include "../includes/math/LinearAlgebra.hpp"
#include <iostream>
#include <string>

namespace coffeemill
{

class PDBAtom
{
  public:
    PDBAtom(){}
    ~PDBAtom(){}

          std::size_t& atom_id()       {return atom_id_;}
    const std::size_t& atom_id() const {return atom_id_;}
          std::size_t& residue_id()       {return residue_id_;}
    const std::size_t& residue_id() const {return residue_id_;}
          std::string& chain_id()       {return chain_id_;}
    const std::string& chain_id() const {return chain_id_;}

    double  x() const {return position_[0];}
    double& x()       {return position_[0];}
    double  y() const {return position_[1];}
    double& y()       {return position_[1];}
    double  z() const {return position_[2];}
    double& z()       {return position_[2];}
          ax::Vector3d& pos()       {return position_;}
    const ax::Vector3d& pos() const {return position_;}

          double& temperature_factor()       {return temperature_factor_;}
    const double& temperature_factor() const {return temperature_factor_;}
          double& occupancy()       {return occupancy_;}
    const double& occupancy() const {return occupancy_;}
          std::string& atom()       {return atom_name_;}
    const std::string& atom() const {return atom_name_;}
          std::string& residue()       {return residue_name_;}
    const std::string& residue() const {return residue_name_;}
          std::string& prefix()       {return prefix_;}
    const std::string& prefix() const {return prefix_;}
          std::string& icode()       {return icode_;}
    const std::string& icode() const {return icode_;}
          std::string& element()       {return element_;}
    const std::string& element() const {return element_;}
          std::string& charge()       {return charge_;}
    const std::string& charge() const {return charge_;}
    char& altloc()       {return altloc_;}
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

bool operator>>(const std::string& line, PDBAtom& atom);
std::ostream& operator<<(std::ostream& os, const PDBAtom& a);
std::istream& operator>>(std::istream& is, PDBAtom& atom);

}
#endif // COFFEE_MILL_PDB_ATOM
