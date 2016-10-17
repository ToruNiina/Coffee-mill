/*!
  @file PDBResidue.hpp
  @brief definition of pdb residue class

  PDBResidue class has several shared pointer of PDBAtom as std::vector.
  this behaves like std::vector.
  Note that this class does not guarantee the member PDBAtoms are the member of
  the same residue and the Atoms are sorted. User must guarantee the point.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 13:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_PDB_RESIDUE
#define COFFEE_MILL_PDB_RESIDUE
#include "PDBAtom.hpp"
#include <vector>
#include <memory>

namespace mill
{

//! PDBResidue class.
/*!
 * PDBResidue contains shared pointer of PDBAtom as vector. 
 * this behaves like std::vector.
 */
template<typename vectorT>
class PDBResidue
{
  public:
    using vector_type    = vectorT;
    using atom_type      = PDBAtom<vector_type>;
    using container_type = std::vector<atom_type>;
    using iterator       = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

    PDBResidue() = default;
    explicit PDBResidue(std::size_t size) : atoms_(size){}
    ~PDBResidue() = default;

    PDBResidue(PDBResidue const&) = default;
    PDBResidue(PDBResidue&&) = default;
    PDBResidue& operator=(PDBResidue const&) = default;
    PDBResidue& operator=(PDBResidue&&) = default;

    int  residue_id() const {return residue_id_;}
    int& residue_id()       {return residue_id_;}
    std::string const& residue_name() const {return residue_name_;}
    std::string&       residue_name()       {return residue_name_;}
    std::string const& chain_id() const {return chain_id_;}
    std::string &      chain_id()       {return chain_id_;}

    std::size_t size() const {return atoms_.size();}
    bool       empty() const {return atoms_.empty();}
    void clear(){atoms_.clear();}
    void resize(const std::size_t s){atoms_.reserve(s);}
    void reserve(const std::size_t s){atoms_.reserve(s);}

    void push_back(const atom_type& atom){atoms_.push_back(atom);}
    void emplace_back(atom_type&& atom){atoms_.emplace_back(std::forward(atom));}

    atom_type &      at(const std::size_t i)       {return atoms_.at(i);}
    atom_type const& at(const std::size_t i) const {return atoms_.at(i);}
    atom_type &      operator[](const std::size_t i)       {return atoms_[i];}
    atom_type const& operator[](const std::size_t i) const {return atoms_[i];}
    iterator begin() {return atoms_.begin();}
    iterator end()   {return atoms_.end();}
    const_iterator cbegin() const {return atoms_.cbegin();}
    const_iterator cend()   const {return atoms_.cend();}

  private:
    int residue_id_;
    std::string residue_name_;
    std::string chain_id_;
    container_type atoms_;
};

} // mill
#endif /* COFFEE_MILL_PDB_RESIDUE */
