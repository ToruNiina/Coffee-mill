#ifndef COFFEE_MILL_PDB_RESIDUE
#define COFFEE_MILL_PDB_RESIDUE
#include "PDBAtom.hpp"
#include <memory>

namespace coffeemill
{

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
    PDBResidue(){}
    PDBResidue(size_type size) : atoms_(size){}
    ~PDBResidue() = default;

    size_type   residue_id() const {return atoms_.front()->residue_id();}
    std::string residue()    const {return atoms_.front()->residue();}

    bool      empty() const {return atoms_.empty();}
    size_type size()  const {return atoms_.size();}
    void clear()                            {return atoms_.clear();}
    void push_back(const element_type& atom){return atoms_.push_back(atom);}

    const element_type& front() const {return atoms_.front();}
          element_type& front()       {return atoms_.front();}
    const element_type& at(index_type i) const {return atoms_.at(i);}
          element_type& at(index_type i)       {return atoms_.at(i);}
    const element_type& operator[](index_type i) const {return atoms_[i];}
          element_type& operator[](index_type i)       {return atoms_[i];}

    iterator begin() {return atoms_.begin();}
    iterator end()   {return atoms_.end();}
    const_iterator cbegin() const {return atoms_.cbegin();}
    const_iterator cend()   const {return atoms_.cend();}

  private:
    container_type atoms_;
};



}
#endif /* COFFEE_MILL_PDB_RESIDUE */
