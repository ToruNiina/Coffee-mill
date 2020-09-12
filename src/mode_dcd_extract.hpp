#ifndef COFFEE_MILL_DCD_EXTRACT
#define COFFEE_MILL_DCD_EXTRACT
#include <mill/dcd/DCDWriter.hpp>
#include <mill/dcd/DCDReader.hpp>
#include <mill/util/file_extension.hpp>
#include <limits>
#include <string_view>
#include <deque>

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
inline int mode_dcd_extract(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::error("mill dcd-mode: too few arguments");
        log::error(dcd_extract_usage());
        return 1;
    }

    const auto fname = args.at(1);
    if(fname == "help")
    {
        log::info(dcd_extract_usage());
        return 0;
    }

    if(extension_of(fname) == ".dcd")
    {
        if(args.size() < 4)
        {
            log::error("mill dcd extract: too few arguments");
            log::error(dcd_extract_usage());
            return 1;
        }
        const std::string dcdname(fname);
        std::size_t beg = std::numeric_limits<std::size_t>::max();
        std::size_t end = std::numeric_limits<std::size_t>::max();
        try
        {
            beg = std::stoi(std::string(args.at(2)));
            end = std::stoi(std::string(args.at(3)));
        }
        catch(std::exception&)
        {
            log::error("error: mill dcd extract: integer parsing failed ",
                       args.at(2), ", ", args.at(3));
            log::error(dcd_extract_usage());
            return 1;
        }
        using namespace std::literals::string_literals;

        const std::string outname = std::string(base_name_of(fname)) + "_"s +
            std::to_string(beg) + "to"s + std::to_string(end) + ".dcd"s;

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
