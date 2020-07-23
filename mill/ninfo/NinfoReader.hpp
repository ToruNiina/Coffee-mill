/*!
  @file NinfoReader.hpp
  @brief definition of NinfoReader class.

  NinfoReader reads a ninfo file and store the data as NinfoData.

  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 10:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_NINFO_READER_HPP
#define COFFEE_MILL_NINFO_READER_HPP
#include "NinfoData.hpp"
#include <mill/util/string.hpp>
#include <mill/util/logger.hpp>
#include <fstream>

namespace mill
{

class NinfoReader
{
  public:
    using real_type = double;
    using data_type = NinfoData;

  public:

    NinfoReader() = default;
    ~NinfoReader() = default;

    data_type read(const std::string& filename) const;
    data_type read(std::istream& is) const;

  private:
    void read_line(data_type& data, const std::string& line) const;
};

inline NinfoData NinfoReader::read(const std::string& filename) const
{
    std::ifstream filestream(filename);
    if(not filestream.good())
    {
        log::fatal("NinfoReader: file open error: ", filename);
    }
    const auto retval = this->read(filestream);
    filestream.close();
    return retval;
}

inline NinfoData NinfoReader::read(std::istream& is) const
{
    NinfoData data;
    std::size_t lineindex = 0;
    while(not is.eof())
    {
        std::string line;
        std::getline(is, line); ++lineindex;
        if(line.empty()) continue;
        line = remove_indent(line);

        try
        {
            if     (line.empty())                {continue;}
            else if(line.front() == '*')         {continue;}
            else if(line.substr(0, 4) == "<<<<") {continue;}
            else if(line.substr(0, 4) == ">>>>") {continue;}
            else {this->read_line(data, line);}
        }
        catch(std::exception& except)
        {
            log::fatal("invalid ninfo line found at line #", lineindex);
        }
    }

    return data;
}

inline void NinfoReader::read_line(data_type& data, const std::string& line) const
{
    std::istringstream iss(line);
    std::string prefix;

    const auto line_head = iss.tellg();
    iss >> prefix;
    iss.seekg(line_head);

    NinfoKind kind;
    std::shared_ptr<NinfoBase> ninfo;
// switch with prefix: set kind and ninfo {{{
    if(prefix == "bond")
    {
        kind = NinfoKind::Bond;
        auto tmp = std::make_shared<NinfoBond>();
        iss >> (*tmp);
        ninfo = tmp;
    }
    else if(prefix == "angl")
    {
        kind = NinfoKind::Angl;
        auto tmp = std::make_shared<NinfoAngl>();
        iss >> (*tmp);
        ninfo = tmp;
    }
    else if(prefix == "aicg13")
    {
        kind = NinfoKind::Aicg13;
        auto tmp = std::make_shared<NinfoAicg13>();
        iss >> (*tmp);
        ninfo = tmp;
    }
    else if(prefix == "dihd")
    {
        kind = NinfoKind::Dihd;
        auto tmp = std::make_shared<NinfoDihd>();
        iss >> (*tmp);
        ninfo = tmp;
    }
    else if(prefix == "aicg14")
    {
        kind = NinfoKind::Aicg14;
        auto tmp = std::make_shared<NinfoAicg14>();
        iss >> (*tmp);
        ninfo = tmp;
    }
    else if(prefix == "aicgdih")
    {
        kind = NinfoKind::Aicgdih;
        auto tmp = std::make_shared<NinfoAicgdih>();
        iss >> (*tmp);
        ninfo = tmp;
    }
    else if(prefix == "contact")
    {
        kind = NinfoKind::Contact;
        auto tmp = std::make_shared<NinfoContact>();
        iss >> (*tmp);
        ninfo = tmp;
    }
    else if(prefix == "basepair")
    {
        kind = NinfoKind::BasePair;
        auto tmp = std::make_shared<NinfoBasePair>();
        iss >> (*tmp);
        ninfo = tmp;
    }
    else if(prefix == "basestack")
    {
        kind = NinfoKind::BaseStack;
        auto tmp = std::make_shared<NinfoBaseStack>();
        iss >> (*tmp);
        ninfo = tmp;
    }
    else
    {
        log::fatal("NinfoReader: invalid line starting with ", prefix);
    }
//}}}

    if(data.count(kind) == 0) // if data do not have this kind of ninfo yet
    {
        std::vector<std::shared_ptr<NinfoBase>> block{ninfo};
        data.emplace(kind, block);
    }
    else
    {
        data[kind].push_back(ninfo);
    }
    return;
}

}//mill
#endif //COFFEE_MILL_NINFO_READER
