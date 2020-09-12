#ifndef COFFEE_MILL_DCD_CALC_THETA
#define COFFEE_MILL_DCD_CALC_THETA
#include <mill/dcd/DCDReader.hpp>
#include <toml/toml.hpp>

#include <deque>
#include <string_view>

namespace mill
{

inline const char* dcd_calc_theta_usage() noexcept
{
    return "usage: mill dcd calc_theta [parameters...]\n"
           "    $ mill dcd calc_theta input.toml\n"
           "sample input file is like this.\n"
           "input   = \"traj.dcd\"\n"
           "output  = \"analyzed.dat\"\n"
           "vector1 = [[1, 2], [3]] # vector from (the center of 1 and 2) to (3).\n"
           "vector2 = [[4],    [5]] # vector from 4 to 5.\n";
}

// argv := arrayof{ "calc_theta", "filename", {rests...} }
inline int mode_dcd_calc_theta(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::error("mill dcd calc_theta: too few arguments");
        log::error(dcd_extract_usage());
        return 1;
    }

    const auto fname = args.at(1);
    if(fname == "help")
    {
        log::info(dcd_extract_usage());
        return 0;
    }
    args.pop_front();

    if(extension_of(fname) == ".toml")
    {
        const auto data   = toml::parse(std::string(fname));
        const auto input  = toml::find<std::string>(data, "input");
        const auto output = toml::find<std::string>(data, "output");

        const auto vidx1 = toml::find<std::array<std::vector<std::size_t>, 2>>(data, "vector1");
        const auto vidx2 = toml::find<std::array<std::vector<std::size_t>, 2>>(data, "vector2");

        std::ofstream ofs(output);
        if(!ofs.good())
        {
            log::error("mill dcd calc_theta: file open error: ", output);
            return 1;
        }
        ofs << "# dot product, theta\n";

        DCDReader reader(input);
        using vector_type = DCDReader::vector_type;

        for(const auto& snapshot : reader)
        {
            vector_type v1_from(0., 0., 0.);
            for(const auto idx : vidx1[0])
            {
                v1_from += snapshot.at(idx).position();
            }
            v1_from /= vidx1[0].size();

            vector_type v1_to(0., 0., 0.);
            for(const auto idx : vidx1[1])
            {
                v1_to += snapshot.at(idx).position();
            }
            v1_to /= vidx1[1].size();

            vector_type v2_from(0., 0., 0.);
            for(const auto idx : vidx2[0])
            {
                v2_from += snapshot.at(idx).position();
            }
            v2_from /= vidx2[0].size();

            vector_type v2_to(0., 0., 0.);
            for(const auto idx : vidx2[1])
            {
                v2_to += snapshot.at(idx).position();
            }
            v2_to /= vidx2[1].size();


            const vector_type v1 = v1_to - v1_from;
            const vector_type v2 = v2_to - v2_from;

            const vector_type v1_n = v1 / length(v1);
            const vector_type v2_n = v2 / length(v2);

            auto dot = dot_product(v1_n, v2_n);

            if     (dot < -1.0) {dot = -1.0;}
            else if(dot >  1.0) {dot =  1.0;}
            const auto ang = std::acos(dot);
            const double pi = 3.141592653589793238;

            ofs << std::setw(20) << std::setprecision(12) << dot << ' '
                << std::setw(20) << std::setprecision(12) << ang / pi * 180.0 << '\n';
        }
        return 0;
    }
    else
    {
        log::error("mill dcd calc_theta: unknown file extension: ", fname);
        log::error(dcd_extract_usage());
        return 1;
    }
}

} // mill


#endif// COFFEE_MILL_DCD_CALC_THETA
