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
    args.pop_front();

    using attribute_container_type = Trajectory::attribute_container_type;
    std::optional<attribute_container_type> ref_header = std::nullopt;
    std::optional<Snapshot>                 ref_frame  = std::nullopt;
    if(!args.empty())
    {
        auto     r = reader(args.front());
        ref_header = r.read_header();
        ref_frame  = r.read_frame(); // read 1st frame
    }

    const std::string output = std::string(input) + "_converted." + std::string(format);
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
            // copy reference info because std::map::merge moves out the values
            Snapshot ref(*ref_frame);

            frame.merge_attributes(ref);
        }
        w.write_frame(frame);
    }
    return 0;
}

} // mill
