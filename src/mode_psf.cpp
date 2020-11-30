#include "mode_psf.hpp"
#include "mode_psf_help.hpp"
#include "mode_psf_gen.hpp"
#include <mill/util/logger.hpp>
#include <iostream>

namespace mill
{

int mode_psf(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill psf mode: too few arguments");
        mode_psf_help({});
        return 1;
    }

    const auto command = args.front();
    if(command == "gen")
    {
        return mode_psf_gen(std::move(args));
    }
    else if(command == "help")
    {
        return mode_psf_help(std::move(args));
    }
    else
    {
        log::error("mill psf mode: unknown command: ", command);
        log::error(mode_psf_help_usage());
        return 1;
    }
}

} // mill
