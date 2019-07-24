/*!
  @file NinfoSplitter.hpp
  @brief definition of NinfoSplitter class.

  NinfoSplitter splits an all-in-one style ninfo file into
  several one-by-one style ninfo files.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 10:30
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_NINFO_SPLITTER
#define COFFEE_MILL_NINFO_SPLITTER
#include "ninfo/NinfoData.hpp"
#include "ninfo/NinfoReader.hpp"
#include "ninfo/NinfoWriter.hpp"
#include <cstdint>

namespace mill
{

/*!
 * @brief split all-in-one ninfo to one-by-one ninfo.
 */
template<typename realT>
class NinfoSplitter
{
  public:

    using size_type      = std::size_t;
    using index_type     = size_type;
    using real_type      = realT;
    using data_type      = NinfoData<real_type>;
    using unit_pair_type = std::pair<index_type, index_type>;

  public:

    NinfoSplitter() = default;
    ~NinfoSplitter() = default;

    /*!
     * @brief read all-in-one ninfo and split into one-by-one
     * @return map of pair of iunit and NinfoData.
     */
    std::map<unit_pair_type, data_type> split(const std::string& filename) const;

    /*!
     * @brief write one-by-one ninfo.
     * @param data is map of pair of iunit and NinfoData. same as return value of split.
     * @param outputname must be without extention.
     * @sa NinfoSplitter::split
     */
    void write(const std::map<unit_pair_type, data_type>& data,
               const std::string& outputname) const;

    /*!
     * @brief write cafemol input native_info_simN block
     * @param simN N of native_info_simN.
     * @param inpname a name of file to write input block
     * @param splitted splitted ninfo data.
     */
    void write_cafe_inp(const index_type simN, const std::string& inpname,
                        const std::string& outputname,
                        const std::map<unit_pair_type, data_type>& splitted) const;
  private:

    std::string make_1by1_fname(const std::string& fname,
                                const unit_pair_type& units) const;
};

template<typename realT>
std::map<typename NinfoSplitter<realT>::unit_pair_type, NinfoData<realT>>
NinfoSplitter<realT>::split(const std::string& filename) const
{
    NinfoReader<realT> reader;
    const auto all_in_one = reader.read(filename);

    std::map<unit_pair_type, data_type> splitted;
    for(auto block = all_in_one.cbegin(); block != all_in_one.cend(); ++block)
    {
        NinfoKind kind = block->first;
        for(auto line = block->second.cbegin();
                line != block->second.cend(); ++line)
        {
            unit_pair_type units =
                std::make_pair((*line)->unit_at(0), (*line)->unit_at(1));

            // search this pair of units is already found or not.
            if(splitted.find(units) == splitted.cend())
            {
                splitted.emplace(units, data_type());
            }

            // search this kind of native information is already found or not.
            if(splitted.at(units).find(kind) == splitted.at(units).cend())
            {
                splitted.at(units).emplace(kind,
                        std::vector<std::shared_ptr<NinfoBase<realT>>>{});
            }

            // insert data to return value.
            splitted[units][kind].push_back(*line);
        }
    }

    return splitted;
}

template<typename realT>
void NinfoSplitter<realT>::write(
        const std::map<unit_pair_type, data_type>& data,
        const std::string& outputname) const
{
    for(auto iter = data.cbegin(); iter != data.cend(); ++iter)
    {
        const NinfoWriter<realT> writer;
        writer.write(iter->second, this->make_1by1_fname(outputname, iter->first));
    }
    return;
}

template<typename realT>
inline std::string
NinfoSplitter<realT>::make_1by1_fname(
        const std::string& outputname, const unit_pair_type& units) const
{
//     std::cout << "make_1by1_fname" << std::endl;
//     std::cout << this->filename_ << std::endl;
//     std::cout << this->filename_.size() << std::endl;
    const std::string retval = outputname + 
           "_" + std::to_string(units.first) +
           "_" + std::to_string(units.second) + ".ninfo";
//     std::cout << retval << std::endl;
    return retval;
}

template<typename realT>
void NinfoSplitter<realT>::write_cafe_inp(
        const index_type simN, const std::string& inpname,
        const std::string& outputname,
        const std::map<unit_pair_type, data_type>& splitted) const
{
    std::ofstream inp(inpname);
    if(not inp.good())
        throw std::runtime_error("file open error: " + inpname);

    inp << "<<<< native_info_sim" << simN << std::endl;

    std::uint32_t index = 1;
    for(auto iter = splitted.cbegin(); iter != splitted.cend(); ++iter)
    {
        inp << "NINFO(" << iter->first.first << "/" << iter->first.second << ") "
            << index << std::endl;
        ++index;
    }

    index = 1;
    for(auto iter = splitted.cbegin(); iter != splitted.cend(); ++iter)
    {
        inp << index << " = " << this->make_1by1_fname(outputname, iter->first)
            << std::endl;
        ++index;
    }

    inp << ">>>>" << std::endl;
    return;
}

}// mill

#endif //COFFEE_MILL_NINFO_SPLITTER
