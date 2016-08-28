/*!
  @file NinfoReader.hpp
  @brief definition of NinfoReader class.

  NinfoReader reads a ninfo file and store the data as NinfoData.

  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 10:00
  @copyright Toru Niina 2016 on MIT License
*/

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

    //! ctor
    NinfoReader(){}
    //! ctor
    /*!
     * ctor.
     * @param filename file to read.
     */
    NinfoReader(const std::string& filename)
        : filename_(filename)
    {}

    //! dtor
    ~NinfoReader() = default;

    //! read file.
    /*!
     * read file. If this class already read a file and store ninfo data,
     * the data will be discarded.
     */
    void read();
    //! read file specified by argument.
    /*!
     * read file. If this class already read a file and store ninfo data,
     * the data will be discarded.
     * @param filename name of a file to read.
     */
    void read(const std::string& filename){filename_ = filename; return this->read();}

    //! ninfo filename
          std::string& filename()       {return filename_;}
    //! ninfo filename
    const std::string& filename() const {return filename_;}
    //! ninfo data
          data_type& data()       {return data_;}
    //! ninfo data
    const data_type& data() const {return data_;}

  private:
    void       read_block(std::ifstream& ifs, const std::string& line);
    template<typename T_ninfo>
    NinfoBlock read_block(std::ifstream& ifs) const;
    NinfoKind  judge_block(const std::string& line) const;
  
  private:
    std::string filename_; //!< filename to read
    data_type   data_;     //!< ninfo data

};

}
#endif //COFFEE_MILL_NINFO_READER
