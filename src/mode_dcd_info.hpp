#ifndef COFFEE_MILL_DCD_INFO
#define COFFEE_MILL_DCD_INFO
#include <mill/data/DCDWriter.hpp>
#include <mill/data/DCDReader.hpp>

namespace mill
{

inline const char* dcd_info_usage() noexcept
{
    return "usage: mill dcd info <traj.dcd>\n"
           "      dump header information of traj.dcd into stdout.\n"
           "      you can check the size of traj, number of particles included,"
           " and etc.\n";
}

template<typename vectorT>
int mode_dcd_info(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        std::cerr << "error: mill dcd info: too few arguments\n";
        std::cerr << dcd_info_usage() << std::endl;
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        std::cerr << dcd_info_usage() << std::endl;
        return 0;
    }

    if(fname.substr(fname.size()-4, 4) == ".dcd")
    {
        std::ifstream ifs(fname);
        DCDReader<vectorT> reader;
        const auto data = reader.read_header(ifs);

        std::cout << "number of frames       : " << data.nset() << std::endl;
        std::cout << "initial step           : " << data.istart() << std::endl;
        std::cout << "interval between frames: " << data.nstep_save() << std::endl;
        std::cout << "total step             : " << data.nstep() << std::endl;
        std::cout << "number of units        : " << data.nunit() << std::endl;
        std::cout << "number of particles    : " << data.nparticle() << std::endl;
        std::cout << "delta t                : " << data.delta_t() << std::endl;
        std::cout << "comment                :\n";
        for(const auto& str : data.comment())
        {
            std::cout << str << std::endl;
        }
        return 0;
    }
    else
    {
        std::cerr << "error: mill dcd info: unknown file extension"
                  << fname << '\n';
        std::cerr << dcd_info_usage() << std::endl;
        return 1;
    }
}

} // mill
#endif  //COFFEE_MILL_DCD_INFO
