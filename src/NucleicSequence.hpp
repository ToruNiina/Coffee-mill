#ifndef COFFEE_MILL_DNA_SEQUENCE
#define COFFEE_MILL_DNA_SEQUENCE
#include <string>
#include <algorithm>
#include "utility.hpp"

namespace coffeemill
{

class NucleicSequence
{
  public:
    using size_type      = std::size_t;
    using index_type     = size_type;
    using element_type   = char;
    using sequence_type  = std::string;
    using iterator       = sequence_type::iterator;
    using const_iterator = sequence_type::const_iterator;

  public:
    NucleicSequence(){}
    NucleicSequence(const std::string& seq) : seq_(seq){}
    ~NucleicSequence() = default;

    bool is_DNA() const {return is_composed_of(seq_, DNAbase_);}
    bool is_RNA() const {return is_composed_of(seq_, RNAbase_);}
    bool is_palindromic() const {return seq_ == this->reverse();}

    std::string reverse() const {return std::string(seq_.rbegin(), seq_.rend());}
    std::string anti()    const
    {
        std::string retval("3-");
        for(auto iter = this->cbegin(); iter != this->cend(); ++iter)
            switch(*iter)
            {
                case 'A':
                    if(this->is_DNA())
                        retval += 'T';
                    else
                        retval += 'U';
                    break;
                case 'T':
                    retval += 'A';
                    break;
                case 'U':
                    retval += 'A';
                    break;
                case 'C':
                    retval += 'G';
                    break;
                case 'G':
                    retval += 'C';
                    break;
                default:
                    throw std::invalid_argument("invalid sequence: " + seq_);
            }
        retval += '-'; retval += '5';
        return retval;
    }

    std::string complemental() const
    {
        std::string anti = this->anti();
        return std::string(anti.rbegin(), anti.rend());
    }

    bool      empty() const {return seq_.empty();}
    size_type size()  const {return seq_.size();}
    void      clear()       {return seq_.clear();}
    const element_type& at(index_type i) const {return seq_.at(i);}
          element_type& at(index_type i)       {return seq_.at(i);}
    const element_type& operator[](index_type i) const {return seq_[i];}
          element_type& operator[](index_type i)       {return seq_[i];}
    const element_type& front() const {return seq_.front();}
          element_type& front()       {return seq_.front();}
    const element_type& back() const {return seq_.back();}
          element_type& back()       {return seq_.back();}

          iterator begin()  {return seq_.begin();}
          iterator end()    {return seq_.end();}
    const_iterator cbegin() const {return seq_.cbegin();}
    const_iterator cend()   const {return seq_.cend();}

    const std::string& seq() const {return seq_;}
          std::string& seq()       {return seq_;}

  private:
    const std::string DNAbase_ = "ATCG";
    const std::string RNAbase_ = "AUCG";
    std::string seq_;
};



}

#endif /* COFFEE_MILL_DNA_SEQUENCE */
