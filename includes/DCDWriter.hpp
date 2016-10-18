/*!
  @file DCDWriter.hpp
  @brief definition of a class that writes dcd file.

  DCDWriter writes DCDData into DCD file. this can write only one snapshot
  step by step. But, when you write trajectory step by step, the consistency
  between header data and trajectory size will not be guaranteed.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_WRITER
#define COFFEE_MILL_DCD_WRITER
#include <cstring>
#include "DCDData.hpp"

namespace coffeemill
{

//! DCD writer class.
/*!
 *  DCDWriter writes not only entire DCDData but also many Snapshots into dcd file.
 */
template <typename T = DefaultTraits>
class DCDWriter
{
  public:
    using traits_type   = T;
    using data_type     = DCDData<traits_type>;
    using char_type     = traits_type::char_type;
    using size_type     = data_type::size_type;
    using string_type   = data_type::string_type;
    using header_type   = data_type::header_type;
    using snapshot_type = data_type::snapshot_type;

  public:

    DCDWriter(){}
    DCDWriter(const std::string& filename): filename_(filename){}
    ~DCDWriter() = default;

    void write(std::basic_ostream<char_type>& os, const data_type& filename);
    void write_header(std::basic_ostream<char_type>& os, const header_type& header);

    void write_snapshot(const snapshot_type& snapshot);

    bool header_written() const {return header_written_;}

    std::string const& filename() const {return filename_;}
    std::string      & filename()       {return filename_;}
    data_type const& data() const {return data_;}
    data_type      & data()       {return data_;}

  private:

    void write_core       (std::ofstream& dcdfile);
    void write_header     (std::ofstream& dcdfile);
    void write_head_block1(std::ofstream& dcdfile);
    void write_head_block2(std::ofstream& dcdfile);
    void write_head_block3(std::ofstream& dcdfile);
    void write_snapshot   (std::ofstream& dcdfile, const snapshot_type& snapshot);
    void write_coord      (std::ofstream& dcdfile, const std::vector<double>& coord);

  private:

    std::string filename_; //!< filename
    data_type   data_;     //!< DCD data to write
    bool header_written_ = false;//! written header or not

  private:

    static constexpr int size_int   = sizeof(int);
    static constexpr int size_float = sizeof(float);
    static constexpr int size_char  = sizeof(char);
};


}//end namespace coffeemill

#endif //COFFEE_MILL_DCD_WRITER
