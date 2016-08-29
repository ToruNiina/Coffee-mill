#include "NinfoSplitter.hpp"
#include "NinfoReader.hpp"
#include "NinfoWriter.hpp"

namespace coffeemill
{

std::map<NinfoSplitter::unit_pair_type, NinfoData> NinfoSplitter::split() const
{
    std::map<unit_pair_type, NinfoData> splitted;

    NinfoReader reader(this->filename_);
    reader.read();
    auto all_in_one = reader.data();

    for(auto block = all_in_one.cbegin(); block != all_in_one.cend(); ++block)
    {
        NinfoKind kind = block->first;
        for(auto line = block->second.cbegin();
                line != block->second.cend(); ++line)
        {
            unit_pair_type units =
                std::make_pair((*line)->unit_at(0), (*line)->unit_at(1));
            if(splitted.find(units) == splitted.cend())
            {
                splitted.emplace(units, NinfoData());
            }
            if(splitted.at(units).find(kind) == splitted.at(units).cend())
            {
                splitted.at(units).emplace(kind, NinfoBlock());
            }
            splitted[units][kind].push_back(*line);
        }
    }

    return splitted;
}

void NinfoSplitter::write(const std::map<unit_pair_type, NinfoData>& data) const
{
    for(auto iter = data.cbegin(); iter != data.cend(); ++iter)
    {
        (NinfoWriter(this->make_1by1_fname(iter->first))).write(iter->second);
    }
    return;
}

std::string NinfoSplitter::make_1by1_fname(const unit_pair_type& units) const
{
    std::cout << "make_1by1_fname" << std::endl;
    std::cout << this->filename_ << std::endl;
    std::cout << this->filename_.size() << std::endl;
    std::string retval = this->filename_.substr(0, this->filename_.size() - 6) + 
           "_" + std::to_string(units.first) +
           "_" + std::to_string(units.second) + ".ninfo";
    std::cout << retval << std::endl;
    return retval;
}

void NinfoSplitter::write_cafe_inp(index_type simN, const std::string& inpname,
                    const std::map<unit_pair_type, NinfoData>& splitted) const
{
    std::ofstream inp(inpname);
    if(!inp.good())
        throw std::runtime_error("file open error: " + inpname);

    inp << "<<<< native_info_sim" << simN << std::endl;

    unsigned index = 1;
    for(auto iter = splitted.cbegin(); iter != splitted.cend(); ++iter)
    {
        inp << "NINFO(" << iter->first.first << "/" << iter->first.second << ") "
            << index << std::endl;
        ++index;
    }

    index = 1;
    for(auto iter = splitted.cbegin(); iter != splitted.cend(); ++iter)
    {
        inp << index << " = " << this->make_1by1_fname(iter->first) << std::endl;
        ++index;
    }

    inp << ">>>>" << std::endl;
    return;
}

}
