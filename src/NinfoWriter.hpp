#include "NinfoData.hpp"
#include <fstream>

namespace coffeemill
{

class NinfoWriter
{
  public:
    NinfoWriter(){}
    NinfoWriter(const std::string& filename): filename_(filename){}
    ~NinfoWriter() = default;

    void write(const NinfoData& data) const;

          std::string& filename()       {return filename_;}
    const std::string& filename() const {return filename_;}

  private:
    template<typename T_ninfo>
    void write_block(std::ofstream& ofs, const NinfoBlock& block) const;

  private:
    std::string filename_;
};

}
