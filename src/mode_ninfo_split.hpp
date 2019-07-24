#ifndef COFFEE_MILL_NINFO_SPLIT
#define COFFEE_MILL_NINFO_SPLIT
#include <mill/ninfo/NinfoReader.hpp>
#include <mill/ninfo/NinfoWriter.hpp>

namespace mill
{

inline const char* ninfo_split_usage() noexcept
{
    return "usage: mill ninfo split [parameters...]\n"
           "    $ mill ninfo split data.ninfo \n"
           "      split data.ninfo into one-by-one ninfo files\n";
}

//! argv = {"split", {args...}}
template<typename vectorT>
int mode_ninfo_split(int argument_c, const char** argument_v)
{
    typedef typename scalar_type_of<vectorT>::type realT;

    if(argument_c == 1)
    {
        std::cerr << "error: mill ninfo split: too few arguments\n";
        std::cerr << ninfo_split_usage() << std::endl;
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        std::cerr << ninfo_split_usage() << std::endl;
        return 0;
    }

    if(fname.size() < 6)
    {
        std::cerr << "error: mill ninfo split: invalid argument : "
                  << fname << '\n';
        std::cerr << ninfo_split_usage() << std::endl;
        return 1;
    }

    if(fname.substr(fname.size() - 6, 6) == ".ninfo")
    {
        //! argv = {"split", "traj.ninfo"}
        NinfoReader<realT> reader;
        const auto all_in_one = reader.read(fname);

        std::map<std::pair<std::size_t, std::size_t>, NinfoData<realT>> splitted;
        for(const auto& block : all_in_one)
        {
            const NinfoKind kind = block.first;
            for(const auto& line : block.second)
            {
                const auto units =
                    std::make_pair(line->unit_at(0), line->unit_at(1));

                // search this pair of units is already found or not.
                if(splitted.count(units) == 0)
                {
                    splitted.emplace(units, NinfoData<realT>());
                }

                // search this kind of native information is already found or not.
                if(splitted.at(units).count(kind) == 0)
                {
                    splitted.at(units).emplace(kind,
                            std::vector<std::shared_ptr<NinfoBase<realT>>>{});
                }

                // insert data to return value.
                splitted[units][kind].push_back(line);
            }
        }

        NinfoWriter<realT> writer;
        const std::string file_prefix = fname.substr(0, fname.size() - 6);
        for(const auto& one_by_one : splitted)
        {
            const auto units = one_by_one.first;
            const std::string outname = file_prefix + std::string("_unit") +
                std::to_string(units.first)  + std::string("-") +
                std::to_string(units.second) + std::string(".ninfo");
            writer.write(one_by_one.second, outname);
        }

        return 0;
    }
    else
    {
        std::cerr << "error: mill ninfo split: invalid argument : "
                  << fname << '\n';
        std::cerr << ninfo_split_usage() << std::endl;
        return 1;
    }
}



} // mill
#endif// COFFEE_MILL_NINFO_SPLIT
