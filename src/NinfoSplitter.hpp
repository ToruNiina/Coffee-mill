/*!
  @file NinfoSplitter.hpp
  @brief definition of NinfoSplitter class.

  NinfoSplitter splits an all-in-one style ninfo file into
  several one-by-one style ninfo files.
  
  @author Toru Niina (niina.toru.68@gmail.com)
  @date 2016-06-10 10:30
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_NINFO_SPLITTER
#define COFFEE_MILL_NINFO_SPLITTER
#include "NinfoData.hpp"

namespace coffeemill
{

/*!
 * @brief split all-in-one ninfo to one-by-one ninfo.
 */
class NinfoSplitter
{
  public:

    using size_type      = std::size_t;
    using index_type     = std::size_t;
    using unit_pair_type = std::pair<index_type, index_type>;

  public:

    //! ctor
    NinfoSplitter(){}
    /*!
     *  @brief ctor
     *  @param filename a name of the all-in-one ninfo file to split
     */
    NinfoSplitter(const std::string& filename) : filename_(filename){}
    //! dtor
    ~NinfoSplitter() = default;

    /*!
     * @brief read all-in-one ninfo and split into one-by-one
     * @return map of pair of iunit and NinfoData.
     */
    std::map<unit_pair_type, NinfoData> split() const;

    /*!
     * @brief write one-by-one ninfo.
     * @param map of pair of iunit and NinfoData. same as return value of split.
     * @sa NinfoSplitter::split
     */
    void write(const std::map<unit_pair_type, NinfoData>& data) const;

    /*!
     * @brief write cafemol input native_info_simN block
     * @param simN N of native_info_simN.
     * @param inpname a name of file to write input block
     * @param splitted splitted ninfo data.
     */
    void write_cafe_inp(index_type simN, const std::string& inpname,
                        const std::map<unit_pair_type, NinfoData>& splitted) const;

    //! filename
          std::string& filename()       {return filename_;}
    //! filename
    const std::string& filename() const {return filename_;}

  private:

    std::string make_1by1_fname(const unit_pair_type& units) const;

  private:
    std::string filename_;//!< all-in-one filename to split
};


}
#endif //COFFEE_MILL_NINFO_SPLITTER
