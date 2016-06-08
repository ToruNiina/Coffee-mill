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

    NinfoReader(){}
    NinfoReader(const std::string& filename)
        : filename_(filename)
    {}
    ~NinfoReader() = default;

    void read();
    void read(const std::string& filename){filename_ = filename; return this->read();}

          std::string& filename()       {return filename_;}
    const std::string& filename() const {return filename_;}
          data_type& data()       {return data_;}
    const data_type& data() const {return data_;}

  private:
    void       read_block(std::ifstream& ifs, const std::string& line);
    template<typename T_ninfo>
    NinfoBlock read_block(std::ifstream& ifs) const;
    NinfoKind  judge_block(const std::string& line) const;
  
  private:
    std::string filename_;
    data_type   data_;

};

}
#endif //COFFEE_MILL_NINFO_READER
