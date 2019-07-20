#ifndef COFFEE_MILL_DCD_CALC_DIST
#define COFFEE_MILL_DCD_CALC_DIST
#include <mill/data/DCDReader.hpp>
#include <toml/toml.hpp>

namespace mill
{

inline const char* dcd_calc_dist_usage() noexcept
{
    return "usage: mill dcd calc_dist [parameters...]\n"
           "     $ mill dcd calc_dist input.toml\n";
}

// argv := arrayof{ "calc_dist", "filename", {rests...} }
template<typename vectorT>
int mode_dcd_calc_dist(int argument_c, const char **argument_v)
{
    if(argument_c == 1)
    {
        std::cerr << "error: mill dcd calc_dist: too few arguments\n";
        std::cerr << dcd_calc_dist_usage() << std::endl;
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        std::cerr << dcd_calc_dist_usage() << std::endl;
        return 0;
    }

    if(fname.substr(fname.size()-5, 5) == ".toml")
    {
        const auto data   = toml::parse(fname);
        const auto input  = toml::find<std::string>(data, "input");
        const auto output = toml::find<std::string>(data, "output");
        const auto from   = toml::find<std::size_t>(data, "from");
        const auto to     = toml::find<std::size_t>(data, "to");

        std::ofstream ofs(output);
        if(!ofs.good())
        {
            std::cerr << "file open error: " << output << std::endl;
            return 1;
        }
        ofs << "# distance\n";

        DCDReader<vectorT> reader;
        const auto dcdtraj = reader.read(input);
        for(const auto& snap : dcdtraj)
        {
            const vectorT v = snap.at(to) - snap.at(from);

            ofs << std::setw(20) << std::setprecision(12) << length(v) << '\n';
        }
        return 0;
    }
    else
    {
        std::cerr << "error: mill dcd calc_dist: unknown file extension: "
                  << fname << '\n';
        std::cerr << dcd_calc_dist_usage() << std::endl;
        return 1;
    }
}

} // mill

#endif// COFFEE_MILL_DCD_CALC_DIST
