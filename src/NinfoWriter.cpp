#include "NinfoWriter.hpp"

namespace coffeemill
{

void NinfoWriter::write(const NinfoData& data) const
{
    std::ofstream ninfo(this->filename_);
    if(!ninfo.good())
        throw std::runtime_error("file open error");

    for(auto iter = data.cbegin(); iter != data.cend(); ++iter)
    {
        ninfo << "<<<< ";
        switch(iter->first)
        {
          case NinfoKind::Bond:
            {
            ninfo << NinfoData::bond << std::endl;
            this->write_block<NinfoBond>(ninfo, iter->second);
            break;
            }
          case NinfoKind::Angl:
            {
            ninfo << NinfoData::angl << std::endl;
            this->write_block<NinfoAngl>(ninfo, iter->second);
            break;
            }
          case NinfoKind::Dihd:
            {
            ninfo << NinfoData::dihd << std::endl;
            this->write_block<NinfoDihd>(ninfo, iter->second);
            break;
            }
          case NinfoKind::Aicg13:
            {
            ninfo << NinfoData::aicg13 << std::endl;
            this->write_block<NinfoAicg13>(ninfo, iter->second);
            break;
            }
          case NinfoKind::Aicg14:
            {
            ninfo << NinfoData::aicg14 << std::endl;
            this->write_block<NinfoAicg14>(ninfo, iter->second);
            break;
            }
          case NinfoKind::Aicg14p:
            {
            ninfo << NinfoData::aicg14p << std::endl;
            this->write_block<NinfoAicg14p>(ninfo, iter->second);
            break;
            }
          case NinfoKind::Contact:
            {
            ninfo << NinfoData::contact << std::endl;
            this->write_block<NinfoContact>(ninfo, iter->second);
            break;
            }
          case NinfoKind::BasePair:
            {
            ninfo << NinfoData::basepair << std::endl;
            this->write_block<NinfoBasePair>(ninfo, iter->second);
            break;
            }
          case NinfoKind::BaseStack:
            {
            ninfo << NinfoData::basestack << std::endl;
            this->write_block<NinfoBaseStack>(ninfo, iter->second);
            break;
            }
          default:
            throw std::logic_error("invalid NinfoKind");
        }

        ninfo << ">>>>" << std::endl;
        ninfo << std::endl;
    }
    ninfo.close();
    return;
}

template<typename T_ninfo>
void NinfoWriter::write_block(std::ofstream& ofs, const NinfoBlock& block) const
{
    for(auto iter = block.cbegin(); iter != block.cend(); ++iter)
    {
        ofs << *std::dynamic_pointer_cast<T_ninfo>(*iter) << std::endl;
    }

    return;
}

}
