#include <mill/traj.hpp>
#include <mill/util/logger.hpp>
#include "mode_traj_extract.hpp"
#include <iostream>

namespace mill
{

const char* mode_traj_extract_usage() noexcept
{
    return "usage: mill traj extract [trajfile] [start: size_t] [stop: size_t]\n"
           "       The last snapshot will be included.\n"
           "       The index starts from 0. The initial frame is 0-th frame.\n";
}


int mode_traj_extract(std::deque<std::string_view> args)
{
    using namespace std::literals::string_literals;
    if(args.size() < 1)
    {
        log::error("mill traj mode: too few arguments.");
        log::error(mode_traj_extract_usage());
        return 1;
    }

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_traj_extract_usage());
        return 0;
    }

    if(args.size() < 3)
    {
        log::error("mill traj mode: too few arguments.");
        log::error(mode_traj_extract_usage());
        return 1;
    }

    std::size_t beg = std::numeric_limits<std::size_t>::max();
    std::size_t end = std::numeric_limits<std::size_t>::max();
    try
    {
        beg = std::stoi(std::string(args.at(1)));
    }
    catch(std::exception&)
    {
        log::error("integer parsing failed: ", args.at(1));
        log::error(mode_traj_extract_usage());
        return 1;
    }
    try
    {
        end = std::stoi(std::string(args.at(2)));
    }
    catch(std::exception&)
    {
        log::error("integer parsing failed: ", args.at(2));
        log::error(mode_traj_extract_usage());
        return 1;
    }

    const std::string outname = std::string(base_name_of(fname)) + "_"s +
        std::to_string(beg) + "to"s + std::to_string(end) +
        std::string(extension_of(fname));

    if(beg > end)
    {
        log::error("mill traj extract: begin(", beg, ") > end (", end, ")");
        log::error(mode_traj_extract_usage());
        return 1;
    }

    const std::size_t num = end - beg + 1;

    auto r = reader(fname);

    Trajectory traj(r.read_header());

    if(traj.attributes().count("nset") != 0)
    {
        traj.at("nset") = num;
    }
    traj.snapshots().resize(num);

    for(std::size_t i=0; i<beg; ++i)
    {
        // discard until i==beg
        r.read_frame();
    }
    // read "num" frames
    for(std::size_t i=0; i<num; ++i)
    {
        traj[i] = *r.read_frame();
    }

    auto w = writer(outname);
    w.write(traj);
    return 0;
}

} // mill
