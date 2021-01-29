#include <mill/util/logger.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/util/cmdarg.hpp>
#include <mill/traj.hpp>

#include "mode_traj_running_average.hpp"

namespace mill
{

const char* mode_traj_running_average_usage() noexcept
{
    return "usage: mill traj running-average traj.dcd --window=10\n";
}

int mode_traj_running_average(std::deque<std::string_view> args)
{
    using namespace std::literals::string_literals;

    const auto window_size = pop_argument<std::size_t>(args, "window").value_or(10);
    if(args.empty())
    {
        log::error("error: mill traj running_average: too few arguments");
        log::error(mode_traj_running_average_usage());
        return 1;
    }

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_traj_running_average_usage());
        return 0;
    }
    args.pop_front();

    const std::string output = std::string(base_name_of(fname)) + "_averaged_"s +
            std::to_string(window_size) + std::string(extension_of(fname));

    auto r = reader(fname);
    auto w = writer(output);

    w.write_header(r.read_header());

    std::deque<Snapshot> window;

    for(std::size_t i=0; i<window_size; ++i)
    {
        auto frame_opt = r.read_frame();
        if(not frame_opt.has_value())
        {
            log::fatal("trajectory is shorter than the window!");
        }
        window.push_back(std::move(frame_opt.value()));
    }

    const auto normalize = 1.0 / window_size;
    while(not r.is_eof())
    {
        // write out the averaged value

        Snapshot average = window.front();
        window.pop_front();

        for(const auto& frame : window)
        {
            for(std::size_t i=0; i<frame.size(); ++i)
            {
                average.at(i).position() += frame.at(i).position();
            }
        }
        for(auto& particle : average)
        {
            particle.position() *= normalize;
        }
        w.write_frame(average);

        // read the next frame

        auto frame_opt = r.read_frame();
        if(not frame_opt.has_value())
        {
            break;
        }
        window.push_back(std::move(frame_opt.value()));
    }
    return 0;
}

} // mill
