#ifndef COFFEE_MILL_DCD_CALC_DIST
#define COFFEE_MILL_DCD_CALC_DIST
#include <mill/dcd/DCDReader.hpp>
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
        log::error("mill dcd calc_dist: too few arguments");
        log::error(dcd_calc_dist_usage());
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        log::info(dcd_calc_dist_usage());
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
            log::error("file open error: ", output);
            return 1;
        }
        ofs << "# distance\n";

        DCDReader reader(input);
        reader.read_header();
        for(const auto& frame : reader)
        {
            const auto v = frame.at(to).position() - frame.at(from).position();
            ofs << std::setw(20) << std::setprecision(12) << length(v) << '\n';
        }
        return 0;
    }
    else
    {
        log::error("mill dcd calc_dist: unknown file extension: ", fname);
        log::error(dcd_calc_dist_usage());
        return 1;
    }
}

} // mill

#endif// COFFEE_MILL_DCD_CALC_DIST
