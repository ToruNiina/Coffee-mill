#ifndef COFFEE_MILL_PDB_CHAIN
#define COFFEE_MILL_PDB_CHAIN
#include <boost/regex.hpp>
#include "PDBResidue.hpp"

namespace coffeemill
{

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

    PDBChain(){}
    PDBChain(size_type size) : residues_(size){}
    ~PDBChain() = default;

    const std::string& chain_id() const {return this->front()->front()->chain_id();}

    bool      empty() const {return residues_.empty();}
    size_type size()  const {return residues_.size();}
    void      push_back(const element_type& elem) {return residues_.push_back(elem);}

    const element_type& at(index_type i) const {return residues_.at(i);}
          element_type& at(index_type i)       {return residues_.at(i);}
    const element_type& operator[](index_type i) const {return residues_[i];}
          element_type& operator[](index_type i)       {return residues_[i];}
    const element_type& front() const {return residues_.front();}
          element_type& front()       {return residues_.front();}

    iterator begin(){return residues_.begin();}
    iterator end()  {return residues_.end();}
    const_iterator cbegin() const {return residues_.cbegin();}
    const_iterator cend()   const {return residues_.cend();}

  private:

    container_type residues_;
};

}
#endif //COFFEE_MILL_PDB_CHAIN
