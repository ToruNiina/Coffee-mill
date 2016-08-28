/*!
  @file NinfoData.hpp
  @brief definition of a class that is a set of data ninfo file contains.

  NinfoData is a wrapped map that key_type is NinfoKind(enum) and value_type is
  NinfoBlock(vector of ninfobase*).
  this contains one Block for each NinfoKind. normally, this has all the
  information that one ninfo file contains.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 18:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_NINFO_DATA
#define COFFEE_MILL_NINFO_DATA
#include "NinfoBase.hpp"
#include <memory>
#include <map>

namespace coffeemill
{

//! Ninfo data class.
/*!
 *  NinfoData is a map of NinfoKind and NinfoBlock. normally, this can contain
 *  all the information of one ninfo file.
 */
template<typename T = DefaultTraits>
class NinfoData
{
  public:
    using traits_type    = T;
    using ninfobase_type = NinfoBase<traits_type>;
    using key_type       = NinfoKind;
    using value_type     = std::vector<std::shared_ptr<ninfobase_type>>;
    using container_type = std::map<key_type, value_type>;
    using iterator       = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

  public:
    NinfoData(){}
    ~NinfoData() = default;

    bool        empty() const {return data_.empty();}
    std::size_t size()  const {return data_.size();}
    void        clear()       {return data_.clear();}
    void        emplace(const key_type key, const value_type& value)
    {data_.emplace(key, value);}

    //! access to block
    value_type&       operator[](const key_type key) {return data_[key];}
    value_type const& at(const key_type key) const   {return data_.at(key);}

    //! find the block. if not found, return this->end().
          iterator find(const key_type key)       {return data_.find(key);}
    const_iterator find(const key_type key) const {return data_.find(key);}
   
    //! access to whole data.
          container_type& data()       {return data_;}
    const container_type& data() const {return data_;}

    iterator begin() {return data_.begin();}
    iterator end()   {return data_.end();}
    const_iterator cbegin() const {return data_.cbegin();}
    const_iterator cend()   const {return data_.cend();}

  private:

    container_type data_; //!< ninfo data.

  public:

    constexpr static const char* const bond      = "native bond length";
    constexpr static const char* const angl      = "native bond angles";
    constexpr static const char* const dihd      = "native dihedral angles";
    constexpr static const char* const aicg13    = "1-3 contacts with L_AICG2 or L_AICG2_PLUS";
    constexpr static const char* const aicg14    = "1-4 contacts with L_AICG2";
    constexpr static const char* const aicg14p   = "1-4 contacts with L_AICG2_PLUS";
    constexpr static const char* const contact   = "native contact";
    constexpr static const char* const basepair  = "native basepair";
    constexpr static const char* const basestack = "native basestack";
};

}//coffeemill

#endif /* COFFEE_MILL_NINFO_DATA */
