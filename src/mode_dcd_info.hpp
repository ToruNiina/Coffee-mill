#ifndef COFFEE_MILL_DCD_INFO
#define COFFEE_MILL_DCD_INFO
#include <mill/dcd/DCDReader.hpp>

#include <string_view>
#include <deque>

namespace mill
{

inline const char* dcd_info_usage() noexcept
{
    return "usage: mill dcd info <traj.dcd>\n"
           "      dump header information of traj.dcd into stdout.\n"
           "      you can check the size of traj, number of particles included,"
           " and etc.\n";
}

inline int mode_dcd_info(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::error("mill dcd info: too few arguments");
        log::error(dcd_info_usage());
        return 1;
    }

    const auto fname = args.at(1);
    if(fname == "help")
    {
        log::info(dcd_info_usage());
        return 0;
    }

    if(fname.substr(fname.size()-4, 4) == ".dcd")
    {
        DCDReader reader(fname);
        const auto data = reader.read_header();

        std::cout << "number of frames       : " << data.at("nset")       << std::endl;
        std::cout << "initial step           : " << data.at("istart")     << std::endl;
        std::cout << "interval between frames: " << data.at("nstep_save") << std::endl;
        std::cout << "total step             : " << data.at("nstep")      << std::endl;
        std::cout << "number of units        : " << data.at("nunit")      << std::endl;
        std::cout << "number of particles    : " << data.at("nparticle")  << std::endl;
        std::cout << "delta t                : " << data.at("delta_t")    << std::endl;
        std::cout << "comment                : " << data.at("comment")    << std::endl;
        return 0;
    }
    else
    {
        log::error("mill dcd info: unknown file extension ", fname);
        log::error(dcd_info_usage());
        return 1;
    }
}

} // mill
#endif  //COFFEE_MILL_DCD_INFO
