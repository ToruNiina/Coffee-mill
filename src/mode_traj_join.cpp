#include <toml.hpp>
#include <mill/util/logger.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/traj.hpp>
#include "mode_traj_join.hpp"
#include <iostream>

namespace mill
{

const char* mode_traj_join_usage() noexcept
{
    return "usage: mill traj join [parameters...]\n"
           "     - mill traj join traj1.dcd traj2.dcd traj3.dcd\n"
           "       concatenates dcd files and write traj1_joined.dcd\n"
           "     - mill traj join input.toml\n"
           "       specify input, output, and format via TOML file.\n"
           "```toml\n"
           "inputs = [\n"
           "  \"data/traj1.dcd\",\n"
           "  \"data/traj2.dcd\",\n"
           "]\n"
           "output = \"data/traj.dcd\"\n"
           "# When you re-start a simulation from the last snapshot of a traj file, the\n"
           "# next traj file would start with the snapshot and it causes redundancy.\n"
           "# If you want to remove the initial frame from a traj file that would be joined,\n"
           "# set `include_initial = false`.\n"
           "include_initial = true # by default, true\n"
           "```\n";
}

int mode_traj_join(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill traj join: too few arguments");
        log::error(mode_traj_join_usage());
        return 1;
    }

    const auto arg0 = args.front();
    args.pop_front();

    if(arg0 == "help")
    {
        log::info(mode_traj_join_usage());
        return 0;
    }

    std::vector<std::string> files;
    std::string output;
    bool include_initial = true;

    if(extension_of(arg0) == ".toml")
    {
        const auto input = toml::parse(std::string(arg0));

        output = toml::find<std::string>(input, "output");
        files  = toml::find<std::vector<std::string>>(input, "inputs");
        include_initial = toml::find_or(input, "include_initial", true);
    }
    else
    {
        output = std::string(base_name_of(arg0)) + std::string("_joined") +
                 std::string(extension_of(arg0));
        files.push_back(std::string(arg0));
        for(const auto item : args)
        {
            files.push_back(std::string(item));
        }
    }

    auto w = writer(output);
    w.write_header(reader(files.front()).read_header());
    for(const auto& file : files)
    {
        bool is_initial = true;
        for(const auto& frame : reader(file))
        {
            if(!is_initial || include_initial)
            {
                w.write_frame(frame);
            }
            is_initial = false;
        }
    }
    return 0;
}

} // mill
