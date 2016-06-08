#ifndef COFFEE_MILL_NINFO_READER
#define COFFEE_MILL_NINFO_READER
#include "NinfoData.hpp"

namespace coffeemill
{

class NinfoReader
{
  public:

    using data_type = NinfoData;

  public:

    NinfoReader(const std::string& filename)
        : filename_(filename)
    {}
    ~NinfoReader() = default;

    void read();

          std::string& filename()       {return filename_;}
    const std::string& filename() const {return filename_;}
          data_type& data()       {return data_;}
    const data_type& data() const {return data_;}
  
  private:
    std::string filename_;
    data_type   data_;
};

}
#endif //COFFEE_MILL_NINFO_READER
