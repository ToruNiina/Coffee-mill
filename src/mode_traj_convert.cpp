#include <mill/util/logger.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/traj.hpp>
#include "mode_traj_convert.hpp"
#include <iostream>

namespace mill
{

const char* mode_traj_convert_usage() noexcept
{
    return "usage: mill traj convert [trajfile] [format] <reference>_opt\n"
           "       convert format from [trajfile] to [format].\n"
           "       Only the first frame in a reference file is used to merge information.\n";
}

int mode_traj_convert(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill traj convert: too few arguments");
        log::error(mode_traj_convert_usage());
        return 1;
    }

    const auto input = args.front();
    args.pop_front();

    if(input == "help")
    {
        log::info(mode_traj_convert_usage());
        return 0;
    }

    const auto format = args.front();
    log::debug("format is ", format);
    args.pop_front();

    using attribute_container_type = Trajectory::attribute_container_type;
    std::optional<attribute_container_type> ref_header = std::nullopt;
    std::optional<Snapshot>                 ref_frame  = std::nullopt;
    if(!args.empty())
    {
        log::debug("reference file is ", args.front());

        auto     r = reader(args.front());
        ref_header = r.read_header();
        ref_frame  = r.read_frame(); // read 1st frame

        log::debug("reference file has ", ref_frame->size(), " particles.");
    }

    const std::string output = std::string(input) + "_converted." + std::string(format);
    log::debug("output file is ", output);

    auto w = writer(output);
    auto r = reader(input);

    auto header = r.read_header();
    if(ref_header)
    {
        header.merge(*ref_header);
    }

    w.write_header(header);

    for(auto frame : r)
    {
        if(ref_frame)
        {
            if(ref_frame->size() != frame.size())
            {
                log::warn("number of particles in the reference file \"",
                          args.front(), "\" (", ref_frame->size(),
                          ") differs from the original \"", input, "\" (",
                          frame.size(), "). Skipping.");
            }
            else
            {
                for(std::size_t i=0; i<frame.size(); ++i)
                {
                    frame.at(i).attributes() = ref_frame->at(i).attributes();
                }
            }
        }
        w.write_frame(frame);
    }
    return 0;
}

} // mill
