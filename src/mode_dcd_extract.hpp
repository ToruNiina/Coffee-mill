#ifndef COFFEE_MILL_DCD_EXTRACT
#define COFFEE_MILL_DCD_EXTRACT
#include <mill/dcd/DCDWriter.hpp>
#include <mill/dcd/DCDReader.hpp>
#include <limits>

namespace mill
{

inline const char* dcd_extract_usage() noexcept
{
    return "usage: mill dcd extract [parameters...]\n"
           "    $ mill dcd extract traj.dcd 100 200\n"
           "      write 100-th snapshot to 200-th snapshot into a file traj_100to200.dcd.\n"
           "      the last (here, 200-th) snapshot will be included. (not omitted)\n"
           "      the index starts from 0.\n";
}

// argv := arrayof{ "extract", "filename", {rests...} }
template<typename vectorT>
int mode_dcd_extract(int argument_c, const char **argument_v)
{
    if(argument_c == 1)
    {
        log::error("mill dcd-mode: too few arguments");
        log::error(dcd_extract_usage());
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        log::info(dcd_extract_usage());
        return 0;
    }

    if(fname.substr(fname.size()-4, 4) == ".dcd")
    {
        if(argument_c < 4)
        {
            log::error("mill dcd extract: too few arguments");
            log::error(dcd_extract_usage());
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
        catch(std::exception&)
        {
            log::error("error: mill dcd extract: integer parsing failed ",
                       argument_v[2], ", ", argument_v[3]);
            log::error(dcd_extract_usage());
            return 1;
        }
        const std::string outname =
            fname.substr(0, fname.size() - 4) + std::string("_") +
            std::to_string(beg) + std::string("to") + std::to_string(end) +
            std::string(".dcd");

        if(beg > end)
        {
            log::error("mill dcd extract: begin(", beg, ") > end (", end, ")");
            log::error(dcd_extract_usage());
            return 1;
        }
        const std::size_t num = end - beg + 1;

        DCDReader reader(dcdname);
        Trajectory traj(reader.read_header());
        traj.at("nset") = num;
        traj.snapshots().resize(num);

        for(std::size_t i=0; i<beg; ++i)
        {
            // discard first part
            reader.read_frame();
        }
        for(std::size_t i=0; i<num; ++i)
        {
            traj[i] = *reader.read_frame();
        }

        DCDWriter writer(outname);
        writer.write(traj);
        return 0;
    }
    else
    {
        log::error("mill dcd extract: unknown file extension: ", fname);
        log::error(dcd_extract_usage());
        return 1;
    }
}

} // mill
#endif// COFFEE_MILL_DCD_EXTRACT
