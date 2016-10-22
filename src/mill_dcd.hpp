#ifndef COFFEE_MILL_DCD_MODE
#define COFFEE_MILL_DCD_MODE
#include "DCDtoMovie.hpp"
#include "SuperImpose.hpp"

namespace mill
{

template<typename vectorT>
int mode_dcd(int argument_c, char **argument_v)
{
    if(argument_c < 2) throw std::invalid_argument("too few commands");
    const std::string command(argument_v[1]);

    if(command == "make-movie")
    {
        if(argument_c != 3)
            throw std::invalid_argument(
                    "usage: ./mill dcd make-movie [file.dcd | file.toml]");
        std::string fname(argument_v[2]);
        return dcd_to_movie<vectorT>(fname);
    }
    else if(command == "impose")
    {
        if(argument_c != 3)
            throw std::invalid_argument(
                    "usage: ./mill dcd impose [file.dcd | file.toml]");
        std::string fname(argument_v[2]);
        return superimpose<vectorT>(fname);
    }
    else
    {
        throw std::invalid_argument("command not found");
    }
}

} // mill

#endif /* COFFEE_MILL_PDB_MODE */
