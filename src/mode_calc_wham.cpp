#include "mode_calc_wham.hpp"

#include <mill/reweight/Wham.hpp>

#include <mill/traj.hpp>

#include <toml.hpp>
#include <fstream>

namespace mill
{

const char* mode_calc_wham_usage() noexcept
{
    return "usage: mill calc wham [input file]\n"
        "```toml\n"
        "kBT       = 0.59587\n"
        "tolerance = 1e-4 # default\n"
        "bins      = 100  # default\n"
        "output    = \"mill_wham.dat\" # default\n"
        "[[trajectories]]\n"
        "trajectory = \"traj1.dcd\"\n"
        "potential.type = \"Harmonic\"\n"
        "potential.idxs = [179, 671]\n"
        "potential.k    = 10.0\n"
        "potential.v0   = 5.0\n"
        "[[trajectories]]\n"
        "trajectory = \"traj2.dcd\"\n"
        "potential.type = \"Harmonic\"\n"
        "potential.idxs = [179, 671]\n"
        "potential.k    = 10.0\n"
        "potential.v0   = 6.0\n"
        "# ...\n"
        "```\n";
}

int mode_calc_wham(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc wham: too few arguments.");
        log::error(mode_calc_wham_usage());
        return 1;
    }

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_calc_wham_usage());
        return 0;
    }
    const auto input = toml::parse(std::string(fname));

    const auto kBT           = toml::find_or<double>(input, "kBT", 0.59587);
    const auto tolerance     = toml::find_or<double>(input, "tolerance", 1e-4);
    const auto bins          = toml::find_or<std::size_t>(input, "bins", 100);
    const auto max_iteration = toml::find_or<std::size_t>(input, "max_iteration", 10000);

    WHAMSolver solver(kBT, tolerance, max_iteration);

    std::vector<std::pair<std::vector<double>,
                          std::unique_ptr<PotentialFunction>>> trajs;
    for(const auto& trajectory : input.at("trajectories").as_array())
    {
        // assuming harmonic bond length ...
        const auto pot   = toml::find(trajectory, "potential");
        const auto [i, j] = toml::find<std::pair<std::size_t, std::size_t>>(pot, "idxs");
        const auto k  = toml::find<double>(pot, "k");
        const auto v0 = toml::find<double>(pot, "v0");

        auto potential = std::make_unique<HarmonicPotential>(
                std::make_unique<ReactionDistance>(i, j), k, v0);

        std::vector<double> rcs;
        for(auto frame : reader(toml::find<std::string>(trajectory, "trajectory")))
        {
            rcs.push_back(potential->rc(frame));
        }
        trajs.emplace_back(std::move(rcs), std::move(potential));
    }

    const auto P = solver(trajs, bins);

    const auto output = toml::find_or(input, "output", "mill_wham.dat");
    std::ofstream ofs(output);
    ofs << "#x probability energy\n";
    for(std::size_t i=0; i<P.bins(); ++i)
    {
        const auto x = P.start() + P.dx() * (i + 0.5);
        ofs << x << ' ' << P.at(x) << ' ' << -kBT * std::log(P.at(x)) << '\n';
    }
    return 0;
}

}//mill
