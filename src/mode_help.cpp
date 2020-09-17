#include "mode_help.hpp"

#include <mill/util/logger.hpp>
#include <src/mode_dcd_help.hpp>
#include <src/mode_pdb_help.hpp>
#include <src/mode_ninfo_help.hpp>
#include <src/mode_calc_help.hpp>

namespace mill
{

const char* main_usage() noexcept
{
    return "Usage: mill [--debug|--quiet] [mode] [parameters...]\n"
           "# Log options\n"
           " - `--debug` shows debug informations.\n"
           " - `--quiet` disables all the status logs.\n"
           "# List of modes\n"
           " - pdb\n"
           " - dcd\n"
           " - ninfo\n"
           " - calc\n"
           " - help\n"
           "for more information, try `mill help [mode]`.\n"
           ;
}

//! this function forwards the arguments to different modes.
// argv := { "help", {args...} }
int mode_help(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::info(main_usage());
        return 0;
    }

    const auto command = args.front();

    if(command == "pdb")
    {
        return mode_pdb_help(std::move(args));
    }
    else if(command == "ninfo")
    {
        return mode_ninfo_help(std::move(args));
    }
    else if(command == "dcd")
    {
        return mode_dcd_help(std::move(args));
    }
    else if(command == "calc")
    {
        return mode_calc_help(std::move(args));
    }
    else
    {
        log::error("mill help mode: unknown command", command);
        log::error(main_usage());
        return 1;
    }
}

} // mill
