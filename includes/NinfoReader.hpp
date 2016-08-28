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

template<typename T = DefaultTraits>
class NinfoReader
{
  public:
    using char_type = typename T::char_type;
    using data_type = NinfoData<T>;

  public:

    NinfoReader() = default;
    ~NinfoReader() = default;

    data_type read(const std::basic_string<char_type>& filename);
    data_type read(std::basic_istream<char_type>& is);

  private:
    void read_line(data_type& data, const std::basic_string<char_type>& line);
};

template<typename T_traits>
NinfoData<T_Traits>
NinfoReader<T_traits>::read(std::basic_string<char_type>& filename)
{
    std::basic_ifstream<char_type> filestream(filename);
    if(!filestream.good()) throw std::runtime_error("file open error");
    const auto retval = this->read(filestream);
    filestream.close();
    return retval;
}

template<typename T_traits>
NinfoData<T_traits>
NinfoReader<T_traits>::read(std::basic_istream<char_type>& is)
{
    NinfoData<T_traits> data;
    std::size_t lineindex = 0;
    while(!is.eof())
    {
        std::basic_string<char_type> line;
        std::getline(is, line); ++lineindex;
        if(line.empty()) continue;
        line = remove_indent(line);

        try{
             if(line.empty()) continue;
        else if(line.front() == '*') continue;
        else if(line.substr(0, 4) == "<<<<") continue;
        else if(line.substr(0, 4) == ">>>>") continue;
        else read_line(data, line);
        }
        catch(std::exception& except)
        {
            throw std::runtime_error( "invalid ninfo line found at line #" +
                    std::to_string(lineindex));
        }
    }

    return data;
}

template<typename T_traits>
void NinfoReader<T_traits>::read_line(
        data_type& data, const std::basic_string<char_type>& line)
{
    std::basic_istringstream<char_type> iss(line);
    std::basic_string<char_type> prefix;

    const auto line_head = iss.tellg();
    iss >> prefix;
    iss.seekg(line_head);

    NinfoKind kind;
    std::shared_ptr<NinfoBase<T_traits>> ninfo;
// switch with prefix: set kind and ninfo {{{
    if(prefix == "bond")
    {
        kind = NinfoKind::Bond;
        ninfo = std::make_shared<NinfoBond>();
    }
    else if(prefix == "angl")
    {
        kind = NinfoKind::Angl;
        ninfo = std::make_shared<NinfoAngl>();
    }
    else if(prefix == "aicg13")
    {
        kind = NinfoKind::Aicg13;
        ninfo = std::make_shared<NinfoAicg13>();
    }
    else if(prefix == "dihd")
    {
        kind = NinfoKind::Dihd;
        ninfo = std::make_shared<NinfoDihd>();
    }
    else if(prefix == "aicg14")
    {
        kind = NinfoKind::Aicg14;
        ninfo = std::make_shared<NinfoAicg14>();
    }
    else if(prefix == "aicgdih")
    {
        kind = NinfoKind::Aicgdih;
        ninfo = std::make_shared<NinfoAicgdih>();
    }
    else if(prefix == "contact")
    {
        kind = NinfoKind::Contact;
        ninfo = std::make_shared<NinfoContact>();
    }
    else if(prefix == "basepair")
    {
        kind = NinfoKind::BasePair;
        ninfo = std::make_shared<NinfoBasePair>();
    }
    else if(prefix == "basestack")
    {
        kind = NinfoKind::BaseStack;
        ninfo = std::make_shared<NinfoBaseStack>();
    }
    else
    {
        throw std::logic_error("invalid line");
    }
//}}}

    iss >> (*ninfo);
    if(data.count(kind) == 0)
    {
        std::vector<std::shared_ptr<NinfoBase<T_Traits>>> block{ninfo};
        data.emplace(kind, block);
    }
    else
    {
        data[kind].push_back(ninfo);
    }

    return;
}

}
#endif //COFFEE_MILL_NINFO_READER
