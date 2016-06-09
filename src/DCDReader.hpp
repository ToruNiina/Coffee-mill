#ifndef COFFEE_MILL_DCD_READER
#define COFFEE_MILL_DCD_READER
#include <memory>
#include <utility>
#include "DCDData.hpp"

namespace coffeemill
{
class DCDReader
{
  public:
    using data_type = DCDData;

  public:
    DCDReader(){}
    DCDReader(const std::string& filename)
    : filename_(filename)
    {}
    ~DCDReader() = default;

    void read();
    void read(const std::string& fname);

    void read_header();
    void read_header(const std::string& fname);

    const std::string& filename() const {return filename_;}
          std::string& filename()       {return filename_;}
    const data_type& data() const {return data_;}
          data_type& data()       {return data_;}

  private:

    bool validate_filesize();
    void read_header(std::ifstream& dcdfile);
    void read_core(std::ifstream& dcdfile);
    void read_head_block1(std::ifstream& dcdfile);
    void read_head_block2(std::ifstream& dcdfile);
    void read_head_block3(std::ifstream& dcdfile);
    std::vector<double> read_coord(std::ifstream& dcdfile);

  private:

    // filesize validation
    std::size_t header1_size;
    std::size_t header2_size;
    std::size_t header3_size;
    std::size_t snapshot_size;
    std::size_t file_size;

    // data
    std::string filename_;
    data_type   data_;

  private:

    //XXX: size_value depends on the environment that the simulation
    //     runs, not on the environment where you analysis the dcd file.
    //     so these size value perhaps differ from the true value.
    static constexpr std::size_t size_int   = sizeof(int);
    static constexpr std::size_t size_float = sizeof(float);
    static constexpr std::size_t size_char  = sizeof(char);
};

}//end namespace coffeemill

#endif //COFFEE_MILL_DCD_READER
