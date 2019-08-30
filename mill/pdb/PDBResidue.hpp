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

#ifndef COFFEE_MILL_PDB_RESIDUE_HPP
#define COFFEE_MILL_PDB_RESIDUE_HPP
#include "PDBAtom.hpp"
#include <vector>
#include <stdexcept>

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

    int  residue_id() const {return atoms_.front().residue_id;}
    std::string const& residue_name() const {return atoms_.front().residue_name;}
    std::string const& chain_id() const {return atoms_.front().chain_id;}

    std::size_t size() const {return atoms_.size();}
    bool       empty() const {return atoms_.empty();}
    void clear(){atoms_.clear();}
    void resize(const std::size_t s){atoms_.reserve(s);}
    void reserve(const std::size_t s){atoms_.reserve(s);}

    void push_back(const atom_type& atom);
    void emplace_back(atom_type&& atom);

    atom_type &      at(const std::size_t i)       {return atoms_.at(i);}
    atom_type const& at(const std::size_t i) const {return atoms_.at(i);}
    atom_type &      operator[](const std::size_t i)       {return atoms_[i];}
    atom_type const& operator[](const std::size_t i) const {return atoms_[i];}
    atom_type &      front()       {return atoms_.front();}
    atom_type const& front() const {return atoms_.front();}
    atom_type &      back()       {return atoms_.back();}
    atom_type const& back() const {return atoms_.back();}
    iterator begin() {return atoms_.begin();}
    iterator end()   {return atoms_.end();}
    const_iterator cbegin() const {return atoms_.cbegin();}
    const_iterator cend()   const {return atoms_.cend();}

  private:
    container_type atoms_;
};

template<typename vectorT>
void PDBResidue<vectorT>::push_back(const atom_type& atom)
{
    if(not atoms_.empty() && (this->residue_id() != atom.residue_id ||
                              this->residue_name() != atom.residue_name ||
                              this->chain_id() != atom.chain_id))
    {
        throw std::invalid_argument("emplace invalid atom to residue");
    }
    atoms_.push_back(atom);
    return;
}

template<typename vectorT>
void PDBResidue<vectorT>::emplace_back(atom_type&& atom)
{
    if(not atoms_.empty() && (this->residue_id() != atom.residue_id ||
                              this->residue_name() != atom.residue_name ||
                              this->chain_id() != atom.chain_id))
    {
        throw std::invalid_argument("emplace invalid atom to residue");
    }
    atoms_.emplace_back(std::forward(atom));
    return;
}

} // mill
#endif /* COFFEE_MILL_PDB_RESIDUE */
