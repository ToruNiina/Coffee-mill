#include "mode_ninfo.hpp"
#include "mode_ninfo_help.hpp"
#include "mode_ninfo_split.hpp"
#include <mill/util/logger.hpp>

namespace mill
{

int mode_ninfo(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::error("mill ninfo mode: too few arguments");
        mode_ninfo_help(std::move(args));
        return 1;
    }

    const auto command = args.at(1);
    args.pop_front();
    if(command == "split")
    {
        return mode_ninfo_split(std::move(args));
    }
    else
    {
        log::error("mill ninfo mode: unknown command", command);
        log::error(mode_ninfo_help_usage());
        return 1;
    }
}

} // mill
