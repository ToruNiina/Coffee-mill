#include <mill/util/logger.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/traj.hpp>
#include "mode_traj_translate.hpp"
#include <iostream>

namespace mill
{

const char* mode_traj_translate_usage() noexcept
{
    return "usage: mill traj translate [trajfile] [x y z]\n"
           "       translate the snapshot by a specified vector.\n";
}

int mode_traj_translate(std::deque<std::string_view> args)
{
    using namespace std::literals::string_view_literals;
    using namespace std::literals::string_literals;
    if(args.empty())
    {
        log::error("mill traj translate: too few arguments");
        log::error(mode_traj_translate_usage());
        return 1;
    }

    const auto input = args.front();
    args.pop_front();

    if(input == "help")
    {
        log::info(mode_traj_translate_usage());
        return 0;
    }

    if(args.size() != 3)
    {
        log::error("mill traj translate: too few arguments");
        log::error(mode_traj_translate_usage());
        return 1;
    }

    const auto x = std::stod(std::string(args.front())); args.pop_front();
    const auto y = std::stod(std::string(args.front())); args.pop_front();
    const auto z = std::stod(std::string(args.front())); args.pop_front();

    Vector<double, 3> dr(x, y, z);
    const auto output = std::string(base_name_of(input)) + "_translated_"s +
        std::to_string(x) + "_"s + std::to_string(y) + "_"s + std::to_string(z) +
        std::string(extension_of(input));

    auto w = writer(output);
    auto r = reader(input);
    w.write_header(r.read_header());

    for(auto frame : r)
    {
        for(auto& p : frame)
        {
            p.position() += dr;
        }
        w.write_frame(frame);
    }
    return 0;
}

} // mill
