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

//! definition of shared ptr of NinfoBase. this represents a line in ninfo file.
using NinfoSptr  = std::shared_ptr<NinfoBase>;
//! definition of Block. array of NinfoSptr represents a block in ninfo file.
using NinfoBlock = std::vector<NinfoSptr>;

//! Ninfo data class.
/*!
 *  NinfoData is a map of NinfoKind and NinfoBlock. normally, this can contain
 *  all the information of one ninfo file.
 */
class NinfoData
{
  public:
    using key_type       = NinfoKind;
    using value_type     = NinfoBlock;
    using container_type = std::map<key_type, value_type>;
    using iterator       = container_type::iterator;
    using const_iterator = container_type::const_iterator;

  public:
    //! ctor.
    NinfoData(){}
    //! dtor.
    ~NinfoData() = default;

    //! whether the map is empty or not.
    bool        empty() const {return data_.empty();}
    //! the size of map.
    std::size_t size()  const {return data_.size();}
    //! clear all the data.
    void        clear()       {return data_.clear();}
    //! create new block.
    void        emplace(const key_type key, const value_type& value)
    {data_.emplace(key, value);}

    //! access to block
    value_type& operator[](const key_type key) {return data_[key];}
    //! access to block
    const value_type& at(const key_type key) {return data_.at(key);}

    //! find the block. if not found, return this->end().
          iterator find(const key_type key)       {return data_.find(key);}
    //! find the block. if not found, return this->cend().
    const_iterator find(const key_type key) const {return data_.find(key);}
   
    //! access to whole data.
          container_type& data()       {return data_;}
    //! access to whole data.
    const container_type& data() const {return data_;}

    //! iterator begin of a map<NinfoKind, NinfoBlock>..
    iterator begin() {return data_.begin();}
    //! iterator end of a map<NinfoKind, NinfoBlock>..
    iterator end()   {return data_.end();}
    //! const_iterator cbegin of a map<NinfoKind, NinfoBlock>.
    const_iterator cbegin() const {return data_.cbegin();}
    //! const_iterator cend of a map<NinfoKind, NinfoBlock>..
    const_iterator cend()   const {return data_.cend();}

  private:

    container_type data_; //!< ninfo data.

  public:

    static const std::string bond;     //!< <<<< "block name" of bond block.
    static const std::string angl;     //!< <<<< "block name" of angl block.
    static const std::string dihd;     //!< <<<< "block name" of dihd block.
    static const std::string aicg13;   //!< <<<< "block name" of aicg13 block.
    static const std::string aicg14;   //!< <<<< "block name" of aicg14 block.
    static const std::string aicg14p;  //!< <<<< "block name" of aicg14p block.
    static const std::string contact;  //!< <<<< "block name" of contact block.
    static const std::string basepair; //!< <<<< "block name" of basepair block.
    static const std::string basestack;//!< <<<< "block name" of basestack block.
};



}

#endif /* COFFEE_MILL_NINFO_DATA */
