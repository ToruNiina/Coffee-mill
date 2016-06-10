/*!
  @file NinfoWriter.hpp
  @brief definition of NinfoWriter class.

  NinfoWriter writes NinfoData into a file.
  
  @author Toru Niina (niina.toru.68@gmail.com)
  @date 2016-06-10 10:00
  @copyright Toru Niina 2016 on MIT License
*/

#include "NinfoData.hpp"
#include <fstream>

namespace coffeemill
{

/*!
 * @brief write ninfo data into file.
 */
class NinfoWriter
{
  public:
    //! ctor
    NinfoWriter(){}
    /*!
     * @brief ctor.
     * @param filename file to read.
     */
    NinfoWriter(const std::string& filename): filename_(filename){}
    //! dtor
    ~NinfoWriter() = default;

    /*!
     * @brief write data into a file
     * @param data NinfoData to write.
     */
    void write(const NinfoData& data) const;

    //! filename
          std::string& filename()       {return filename_;}
    //! filename
    const std::string& filename() const {return filename_;}

  private:
    template<typename T_ninfo>
    void write_block(std::ofstream& ofs, const NinfoBlock& block) const;

  private:
    std::string filename_; //!< filename to write
};

}
