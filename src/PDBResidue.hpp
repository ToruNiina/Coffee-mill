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
#include <memory>

namespace coffeemill
{

//! PDBResidue class.
/*!
 * PDBResidue contains shared pointer of PDBAtom as vector. 
 * this behaves like std::vector.
 */
class PDBResidue
{
  public:
    using size_type      = std::size_t;
    using index_type     = size_type;
    using element_type   = std::shared_ptr<PDBAtom>;
    using container_type = std::vector<element_type>;
    using iterator       = container_type::iterator;
    using const_iterator = container_type::const_iterator;

  public:

    //! ctor
    PDBResidue(){}
    /*!
     * @brief ctor. allocate the container with size
     * @param size number of residues. equal to size of the container.
    */
    PDBResidue(size_type size) : atoms_(size){}
    //! dtor
    ~PDBResidue() = default;

    /*!
     * @brief residue id of this residue.
     * NOTE: this class does not guarantee the atoms are in the same residue.
     * @return residue id of the 1st PDBAtom.
     */
    size_type          residue_id() const {return atoms_.front()->residue_id();}
    /*!
     * @brief residue name of this residue.
     * NOTE: this class does not guarantee the atoms are in the same residue.
     * @return residue name of the 1st PDBAtom.
     */
    const std::string& residue()    const {return atoms_.front()->residue();}
    /*!
     * @brief chain id of this residue.
     * NOTE: this class does not guarantee the atoms are in the same chain.
     * @return chain id of the 1st PDBAtom.
     */
    const std::string& chain_id()   const {return atoms_.front()->chain_id();}

    //! return whether the container is empty or not
    bool      empty() const {return atoms_.empty();}
    //! return the number of atoms
    size_type size()  const {return atoms_.size();}
    //! clear the container
    void clear(){return atoms_.clear();}
    /*!
     * @brief push new residue into the container.
     * NOTE: this method does not check whether the ATOM line is
     *       in the same residue or not. User must guarantee the point.
     */
    void push_back(const element_type& atom){return atoms_.push_back(atom);}

    //! access to the 1st atom
    const element_type& front() const {return atoms_.front();}
    //! access to the 1st atom
          element_type& front()       {return atoms_.front();}
    //! access to the last atom
    const element_type& back() const {return atoms_.back();}
    //! access to the last atom
          element_type& back()       {return atoms_.back();}
    //! access to the i-th member
    const element_type& at(index_type i) const {return atoms_.at(i);}
    //! access to the i-th member
          element_type& at(index_type i)       {return atoms_.at(i);}
    //! access to the i-th member
    const element_type& operator[](index_type i) const {return atoms_[i];}
    //! access to the i-th member
          element_type& operator[](index_type i)       {return atoms_[i];}

    //! iterator
    iterator begin() {return atoms_.begin();}
    //! iterator
    iterator end()   {return atoms_.end();}
    //! iterator
    const_iterator cbegin() const {return atoms_.cbegin();}
    //! iterator
    const_iterator cend()   const {return atoms_.cend();}

  private:
    container_type atoms_;//!< array of *atoms
};



}
#endif /* COFFEE_MILL_PDB_RESIDUE */
