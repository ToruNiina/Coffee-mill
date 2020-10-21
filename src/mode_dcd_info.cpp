#include <mill/dcd/DCDReader.hpp>
#include <mill/util/file_extension.hpp>
#include "mode_dcd_info.hpp"

namespace mill
{

const char* mode_dcd_info_usage() noexcept
{
    return "usage: mill dcd info <traj.dcd>\n"
           "      dump header information of traj.dcd into stdout.\n"
           "      you can check the size of traj, number of particles included,"
           " and etc.\n";
}

int mode_dcd_info(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::error("mill dcd info: too few arguments");
        log::error(mode_dcd_info_usage());
        return 1;
    }

    const auto fname = args.at(1);
    if(fname == "help")
    {
        log::info(mode_dcd_info_usage());
        return 0;
    }

    if(extension_of(fname) == ".dcd")
    {
        DCDReader reader(fname);
        const auto data  = reader.read_header();
        const auto frame = reader.read_frame();

        std::cout << "number of frames       : " << data.at("nset")       << std::endl;
        std::cout << "initial step           : " << data.at("istart")     << std::endl;
        std::cout << "interval between frames: " << data.at("nstep_save") << std::endl;
        std::cout << "total step             : " << data.at("nstep")      << std::endl;
        std::cout << "number of units        : " << data.at("nunit")      << std::endl;
        std::cout << "number of particles    : " << data.at("nparticle")  << std::endl;
        std::cout << "delta t                : " << data.at("delta_t")    << std::endl;
        std::cout << "comment                : " << data.at("comment")    << std::endl;
        std::cout << "boundary               : " << frame->boundary().kind() << std::endl;
        return 0;
    }
    else
    {
        log::error("mill dcd info: unknown file extension ", fname);
        log::error(mode_dcd_info_usage());
        return 1;
    }
}

} // mill

