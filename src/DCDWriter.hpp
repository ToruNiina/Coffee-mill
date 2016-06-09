/*!
  @file DCDWriter.hpp
  @brief definition of a class that writes dcd file.

  DCDWriter writes DCDData into DCD file. this can write only one snapshot
  step by step. But, when you write trajectory step by step, the consistency
  between header data and trajectory size will not be guaranteed.
  
  @author Toru Niina (niina.toru.68@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_WRITER
#define COFFEE_MILL_DCD_WRITER
#include <cstring>
#include "DCDReader.hpp"

namespace coffeemill
{

//! DCD writer class.
/*!
 *  DCDWriter writes not only entire DCDData but also many Snapshots into dcd file.
 */
class DCDWriter
{
  public:
    using data_type       = DCDData;
    using trajectory_type = typename data_type::trajectory_type;
    using snapshot_type   = typename data_type::snapshot_type;
    using position_type   = typename data_type::position_type;

  public:

    //! ctor
    DCDWriter(){}
    //! ctor. specify the filename to write
    DCDWriter(const std::string& filename)
        :filename_(filename)
    {}
    //! dtor
    ~DCDWriter() = default;

    //! write the data.
    void write();
    //! write the data into the file specified by argument.
    /*!
     *  write the data into the file specified by argument.
     *  @param filename output filename
     */
    void write(const std::string& filename);
    //! return whether the header is written. use in the case of step-by-step writing.
    bool header_is_written() const {return header_written;}

    //! write only header.
    void write_header();
    //!write only one snapshot.
    /*!
     * write only one snapshot.
     * @param snapshot snapshot (a.k.a. vector<ax::Vector3d>.)
     */
    void write_snapshot(const snapshot_type& snapshot);

    //! filename 
    const std::string& filename() const {return filename_;}
    //! filename 
          std::string& filename()       {return filename_;}
    //! data to write
    const data_type& data() const {return data_;}
    //! data to write
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

    std::string filename_; //!< filename
    data_type   data_;     //!< DCD data to write
    bool header_written = false;//! written header or not

  private:

    static constexpr int size_int   = sizeof(int);
    static constexpr int size_float = sizeof(float);
    static constexpr int size_char  = sizeof(char);
};


}//end namespace coffeemill

#endif //COFFEE_MILL_DCD_WRITER
