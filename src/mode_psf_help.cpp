#include "mode_psf_gen.hpp"
#include "mode_psf_help.hpp"
#include <mill/util/logger.hpp>

namespace mill
{

const char* mode_psf_help_usage() noexcept
{
    return "usage: mill psf [command] [parameters...]\n\n"
           "    avaiable commands\n"
           "    - gen\n"
           "      : generate psf file for visualization from Mjolnir input file\n"
           "    - help\n"
           "      : print detailed explanation of each command\n";
}

//! this function forwards the arguments to different modes.
// argv := { "help" or "psf", {args...} }
int mode_psf_help(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::info(mode_psf_help_usage());
        return 0;
    }

    const auto command = args.front();

    if(command == "gen")
    {
        using namespace std::literals::string_view_literals;
        return mode_psf_gen({"gen"sv, "help"sv});
    }
    else
    {
        log::error("mill psf help: unknown command : ", command);
        log::error(mode_psf_help_usage());
        return 1;
    }
}
} // mill
