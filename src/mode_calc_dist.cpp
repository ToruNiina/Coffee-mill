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
    if(args.size() < 2)
    {
        log::error("mill calc dist: too few arguments.");
        log::error(mode_calc_dist_usage());
        return 1;
    }

    const auto fname = args.at(1);
    if(fname == "help")
    {
        log::info(mode_calc_dist_usage());
        return 0;
    }

    if(fname.size() < 5)
    {
        log::error("mill calc dist: unknown file format: ", fname);
        log::error(mode_calc_dist_usage());
        return 1;
    }

    if(extension_of(fname) == ".toml")
    {
        const auto config = toml::parse(std::string(fname));
        std::ofstream output(toml::find<std::string>(config, "output"));
        const auto traj  = toml::find<std::string>(config, "input");
        const auto pairs = toml::find<std::vector<std::pair<std::size_t, std::size_t>>>(config, "pairs");
        for(const auto frame : read(traj))
        {
            for(const auto [i, j] : pairs)
            {
                output << std::setprecision(16) << length(frame[i].position() - frame[j].position()) << ' ';
            }
            output << '\n';
        }
    }
    else
    {
        const auto i = std::stoi(std::string(args.at(2)));
        const auto j = std::stoi(std::string(args.at(3)));
        for(const auto frame : read(fname))
        {
            std::cout << std::setprecision(16) << length(frame[i].position() - frame[j].position()) << '\n';
        }
    }
    return 0;
}

} // mill
