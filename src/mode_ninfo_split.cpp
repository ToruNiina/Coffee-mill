#include "mode_ninfo_split.hpp"
#include <mill/ninfo/NinfoReader.hpp>
#include <mill/ninfo/NinfoWriter.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/util/logger.hpp>

namespace mill
{

const char* ninfo_split_usage() noexcept
{
    return "usage: mill ninfo split <data.ninfo>\n"
           "       split data.ninfo into one-by-one ninfo files\n";
}

//! argv = {"split", args...}
int mode_ninfo_split(std::deque<std::string_view> args)
{
    if(args.size() == 1)
    {
        log::error("mill ninfo split: too few arguments");
        log::error(ninfo_split_usage());
        return 1;
    }

    const auto fname = args.at(1);
    if(fname == "help")
    {
        log::error(ninfo_split_usage());
        return 0;
    }

    if(extension_of(fname) != ".ninfo")
    {
        log::error("mill ninfo split: invalid filename: ", fname);
        log::error(ninfo_split_usage());
        return 1;
    }

    //! argv = {"split", "traj.ninfo"}
    NinfoReader reader;
    const auto all_in_one = reader.read(std::string(fname));

    std::map<std::pair<std::size_t, std::size_t>, NinfoData> splitted;
    for(const auto& block : all_in_one)
    {
        const NinfoKind kind = block.first;
        for(const auto& line : block.second)
        {
            const auto units = std::make_pair(line->unit_at(0), line->unit_at(1));

            // search this pair of units is already found or not.
            if(splitted.count(units) == 0)
            {
                log::debug("mill ninfo split: found ninfo block "
                        "between ", units.first, " and ", units.second);
                splitted.emplace(units, NinfoData());
            }

            // search this kind of native information is already found or not.
            if(splitted.at(units).count(kind) == 0)
            {
                log::debug("mill ninfo split: found ninfo ", kind,
                        " between ", units.first, " and ", units.second);

                splitted.at(units).emplace(kind,
                        std::vector<std::shared_ptr<NinfoBase>>{});
            }

            // insert data to return value.
            splitted[units][kind].push_back(line);
        }
    }

    log::debug("mill ninfo split: blocks are splitted into ",
            splitted.size(), " ninfoes. writing one-by-one...");

    NinfoWriter writer;
    const std::string basename(base_name_of(fname));
    for(const auto& one_by_one : splitted)
    {
        using namespace std::literals::string_literals;
        const auto units = one_by_one.first;
        const std::string outname = basename + "_unit"s +
                std::to_string(units.first)  + "-"s     +
                std::to_string(units.second) + ".ninfo"s;

        log::debug("mill ninfo split: writing ninfo between ",
                   units.first, " and ", units.second, " into ", outname);
        writer.write(one_by_one.second, outname);
    }
    return 0;
}

} // mill
