#include "mode_pdb_help.hpp"
#include "mode_pdb_seq.hpp"
#include <mill/util/logger.hpp>

namespace mill
{

const char* mode_pdb_help_usage() noexcept
{
    return "usage: mill pdb [command] [parameters...]\n\n"
           "    avaiable commands\n"
           "    - seq\n"
           "        print a sequence of each chain\n"
           "    - mutate\n"
           "        mutate coarse-grained chain by renaming residues\n"
           "    - help\n"
           "        print detailed explanation of each command\n";
}

//! this function forwards the arguments to different modes.
// argv := { "help" or "pdb", {args...} }
int mode_pdb_help(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::info(mode_pdb_help_usage());
        return 0;
    }

    const auto command = args.front();
    if(command == "seq")
    {
        using namespace std::literals::string_view_literals;
        return mode_pdb_seq({"seq"sv, "help"sv});
    }
    else
    {
        log::error("mill pdb help: unknown command : ", command);
        log::error(mode_pdb_help_usage());
        return 1;
    }
}
} // mill
