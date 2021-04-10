#include "mode_calc_BAR.hpp"

#include <mill/util/logger.hpp>
#include <toml/toml.hpp>
#include <numeric>
#include <cmath>

namespace mill
{

const char* mode_calc_BAR_usage() noexcept
{
    return "usage: mill calc BAR [input.toml]\n"
           "       It calculates BAR from a column of a space-separated file.\n"
           "       Lines starting with `#` will be skipped as a comment.\n"
           "       Each file must contain 2 columns, energy values of self potential and other's potential, respectively.\n"
           "       ```toml\n"
           "       [input]\n"
           "       traj1 = \"traj1.dat\"\n"
           "       traj2 = \"traj2.dat\"\n"
           "       kBT   = 0.6 # ~300K in kcal/mol\n"
           "\n"
           "       [output]\n"
           "       filename = \"BAR.dat\"\n"
           "       C.start = -6.0\n"
           "       C.stop  =  6.0\n"
           "       C.delta =  0.01\n"
           "       ```\n"
           ;
}

std::vector<std::pair<double, double>> read_energies(const std::string& fname)
{
    std::ifstream ifs(fname);
    if(not ifs.good())
    {
        log::fatal("file open error: ", fname);
    }

    std::vector<std::pair<double, double>> es;
    std::size_t n_line = 0;

    log::info("reading ", fname, "...");
    while(not ifs.eof())
    {
        n_line += 1;

        std::string line;
        std::getline(ifs, line);
        if(line.empty() || line.at(0) == '#')
        {
            continue;
        }

        std::istringstream iss(line);
        double e1 = std::numeric_limits<double>::quiet_NaN();
        double e2 = std::numeric_limits<double>::quiet_NaN();
        iss >> e1 >> e2;
        if(iss.fail())
        {
            log::fatal("line ", n_line, " does not have 2 column.");
        }
        es.emplace_back(e1, e2);
    }
    log::info("done.");

    return es;
}

// <1 / (1 + exp(βU_other - βU_self - C))>
double calc_weights(const std::vector<std::pair<double, double>>& energies, const double C, const double beta)
{
    double W = 0.0;
    for(const auto [Uself, Uother] : energies)
    {
        W += 1.0 / (1.0 + std::exp(beta * (Uother - Uself) - C));
    }
    return W;
}

int mode_calc_BAR(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill calc BAR: too few arguments.");
        log::error(mode_calc_BAR_usage());
        return 1;
    }

    const std::string fname(args.front());
    args.pop_front();
    if(fname == "help")
    {
        log::info(mode_calc_BAR_usage());
        return 0;
    }

    const auto input = toml::parse(fname);

    const auto kBT  = toml::find<double>(input, "input", "kBT");
    const auto beta = 1.0 / kBT;

    const auto es1 = read_energies(toml::find<std::string>(input, "input", "traj1"));
    const auto es2 = read_energies(toml::find<std::string>(input, "input", "traj2"));

    const double logn1n2 = std::log(double(es2.size()) / double(es1.size()));

    const auto out = toml::find<std::string>(input, "output", "filename");
    std::ofstream ofs(out);
    if(not ofs.good())
    {
        log::fatal("file open error: ", out);
    }


    const auto C_beg = toml::find<double>(input, "output", "C", "start");
    const auto C_end = toml::find<double>(input, "output", "C", "stop");
    const auto dC    = toml::find<double>(input, "output", "C", "delta");
    const std::size_t nC = (C_end - C_beg) / dC;
    const double rnC = 1.0 / nC;

    ofs << "# C   weight1   weight2\n";
    for(std::size_t i=0; i<=nC; ++i)
    {
        const auto C = C_beg * (1.0 - i * rnC) + C_end * (i * rnC);
        ofs << C << ' ' << calc_weights(es1,  C, beta)
                 << ' ' << calc_weights(es2, -C, beta)
                 << ' ' << logn1n2 << '\n';
    }

    return 0;
}

} // mill
