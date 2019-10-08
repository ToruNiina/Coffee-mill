/*!
  @file PDBChain.hpp
  @brief definition of pdb chain class

  PDBChain class contains shared pointer of PDBResidue as std::vector.
  this behaves like as std::vector and has chain_id method.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 13:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_PDB_CHAIN_HPP
#define COFFEE_MILL_PDB_CHAIN_HPP
#include "PDBResidue.hpp"

namespace mill
{

//! PDBChain class.
/*!
 * PDBChain contains shared pointer of PDBResidue as vector.
 * this behaves like std::vector.
 */
template<typename vectorT>
class PDBChain
{
  public:

    using size_type      = std::size_t;
    using index_type     = size_type;
    using vector_type    = vectorT;
    using residue_type   = PDBResidue<vector_type>;
    using container_type = std::vector<residue_type>;
    using iterator       = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

  public:

    PDBChain(){}
    explicit PDBChain(size_type size) : residues_(size){}
    ~PDBChain() = default;

    PDBChain(const PDBChain&) = default;
    PDBChain(PDBChain&&) = default;
    PDBChain& operator=(const PDBChain&) = default;
    PDBChain& operator=(PDBChain&&) = default;

    /*!
     * @brief chain_id of this chain.
     * NOTE: this class does not guarantee that all the PDBAtom
     *       included is the same chain.
     * @return the chain id of first atom line included in container.
     */
    const std::string& chain_id() const {return this->front().front().chain_id;}

    bool empty() const {return residues_.empty();}
    std::size_t size() const {return residues_.size();}
    void push_back(const residue_type& res);
    void emplace_back(residue_type&& res);
    void clear(){return residues_.clear();}

    residue_type const& at(index_type i) const {return residues_.at(i);}
    residue_type &      at(index_type i)       {return residues_.at(i);}
    residue_type const& operator[](index_type i) const {return residues_[i];}
    residue_type &      operator[](index_type i)       {return residues_[i];}

    residue_type const& front() const {return residues_.front();}
    residue_type &      front()       {return residues_.front();}
    residue_type const& back() const {return residues_.back();}
    residue_type &      back()       {return residues_.back();}

    iterator begin(){return residues_.begin();}
    iterator end()  {return residues_.end();}
    const_iterator cbegin() const {return residues_.cbegin();}
    const_iterator cend()   const {return residues_.cend();}

  private:

    container_type residues_;//!< array of shared pointer of residues
};

template<typename vectorT>
void PDBChain<vectorT>::push_back(const residue_type& res)
{
    if(not this->empty() && (res.chain_id() != this->chain_id()))
    {
        throw std::invalid_argument("emplace invalid residue into chain");
    }
    this->residues_.push_back(res);
    return;
}

template<typename vectorT>
void PDBChain<vectorT>::emplace_back(residue_type&& res)
{
    if(not this->empty() && (res.chain_id() != this->chain_id()))
    {
        throw std::invalid_argument("emplace invalid residue into chain");
    }
    this->residues_.emplace_back(std::forward(res));
    return;
}

}//mill
#endif //COFFEE_MILL_PDB_CHAIN
