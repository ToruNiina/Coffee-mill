/*!
  @file NinfoWriter.hpp
  @brief definition of NinfoWriter class.

  NinfoWriter writes NinfoData into a file.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 10:00
  @copyright Toru Niina 2016 on MIT License
*/
#ifndef COFFEE_MILL_NINFO_WRITER_HPP
#define COFFEE_MILL_NINFO_WRITER_HPP
#include "NinfoData.hpp"
#include <fstream>

namespace mill
{

class NinfoWriter
{
  public:
    using real_type = double;
    using data_type = NinfoData;
    using block_type = typename data_type::value_type;

  public:
    NinfoWriter(){}
    ~NinfoWriter() = default;

    void write(const data_type& data, const std::string& filename) const;
    void write(const data_type& data, std::ostream& stream) const;

  private:
    template<typename ninfoT>
    void write_block(std::ostream& stream, const block_type& block) const
    {
        for(const auto& ptr : block)
        {
            stream << *(std::dynamic_pointer_cast<ninfoT>(ptr)) << std::endl;
        }
    }
};

inline void NinfoWriter::write(
        const data_type& data, const std::string& filename) const
{
    std::ofstream file_stream(filename);
    if(not file_stream.good())
    {
        log::fatal("NinfoWriter: file open error: ", filename);
    }
    this->write(data, file_stream);
    file_stream.close();
    return;
}

inline void NinfoWriter::write(
        const data_type& data, std::ostream& stream) const
{
    for(auto iter = data.cbegin(); iter != data.cend(); ++iter)
    {
        stream << "<<<< ";
        switch(iter->first)
        {
            case NinfoKind::Bond:
            {
                stream << NinfoData::bond << std::endl;
                this->write_block<NinfoBond>(stream, iter->second);
                break;
            }
            case NinfoKind::Angl:
            {
                stream << NinfoData::angl << std::endl;
                this->write_block<NinfoAngl>(stream, iter->second);
                break;
            }
            case NinfoKind::Dihd:
            {
                stream << NinfoData::dihd << std::endl;
                this->write_block<NinfoDihd>(stream, iter->second);
                break;
            }
            case NinfoKind::Aicg13:
            {
                stream << NinfoData::aicg13 << std::endl;
                this->write_block<NinfoAicg13>(stream, iter->second);
                break;
            }
            case NinfoKind::Aicg14:
            {
                stream << NinfoData::aicg14 << std::endl;
                this->write_block<NinfoAicg14>(stream, iter->second);
                break;
            }
            case NinfoKind::Aicgdih:
            {
                stream << NinfoData::aicgdih << std::endl;
                this->write_block<NinfoAicgdih>(stream, iter->second);
                break;
            }
            case NinfoKind::Contact:
            {
                stream << NinfoData::contact << std::endl;
                this->write_block<NinfoContact>(stream, iter->second);
                break;
            }
            case NinfoKind::BasePair:
            {
                stream << NinfoData::basepair << std::endl;
                this->write_block<NinfoBasePair>(stream, iter->second);
                break;
            }
            case NinfoKind::BaseStack:
            {
                stream << NinfoData::basestack << std::endl;
                this->write_block<NinfoBaseStack>(stream, iter->second);
                break;
            }
            default:
            {
                throw std::logic_error("invalid NinfoKind : " +
                        std::to_string(static_cast<int>(iter->first)));
            }
        }

        stream << ">>>>" << std::endl;
        stream << std::endl;
    }
    return;
}

}//mill
#endif /* COFFEE_MILL_NINFO_WRITER */
