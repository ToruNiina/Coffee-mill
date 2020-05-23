#ifndef COFFEE_MILL_DCD_CALC_THETA
#define COFFEE_MILL_DCD_CALC_THETA
#include <mill/dcd/DCDReader.hpp>
#include <toml/toml.hpp>

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
template<typename vectorT>
int mode_dcd_calc_theta(int argument_c, const char **argument_v)
{
    if(argument_c == 1)
    {
        std::cerr << "error: mill dcd calc_theta: too few arguments\n";
        std::cerr << dcd_extract_usage() << std::endl;
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        std::cerr << dcd_calc_theta_usage() << std::endl;
        return 0;
    }

    if(fname.substr(fname.size()-5, 5) == ".toml")
    {
        const auto data   = toml::parse(fname);
        const auto input  = toml::find<std::string>(data, "input");
        const auto output = toml::find<std::string>(data, "output");

        const auto vidx1 = toml::find<std::array<std::vector<std::size_t>, 2>>(data, "vector1");
        const auto vidx2 = toml::find<std::array<std::vector<std::size_t>, 2>>(data, "vector2");

        std::ofstream ofs(output);
        if(!ofs.good())
        {
            std::cerr << "file open error: " << output << std::endl;
            return 1;
        }
        ofs << "# dot product, theta\n";

        DCDReader reader(input);
        for(const auto& snapshot : reader)
        {
            vectorT v1_from(0., 0., 0.);
            for(const auto idx : vidx1[0])
            {
                v1_from += snapshot.at(idx).position();
            }
            v1_from /= vidx1[0].size();

            vectorT v1_to(0., 0., 0.);
            for(const auto idx : vidx1[1])
            {
                v1_to += snapshot.at(idx).position();
            }
            v1_to /= vidx1[1].size();

            vectorT v2_from(0., 0., 0.);
            for(const auto idx : vidx2[0])
            {
                v2_from += snapshot.at(idx).position();
            }
            v2_from /= vidx2[0].size();

            vectorT v2_to(0., 0., 0.);
            for(const auto idx : vidx2[1])
            {
                v2_to += snapshot.at(idx).position();
            }
            v2_to /= vidx2[1].size();


            const vectorT v1 = v1_to - v1_from;
            const vectorT v2 = v2_to - v2_from;

            const vectorT v1_n = v1 / length(v1);
            const vectorT v2_n = v2 / length(v2);

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
        std::cerr << "error: mill dcd calc_theta: unknown file extension: "
                  << fname << '\n';
        std::cerr << dcd_calc_theta_usage() << std::endl;
        return 1;
    }
}

} // mill


#endif// COFFEE_MILL_DCD_CALC_THETA
