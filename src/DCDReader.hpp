/*!
  @file DCDReader.hpp
  @brief definition of a class that reads dcd file.

  DCDReader reads dcd file and store the information included in the file.
  the data is stored as DCDData. Reader can read only a header region.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_READER
#define COFFEE_MILL_DCD_READER
#include <memory>
#include <utility>
#include "DCDData.hpp"

namespace coffeemill
{

//! DCD reader class.
/*!
 *  DCDReader reads dcd file and store the data as DCDData.
 */
class DCDReader
{
  public:
    using data_type = DCDData;

  public:
    //! ctor.
    DCDReader(){}
    //! ctor. specify the filename to read.
    explicit DCDReader(const std::string& filename)
    : filename_(filename)
    {}
    //! dtor.
    ~DCDReader() = default;

    //! read the file specified by this->filename_.
    void read();
    //! read the file specified by argument.
    /*!
     *  read the file specified by argument. if you already read a dcdfile, 
     *  the data will be discarded.
     *  @param fname filename you want to read.
     */
    void read(const std::string& fname);

    //! read only a header.
    void read_header();
    //! read only a header of the file specified by argument.
    /*!
     *  read only a header of the file specified by argument.
     *  if you already read a dcdfile, the data, including a trajectory, will be
     *  discarded.
     *  @param fname filename you want to read header.
     */
    void read_header(const std::string& fname);

    //! access filename.
    const std::string& filename() const {return filename_;}
    //! access filename.
          std::string& filename()       {return filename_;}

    //! access DCDData.
    const data_type& data() const {return data_;}
    //! access DCDData.
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
    std::size_t header1_size; //!< size of header 1st block
    std::size_t header2_size; //!< size of header 2nd block
    std::size_t header3_size; //!< size of header 3rd block
    std::size_t snapshot_size;//!< size of snapshot
    std::size_t file_size;    //!< size of entire file

    // data
    std::string filename_; //!< filename
    data_type   data_;     //!< data in dcd file

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
