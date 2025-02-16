#include "mode_calc_local.hpp"

#include <mill/traj.hpp>
#include <toml.hpp>

#include <unordered_set>

namespace mill
{

const char* mode_calc_local_usage() noexcept
{
    return "usage: mill calc local [options...]\n"
           "    1. mill calc local [traj: filename] [index: uint] [cutoff: float]\n"
           "       - localance and angles around a particle specified by an index.\n"
           "       - output will be printed to stdout.\n";
}

struct mode_calc_local_input
{
    std::string trajfile;
    std::size_t index;
    double      cutoff;
    bool angle_next;
    bool angle_prev;
    bool angle_sandwich;

    mode_calc_local_input(std::deque<std::string_view> args)
      : trajfile      (std::string(args.at(0))),
        index         (std::stoull(std::string(args.at(1)))),
        cutoff        (std::stod(std::string(args.at(2)))),
        angle_next    (true),
        angle_prev    (true),
        angle_sandwich(true)
    {}
};

int mode_calc_local(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc local: too few arguments.");
        log::error(mode_calc_local_usage());
        return 1;
    }

    if(args.front() == "help")
    {
        log::info(mode_calc_local_usage());
        return 0;
    }
    if(args.size() != 3)
    {
        log::error("mill calc local: too few arguments.");
        log::error(mode_calc_local_usage());
        return 1;
    }

    const mode_calc_local_input config(args);

    const auto js = [&config]() {
        std::unordered_set<std::size_t> js;
        for(const auto& frame : reader(config.trajfile))
        {
            const auto ri = frame.at(config.index).position();
            for(std::size_t j=0; j<frame.size(); ++j)
            {
                const auto rj = frame.at(j).position();
                const auto dr = length(ri - rj);
                if(dr < config.cutoff)
                {
                    if( ! js.contains(j))
                    {
                        js.insert(j);
                    }
                }
            }
        }
        return std::vector<std::size_t>(js.begin(), js.end());
    }();

    std::cout << "t";
    for(const auto j : js)
    {
        std::cout << ' ' << j << " angle(i,j,j-1) angle(i,j,j+1) angle(i-j, j-1->j+1)";
    }
    std::cout << '\n';

    for(std::size_t f=0; const auto& frame : reader(config.trajfile))
    {
        log::debug(f, "-th frame.");
        std::cout << std::setw(6) << std::right << f;
        const auto ri = frame.at(config.index).position();
        for(const auto j : js)
        {
            const auto rj = frame.at(j).position();
            
            std::cout << ' ' << std::setprecision(16) << std::setw(20) << length(ri - rj);

            if(j != 0)
            {
                const auto rj_prev = frame.at(j-1).position();
                const auto rji  = ri - rj;
                const auto rjjp = rj_prev - rj;
                const auto theta = angle(rji, rjjp) / 3.14159265 * 180;
                std::cout << ' ' << std::setprecision(16) << std::setw(20) << theta;
            }
            else
            {
                std::cout << ' ' << std::setprecision(16) << std::setw(20) << 0.0;
            }

            if(j+1 != frame.size())
            {
                const auto rj_next = frame.at(j+1).position();
                const auto rji  = ri - rj;
                const auto rjjn = rj_next - rj;
                const auto theta = angle(rji, rjjn) / 3.14159265 * 180;
                std::cout << ' ' << std::setprecision(16) << std::setw(20) << theta;
            }
            else
            {
                std::cout << ' ' << std::setprecision(16) << std::setw(20) << 0.0;
            }

            if(j != 0 && j+1 != frame.size())
            {
                const auto rj_prev = frame.at(j-1).position();
                const auto rj_next = frame.at(j+1).position();

                const auto rji  = ri - rj;
                const auto rjpn = rj_next - rj_prev;
                const auto theta = angle(rji, rjpn) / 3.14159265 * 180;
                std::cout << ' ' << std::setprecision(16) << std::setw(20) << theta;
            }
            else
            {
                std::cout << ' ' << std::setprecision(16) << std::setw(20) << 0.0;
            }
        }
        std::cout << std::endl;
        f++;
    }

    return 0;
}

} // mill
