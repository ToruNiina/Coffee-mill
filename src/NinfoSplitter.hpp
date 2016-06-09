#ifndef COFFEE_MILL_NINFO_SPLITTER
#define COFFEE_MILL_NINFO_SPLITTER
#include "NinfoData.hpp"

namespace coffeemill
{

class NinfoSplitter
{
  public:

    using size_type      = std::size_t;
    using index_type     = std::size_t;
    using unit_pair_type = std::pair<index_type, index_type>;

  public:

    NinfoSplitter(){}
    NinfoSplitter(const std::string& filename) : filename_(filename){}
    ~NinfoSplitter() = default;

    std::map<unit_pair_type, NinfoData> split() const;
    void write(const std::map<unit_pair_type, NinfoData>& data) const;

    void write_cafe_inp(index_type simN, const std::string& inpname,
                        const std::map<unit_pair_type, NinfoData>& splitted) const;

          std::string& filename()       {return filename_;}
    const std::string& filename() const {return filename_;}

  private:

    std::string make_1by1_fname(const unit_pair_type& units) const;

  private:
    std::string filename_;
};


}
#endif //COFFEE_MILL_NINFO_SPLITTER
