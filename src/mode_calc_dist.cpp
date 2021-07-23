#include "mode_calc_dist.hpp"

#include <mill/traj.hpp>
#include <toml/toml.hpp>

namespace mill
{

const char* mode_calc_dist_usage() noexcept
{
    return "usage: mill calc dist [options...]\n"
           "    1. mill calc dist traj.dcd [i:size_t] [j:size_t]\n"
           "       - distance between only one pair can be calculated.\n"
           "       - output will be printed on stdout.\n"
           "    2. mill calc dist input.toml\n"
           "       - distance between several pair can be calculated simultaneously.\n"
           "       - output file can be specified via input file.\n"
           "    ```toml\n"
           "    output = \"output.dat\"\n"
           "    input  = \"traj.dcd\"\n"
           "    pairs  = [\n"
           "        [1, 2],\n"
           "        [2, 3],\n"
           "    ]\n"
           "    ```\n";
}

int mode_calc_dist(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc dist: too few arguments.");
        log::error(mode_calc_dist_usage());
        return 1;
    }

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_calc_dist_usage());
        return 0;
    }

    if(extension_of(fname) == ".toml")
    {
        const auto config = toml::parse(std::string(fname));

        const auto out_fname = toml::find<std::string>(config, "output");
        std::ofstream output(out_fname);
        if(not output.good())
        {
            log::fatal("output file ", out_fname, " could not be opened.");
        }

        const auto traj  = toml::find<std::string>(config, "input");
        const auto pairs = toml::find<std::vector<std::pair<std::size_t, std::size_t>>>(config, "pairs");

        log::debug(pairs.size(), "pairs found.");
        for(const auto& [i, j] : pairs)
        {
            log::debug(i,  ", ", j);
        }

        std::size_t f=0;
        for(const auto& frame : reader(traj))
        {
            log::debug(f, "-th frame.");
            output << std::setw(6) << std::right << f << ' ';
            for(const auto& [i, j] : pairs)
            {
                log::debug("calculating distance between ", i, ", ", j, " pair");
                output << std::setprecision(16) << std::setw(20) << length(frame[i].position() - frame[j].position()) << ' ';
            }
            output << std::endl;
            f++;
        }
    }
    else if(args.size() == 3) // traj.dcd 100 110
    {
        const auto i = std::stoi(std::string(args.at(1)));
        const auto j = std::stoi(std::string(args.at(2)));
        for(const auto& frame : reader(fname))
        {
            std::cout << std::setprecision(16)
                      << length(frame[i].position() - frame[j].position()) << '\n';
        }
    }
    else
    {
        log::error("mill calc dist: too few arguments.");
        log::error(mode_calc_dist_usage());
        return 1;
    }
    return 0;
}

} // mill
