#ifndef COFFEE_MILL_DCD_EXTRACT
#define COFFEE_MILL_DCD_EXTRACT
#include <mill/data/DCDWriter.hpp>
#include <mill/data/DCDReader.hpp>
#include <toml/toml.hpp>
#include <limits>

namespace mill
{

inline const char* dcd_extract_usage() noexcept
{
    return "usage: mill dcd extract [parameters...]\n"
           "    $ mill dcd extract traj.dcd 100 200\n"
           "    : write 100-th snapshot to 200-th snapshot into a file traj_100to200.dcd.\n"
           "    : the last (here, 200-th) snapshot will be included. (not omitted)\n"
           "    : the index starts from 0.\n";
}

// argv := arrayof{ "extract", "filename", {rests...} }
template<typename vectorT>
int mode_dcd_extract(int argument_c, const char **argument_v)
{
    if(argument_c == 1)
    {
        std::cerr << "error: mill dcd-mode: too few arguments\n";
        std::cerr << dcd_extract_usage() << std::endl;
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        std::cerr << dcd_extract_usage() << std::endl;
        return 0;
    }

    if(fname.substr(fname.size()-4, 4) == ".dcd")
    {
        if(argument_c < 4)
        {
            std::cerr << "error: mill dcd extract: too few arguments"
                      << std::endl;
            std::cerr << dcd_extract_usage() << std::endl;
            return 1;
        }
        const std::string dcdname = fname;
        std::size_t beg = std::numeric_limits<std::size_t>::max();
        std::size_t end = std::numeric_limits<std::size_t>::max();
        try
        {
            beg = std::stoi(std::string(argument_v[2]));
            end = std::stoi(std::string(argument_v[3]));
        }
        catch(std::exception)
        {
            std::cerr << "error: mill dcd extract: "
                         "string -> integer parsing failed\n";
            std::cerr << argument_v[2] << std::endl;
            std::cerr << argument_v[3] << std::endl;
            std::cerr << dcd_extract_usage() << std::endl;
            return 1;
        }
        const std::string outname =
            fname.substr(0, fname.size() - 4) + std::string("_") +
            std::to_string(beg) + std::string("to") + std::to_string(end) +
            std::string(".dcd");

        if(beg > end)
        {
            std::cerr << "error: mill dcd extract: begin(" << beg
                      << ") > end (" << end << ")" << std::endl;
            std::cerr << dcd_extract_usage() << std::endl;
            return 1;
        }
        const std::size_t num = end - beg + 1;

        std::ifstream ifs(dcdname);
        if(not ifs.good())
        {
            std::cerr << "error: mill dcd extract: file open error: "
                      << dcdname << std::endl;
            std::cerr << dcd_extract_usage() << std::endl;
            return 1;
        }

        DCDReader<vectorT> reader;
        auto dcddata = reader.read_header(ifs);
        dcddata.nset() = num;
        dcddata.traj().resize(num);
        for(std::size_t i=0; i<beg; ++i)
        {
            // discard first part
            reader.read_snapshot(ifs, dcddata.header());
        }
        for(std::size_t i=0; i<num; ++i)
        {
            dcddata[i] = reader.read_snapshot(ifs, dcddata.header());
        }
        ifs.close();

        DCDWriter<vectorT> writer;
        writer.write(outname, dcddata);
        return 0;
    }
    else
    {
        std::cerr << "error: mill dcd extract: unknown file extension"
                  << fname << '\n';
        std::cerr << dcd_extract_usage() << std::endl;
        return 1;
    }
}

} // mill
#endif// COFFEE_MILL_DCD_EXTRACT