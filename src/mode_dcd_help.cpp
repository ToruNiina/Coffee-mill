#include <mill/math/Vector.hpp>
#include <mill/util/logger.hpp>
#include "mode_dcd_info.hpp"
#include "mode_dcd_help.hpp"
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_dcd_help_usage() noexcept
{
    return "usage: mill dcd [command] [parameters...]\n\n"
           "    avaiable commands\n"
           "    - impose\n"
           "      : superimpose all the snapshots by minimizing RMSD\n"
           "    - info\n"
           "      : dump header information of dcd file\n"
           "    - help\n"
           "      : print detailed explanation of each command\n";
}

//! this function forwards the arguments to different modes.
// argv := { "help" or "dcd", {args...} }
int mode_dcd_help(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::info(mode_dcd_help_usage());
        return 0;
    }

    const auto command = args.at(1);

    using namespace std::literals::string_view_literals;
    if(command == "impose")
    {
        return mode_dcd_impose({"impose"sv, "help"sv});
    }
    else if(command == "info")
    {
        return mode_dcd_info({"info"sv, "help"sv});
    }
    else
    {
        log::error("mill dcd help: unknown command : ", command);
        log::error(mode_dcd_help_usage());
        return 1;
    }
}
} // mill
