/*!
  @file NinfoWriter.hpp
  @brief definition of NinfoWriter class.

  NinfoWriter writes NinfoData into a file.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 10:00
  @copyright Toru Niina 2016 on MIT License
*/
#ifndef COFFEE_MILL_NINFO_WRITER
#define COFFEE_MILL_NINFO_WRITER
#include "NinfoData.hpp"

namespace coffeemill
{

template<typename T = DefaultTraits>
class NinfoWriter
{
  public:
    using traits_type = T;
    using char_type   = typename traits_type::char_type;
    using data_type   = NinfoData<traits_type>;

  public:
    NinfoWriter() = default;
    ~NinfoWriter() = default;

    void write(const data_type& data, const std::string& filename) const;
    void write(const data_type& data, std::basic_ostream<char_type>& stream) const;

  private:
    template<typename T_ninfo>
    void write_block(const typename data_type::value_type& block,
            std::basic_ostream<char_type>& stream) const;
};

template<typename T>
void NinfoWriter<T>::write(
        const data_type& data, const std::string& filename) const
{
    std::basic_ofstream<char_type> file_stream(filename_);
    if(!file_stream.good())
        throw std::runtime_error("file open error");
    this->write(data, file_stream);
    file_stream.close();
    return;
}

template<typename T>
void NinfoWriter<T>::write(
        const data_type& data, std::basic_ostream<char_type>& stream) const
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
          case NinfoKind::Aicg14p:
            {
            stream << NinfoData::aicg14p << std::endl;
            this->write_block<NinfoAicg14p>(stream, iter->second);
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
            throw std::logic_error("invalid NinfoKind");
        }

        stream << ">>>>" << std::endl;
        stream << std::endl;
    }
    return;
}

template<typename T_traits>
template<typename T_ninfo>
void NinfoWriter<T_traits>::write_block(
        const typename data_type::value_type& block,
        std::basic_ostream<char_type>& stream) const;
{
    for(auto iter = block.cbegin(); iter != block.cend(); ++iter)
        stream << *std::dynamic_pointer_cast<T_ninfo>(*iter) << std::endl;

    return;
}


}//coffeemill
#endif /* COFFEE_MILL_NINFO_WRITER */
