#ifndef COFFEE_MILL_NINFO_MODE
#define COFFEE_MILL_NINFO_MODE
#include "NinfoSplitter.hpp"
// #include "toml.hpp"

namespace mill
{

template<typename realT>
int mode_ninfo(int argument_c, char** argument_v)
{
    const std::string command(argument_v[1]);

    if(command == "split")
    {
        const std::string arg2(argument_v[2]);
        if(arg2.substr(arg2.size()-6, 6) == ".ninfo")
        {
            NinfoSplitter<double> splitter;
            auto splitted = splitter.split(arg2);
            splitter.write(splitted, arg2.substr(0, arg2.size()-6));
            return 0;
        }
        else if(arg2.substr(arg2.size()-5, 5) == ".toml")
        {
            // read toml file...
        }
        else
        {
            std::cerr << "usage: mill ninfo split [filename.ninfo] or" << std::endl;
            std::cerr << "     : mill ninfo split [inputfile.toml]" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "command not found: " << command << std::endl;
        return 1;
    }
    return 0;
}

} // mill
#endif /* COFFEE_MILL_NINFO_MODE */
