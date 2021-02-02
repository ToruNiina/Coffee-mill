#include <mill/util/logger.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/util/cmdarg.hpp>
#include <mill/traj.hpp>

#include "mode_traj_mean.hpp"

namespace mill
{

const char* mode_traj_mean_usage() noexcept
{
    return "usage: mill traj mean traj.dcd\n"
           "     : It calculates mean structure from trajectory.\n";
}

int mode_traj_mean(std::deque<std::string_view> args)
{
    using namespace std::literals::string_literals;
    if(args.empty())
    {
        log::error("error: mill traj mean: too few arguments");
        log::error(mode_traj_mean_usage());
        return 1;
    }

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_traj_mean_usage());
        return 0;
    }
    args.pop_front();

    auto traj = reader(fname).read();
    if(traj.size() == 0)
    {
        log::error("mill traj mean: trajectory ", fname, " does not have any frame.");
        return 0;
    }

    for(std::size_t t=1; t<traj.size(); ++t)
    {
        const auto& frame = traj.at(t);
        for(std::size_t i=0; i<frame.size(); ++i)
        {
            traj.at(0).at(i).position() += frame.at(i).position();
        }
    }

    const double normalize = 1.0 / traj.size();
    auto& front = traj.at(0);
    for(std::size_t i=0; i<front.size(); ++i)
    {
        front.at(i).position() *= normalize;
    }
    traj.snapshots().resize(1);

    const std::string output = std::string(base_name_of(fname)) + "_mean"s +
                               std::string(extension_of(fname));
    auto w = writer(output);
    w.write(traj);

    return 0;
}

} // mill
