#include "mode_ninfo.hpp"
#include "mode_ninfo_help.hpp"
#include "mode_ninfo_split.hpp"
#include <mill/util/logger.hpp>

namespace mill
{

int mode_ninfo(int argument_c, const char** argument_v)
{
    if(argument_c < 2)
    {
        log::error("mill ninfo mode: too few arguments");
        mode_ninfo_help({});
        return 1;
    }

    const std::string command(argument_v[1]);
    if(command == "split")
    {
        return mode_ninfo_split(--argument_c, ++argument_v);
    }
    else if(command == "split")
    {
        mode_ninfo_split(--argument_c, ++argument_v);
        return 0;
    }
    else
    {
        log::error("mill ninfo mode: unknown command", command);
        log::error(mode_ninfo_help_usage());
        return 1;
    }
}

} // mill
