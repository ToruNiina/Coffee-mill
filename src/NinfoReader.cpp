#include "NinfoReader.hpp"
#include "utility.hpp"
#include <fstream>

namespace coffeemill
{

void NinfoReader::read()
{
    std::ifstream ninfo(this->filename_);
    if(!ninfo.good())
        throw std::runtime_error("file open error");

    while(!ninfo.eof())
    {
        std::string line;
        std::getline(ninfo, line);
        if(line.empty())        continue;

        line = remove_front<' '>(line);
        if(line.front() == '*') continue;
        if(line.substr(0, 4) == "<<<<") // block start
        {
            this->read_block(ninfo, line);
            continue;
        }

        throw std::invalid_argument("unknown line: " + line);
    }

    return;
}

void NinfoReader::read_block(std::ifstream& ifs, const std::string& line)
{
    NinfoKind kind = this->judge_block(line);

    switch(kind)
    {
      case NinfoKind::Bond: 
        {
        data_.emplace(kind, read_block<NinfoBond>(ifs));
        return;
        }
      case NinfoKind::Angl:
        {
        data_.emplace(kind, read_block<NinfoAngl>(ifs));
        return;
        }
      case NinfoKind::Dihd:
        {
        data_.emplace(kind, read_block<NinfoDihd>(ifs));
        return;
        }
      case NinfoKind::Aicg13:
        {
        data_.emplace(kind, read_block<NinfoAicg13>(ifs));
        return;
        }
      case NinfoKind::Aicg14:
        {
        data_.emplace(kind, read_block<NinfoAicg14>(ifs));
        return;
        }
      case NinfoKind::Aicg14p:
        {
        data_.emplace(kind, read_block<NinfoAicg14p>(ifs));
        return;
        }
      case NinfoKind::Contact:
        {
        data_.emplace(kind, read_block<NinfoContact>(ifs));
        return;
        }
      case NinfoKind::BasePair:
        {
        data_.emplace(kind, read_block<NinfoBasePair>(ifs));
        return;
        }
      case NinfoKind::BaseStack:
        {
        data_.emplace(kind, read_block<NinfoBaseStack>(ifs));
        return;
        }
      default:
        throw std::logic_error("invalid ninfo kind");
    }
}

template<typename T_ninfo>
NinfoBlock NinfoReader::read_block(std::ifstream& ifs) const
{
    NinfoBlock block;

    while(!ifs.eof())
    {
        std::string line;
        std::getline(ifs, line);
        if(line.empty())        continue;
        line = remove_front<' '>(line);
        if(line.front() == '*') continue;
        if(line.substr(0,4) == ">>>>") break;

        T_ninfo *elem = new T_ninfo;
        line >> *(elem);
        block.push_back(NinfoSptr(elem));
    }

    return block;
}

NinfoKind NinfoReader::judge_block(const std::string& line) const
{
         if(is_contain(line, NinfoData::bond))      return NinfoKind::Bond;
    else if(is_contain(line, NinfoData::angl))      return NinfoKind::Angl;
    else if(is_contain(line, NinfoData::dihd))      return NinfoKind::Dihd;
    else if(is_contain(line, NinfoData::aicg13))    return NinfoKind::Aicg13;
    else if(is_contain(line, NinfoData::aicg14p))   return NinfoKind::Aicg14p;
    else if(is_contain(line, NinfoData::aicg14))    return NinfoKind::Aicg14;
    else if(is_contain(line, NinfoData::contact))   return NinfoKind::Contact;
    else if(is_contain(line, NinfoData::basepair))  return NinfoKind::BasePair;
    else if(is_contain(line, NinfoData::basestack)) return NinfoKind::BaseStack;
    else throw std::invalid_argument("unknown ninfo block definition: " + line);
}

}
