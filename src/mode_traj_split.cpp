#include <mill/util/logger.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/traj.hpp>
#include "mode_traj_split.hpp"
#include <iostream>

namespace mill
{

const char* mode_traj_split_usage() noexcept
{
    return "usage: mill dcd split [parameters...]\n\n"
           "     - mill dcd split traj.dcd 100 --skip-initial\n"
           "       It splits traj.dcd for every 100 snapshot.\n"
           "       first contains [0, 99], 2nd [100, 199], ...\n"
           "         If the total number of frams contained is not a multiple of 100,\n"
           "       the last snapshot will contain too few frames.\n"
           "       You can also skip the initial frame by adding --skip-initial.\n"
           ;
}

int mode_traj_split(std::deque<std::string_view> args)
{
    using namespace std::literals::string_literals;
    using namespace std::literals::string_view_literals;
    if(args.empty())
    {
        log::error("mill traj split: too few arguments");
        log::error(mode_traj_split_usage());
        return 1;
    }

    const bool skip_initial = [&args]() {
        const auto found = std::find(args.begin(), args.end(), "--skip-initial"sv);
        if(found != args.end())
        {
            args.erase(found);
            return true;
        }
        return false;
    }();

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_traj_split_usage());
        return 0;
    }
    args.pop_front();

    std::size_t unit = 0;
    try
    {
        unit = std::stoi(std::string(args.front()));
    }
    catch(std::exception& excpt)
    {
        log::error("mill dcd split: invalid argument: ", args.front(),
                   " is not an integer.");
        log::error(mode_traj_split_usage());
        return 1;
    }

    auto r = reader(fname);
    Trajectory traj(r.read_header());
    traj.at("nset") = unit;
    traj.snapshots().resize(unit);

    if(skip_initial)
    {
        // read out the first frame.
        std::ignore = r.read_frame();
    }

    bool next_exists = true;
    std::size_t idx = 0;
    while(next_exists)
    {
        const std::string outname = std::string(base_name_of(fname)) + "_"s +
            std::to_string(idx) + std::string(extension_of(fname));

        auto w = writer(outname);
        w.write_header(traj.attributes());
        for(std::size_t i=0; i < unit; ++i)
        {
            if(auto frame = r.read_frame())
            {
                w.write_frame(*frame);
            }
            else
            {
                next_exists = false;
                break;
            }
        }
        idx += 1;
        next_exists = !r.is_eof();
    }
    return 0;
}

}//mill
