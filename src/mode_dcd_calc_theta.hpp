#ifndef COFFEE_MILL_DCD_CALC_THETA
#define COFFEE_MILL_DCD_CALC_THETA
#include <mill/data/DCDReader.hpp>
#include <toml/toml.hpp>

namespace mill
{

inline const char* dcd_calc_theta_usage() noexcept
{
    return "usage: mill dcd calc_theta [parameters...]\n"
           "    $ mill dcd calc_theta input.toml\n";
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
        const auto input  = toml::get<std::string>(data.at("input"));
        const auto output = toml::get<std::string>(data.at("output"));

        const auto vidx1  = toml::get<std::array<std::size_t, 2>>(data.at("vector1"));
        const auto vidx2  = toml::get<std::array<std::size_t, 2>>(data.at("vector2"));

        std::ofstream ofs(output);
        if(!ofs.good())
        {
            std::cerr << "file open error: " << output << std::endl;
            return 1;
        }
        ofs << "# dot product, theta\n";

        DCDReader<vectorT> reader;
        const auto dcdtraj = reader.read(input);
        for(const auto& snap : dcdtraj)
        {
            const vectorT v1 = snap.at(vidx1[1]) - snap.at(vidx1[0]);
            const vectorT v2 = snap.at(vidx2[1]) - snap.at(vidx2[0]);

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