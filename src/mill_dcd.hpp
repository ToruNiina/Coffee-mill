#ifndef COFFEE_MILL_DCD_MODE
#define COFFEE_MILL_DCD_MODE
#include "DCDtoMovie.hpp"
#include "SuperImpose.hpp"
#include "mill_dcd_join.hpp"
#include "mill_dcd_extract.hpp"

namespace mill
{

// argv := arrayof{ "dcd", "command-name", {rests...} }
template<typename vectorT>
int mode_dcd(int argument_c, char **argument_v)
{
    if(argument_c < 2) throw std::invalid_argument("too few commands");
    const std::string command(argument_v[1]);

    if(command == "make-movie")
    {
        if(argument_c != 3)
        {
            std::cerr << "usage: mill dcd make-movie [file.dcd | file.toml]"
                      << std::endl;
            return 1;
        }
        const std::string fname(argument_v[2]);
        return dcd_to_movie<vectorT>(fname);
    }
    else if(command == "impose")
    {
        if(argument_c != 3)
        {
            std::cerr << "usage: mill dcd impose [file.dcd | file.toml]"
                      << std::endl;
            return 1;
        }
        std::string fname(argument_v[2]);
        return superimpose<vectorT>(fname);
    }
    else if(command == "join")
    {
        if(argument_c < 3)
        {
            std::cerr << "usage: mill dcd join [[file.dcd ...]| file.toml]"
                      << std::endl;
        }
        return mill_dcd_join<vectorT>(argument_c - 2, argument_v+2);
    }
    else if(command == "extract")
    {
        if(argument_c < 3)
        {
            std::cerr << "usage: mill dcd extract [file.dcd | file.toml]"
                      << " [begin, end]" << std::endl;
            return 1;
        }
        return mill_dcd_extract<vectorT>(argument_c - 1, argument_v+1);
    }
    else
    {
        std::cerr << "error: mill command not found" << std::endl;
        return 1;
    }
}

} // mill

#endif /* COFFEE_MILL_PDB_MODE */
