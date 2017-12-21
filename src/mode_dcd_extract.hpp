#ifndef COFFEE_MILL_DCD_EXTRACT
#define COFFEE_MILL_DCD_EXTRACT
#include <mill/data/DCDWriter.hpp>
#include <mill/data/DCDReader.hpp>
#include <toml/toml.hpp>
#include <limits>

namespace mill
{

// argv := arrayof{ "extract", "filename", {rests...} }
template<typename vectorT>
int mill_dcd_extract(int argument_c, char **argument_v)
{
    const std::string fname(argument_v[1]);

    std::string dcdname;
    std::string outname;
    std::size_t beg = std::numeric_limits<std::size_t>::max();
    std::size_t end = std::numeric_limits<std::size_t>::max();
    if(fname.substr(fname.size()-4, 4) == ".dcd")
    {
        if(argument_c < 4)
        {
            std::cerr << "error: mill dcd extract: too few arguments"
                      << std::endl;
            return 1;
        }
        dcdname = fname;
        try{
        beg = std::stoi(std::string(argument_v[2]));
        end = std::stoi(std::string(argument_v[3]));
        }
        catch(std::exception)
        {
            std::cerr << argument_v[2] << std::endl;
            std::cerr << argument_v[3] << std::endl;
            throw;
        }
        outname = fname.substr(0, fname.size() - 4) + std::string("_") +
            std::to_string(beg) + std::string("to") + std::to_string(end) +
            std::string(".dcd");
    }
    else if(fname.substr(fname.size()-5, 5) == ".toml")
    {
        std::ifstream ifs(fname);
        if(not ifs.good())
        {
            std::cerr << "error: mill dcd extract: file open error"
                      << std::endl;
            return 1;
        }
        toml::Data data = toml::parse(ifs);
        dcdname = toml::get<toml::String>(data.at("dcdname"));
        beg = static_cast<std::size_t>(toml::get<toml::Integer>(data.at("begin")));
        end = static_cast<std::size_t>(toml::get<toml::Integer>(data.at("end")));
        outname = toml::get<toml::String>(data.at("outname"));
    }
    else
    {
        std::cerr << "error: mill dcd extract: unknown extension" << std::endl;
        return 1;
    }

    if(beg > end)
    {
        std::cerr << "error: mill dcd extract: begin(" << beg
                  << ") > end (" << end << ")" << std::endl;
        return 1;
    }
    const std::size_t num = end - beg;

    DCDReader<vectorT> reader;
    const auto origin = reader.read(dcdname);
    DCDData<vectorT> data(num);
    data.header() = origin.header();
    data.nset() = num;

    for(std::size_t i=beg; i < end; ++i)
        data.at(i-beg) = origin.at(i);

    DCDWriter<vectorT> writer;
    writer.write(outname, data);

    return 0;
}

} // mill
#endif// COFFEE_MILL_DCD_EXTRACT
