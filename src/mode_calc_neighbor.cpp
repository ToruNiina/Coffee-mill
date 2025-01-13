#include "mode_calc_neighbor.hpp"

#include <mill/math/Vector.hpp>
#include <mill/traj.hpp>
#include <toml/toml.hpp>

namespace mill
{

const char* mode_calc_neighbor_usage() noexcept
{
    return "usage: mill calc neighbor <cutoff> <trajfile>\n"
           "       lists neighboring particles within <cutoff> range";
}

int mode_calc_neighbor(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc neighbor: too few arguments.");
        log::error(mode_calc_neighbor_usage());
        return 1;
    }

    if(args.front() == "help")
    {
        log::info(mode_calc_neighbor_usage());
        return 0;
    }
    const auto rc = std::stod(std::string(args.front()));
    args.pop_front();

    const auto fname = args.front();
    args.pop_front();

    std::cout << "i : neighbors\n";
    for(const auto& frame : reader(fname))
    {
        for(std::size_t i=0; i<frame.size(); ++i)
        {
            std::cout << i << " : ";
            const auto ri = frame.at(i).position();
            for(std::size_t j=0; j<frame.size(); ++j)
            {
                if(i == j) {continue;}
                const auto rj = frame.at(j).position();
                const auto dr = length(ri - rj);

                if(dr < rc)
                {
                    std::cout << " " << j;
                }
            }
            std::cout << '\n';
        }
        std::cout << "-------------------------------------------------------\n";
    }
    std::cout << std::flush;
    return 0;
}

} // mill
