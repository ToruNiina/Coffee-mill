#ifndef COFFEE_MILL_DCD_WRITER
#define COFFEE_MILL_DCD_WRITER
#include <cstring>
#include "DCDReader.hpp"

namespace coffeemill
{

class DCDWriter
{
  public:
    using data_type       = DCDData;
    using trajectory_type = typename data_type::trajectory_type;
    using snapshot_type   = typename data_type::snapshot_type;
    using position_type   = typename data_type::position_type;

  public:

    DCDWriter(){}
    DCDWriter(const std::string& filename)
        :filename_(filename)
    {}
    ~DCDWriter() = default;

    void write_file();
    void write_file(const std::string& filename);
    bool header_is_written() const {return header_written;}

    // dcddata required
    void write_header();

    void write_snapshot(const snapshot_type& snapshot);

    const std::string& filename() const {return filename_;}
          std::string& filename()       {return filename_;}
    const data_type& data() const {return data_;}
          data_type& data()       {return data_;}

  private:

    void write_core       (std::ofstream& dcdfile);
    void write_header     (std::ofstream& dcdfile);
    void write_head_block1(std::ofstream& dcdfile);
    void write_head_block2(std::ofstream& dcdfile);
    void write_head_block3(std::ofstream& dcdfile);
    void write_snapshot   (std::ofstream& dcdfile, const snapshot_type& snapshot);
    void write_coord      (std::ofstream& dcdfile, const std::vector<double>& coord);

  private:

    std::string filename_;
    data_type   data_;

    bool header_written = false;// already write header

  private:

    static constexpr int size_int   = sizeof(int);
    static constexpr int size_float = sizeof(float);
    static constexpr int size_char  = sizeof(char);
};


}//end namespace coffeemill

#endif //COFFEE_MILL_DCD_WRITER
