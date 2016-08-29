/*!
  @file PDBChain.hpp
  @brief definition of pdb chain class

  PDBChain class contains shared pointer of PDBResidue as std::vector.
  this behaves like as std::vector and has chain_id method.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 13:00
  @copyright Toru Niina 2016 on MIT License
*/



#ifndef COFFEE_MILL_PDB_CHAIN
#define COFFEE_MILL_PDB_CHAIN
#include "PDBResidue.hpp"

namespace coffeemill
{

//! PDBChain class.
/*!
 * PDBChain contains shared pointer of PDBResidue as vector. 
 * this behaves like std::vector.
 */
class PDBChain
{
  public:

    using size_type      = std::size_t;
    using index_type     = size_type;
    using element_type   = std::shared_ptr<PDBResidue>;
    using container_type = std::vector<element_type>;
    using iterator       = container_type::iterator;
    using const_iterator = container_type::const_iterator;

  public:

    //! ctor.
    PDBChain(){}
    /*!
     * @brief ctor. allocate the container with size
     * @param size number of residues. equal to size of the container.
    */
    PDBChain(size_type size) : residues_(size){}
    //! dtor.
    ~PDBChain() = default;

    /*!
     * @brief chain_id of this chain.
     * NOTE: this class does not guarantee that all the PDBAtom
     *       included is the same chain.
     * @return the chain id of first atom line included in container.
     */
    const std::string& chain_id() const {return this->front()->front()->chain_id();}

    //! return whether the container is empty or not
    bool      empty() const {return residues_.empty();}
    //! return the number of residues
    size_type size()  const {return residues_.size();}
    //! push new residue into the container.
    /*!
     * NOTE: this method does not check whether the ATOM line is in the same chain or not.
     *       User must guarantee the point.
     */
    void      push_back(const element_type& elem) {return residues_.push_back(elem);}
    //! clear the container
    void      clear(){return residues_.clear();}

    //! access to member
    const element_type& at(index_type i) const {return residues_.at(i);}
    //! access to member
          element_type& at(index_type i)       {return residues_.at(i);}
    //! access to member
    const element_type& operator[](index_type i) const {return residues_[i];}
    //! access to member
          element_type& operator[](index_type i)       {return residues_[i];}

    //! access to the 1st member
    const element_type& front() const {return residues_.front();}
    //! access to the 1st member
          element_type& front()       {return residues_.front();}
    //! access to the last member
    const element_type& back() const {return residues_.back();}
    //! access to the last member
          element_type& back()       {return residues_.back();}

    //! iterator
    iterator begin(){return residues_.begin();}
    //! iterator
    iterator end()  {return residues_.end();}
    //! iterator
    const_iterator cbegin() const {return residues_.cbegin();}
    //! iterator
    const_iterator cend()   const {return residues_.cend();}

  private:

    container_type residues_;//!< array of shared pointer of residues
};

}
#endif //COFFEE_MILL_PDB_CHAIN
