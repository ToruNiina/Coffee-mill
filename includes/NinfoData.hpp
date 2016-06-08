#ifndef COFFEE_MILL_NINFO_DATA
#define COFFEE_MILL_NINFO_DATA
#include "NinfoBase.hpp"
#include <memory>
#include <map>

namespace coffeemill
{

using NinfoSptr  = std::shared_ptr<NinfoBase>;
using NinfoBlock = std::vector<NinfoSptr>;

class NinfoData
{
  public:
    using key_type       = NinfoKind;
    using value_type     = NinfoBlock;
    using container_type = std::map<key_type, value_type>;
    using iterator       = container_type::iterator;
    using const_iterator = container_type::const_iterator;

  public:
    NinfoData(){}
    ~NinfoData() = default;


    bool        empty() const {return data_.empty();}
    std::size_t size()  const {return data_.size();}
    void        clear()       {return data_.clear();}
    void        emplace(const key_type key, const value_type& value)
    {data_.emplace(key, value);}

    value_type& operator[](const key_type key) {return data_[key];}
    const value_type& at(const key_type key) {return data_.at(key);}

          iterator find(const key_type key)       {return data_.find(key);}
    const_iterator find(const key_type key) const {return data_.find(key);}
   
          container_type& data()       {return data_;}
    const container_type& data() const {return data_;}

    iterator begin() {return data_.begin();}
    iterator end()   {return data_.end();}
    const_iterator cbegin() const {return data_.cbegin();}
    const_iterator cend()   const {return data_.cend();}


  private:

    container_type data_;
};



}

#endif /* COFFEE_MILL_NINFO_DATA */
