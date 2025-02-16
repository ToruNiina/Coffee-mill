#include "mode_calc_autocorrelation.hpp"

#include <mill/util/cmdarg.hpp>
#include <mill/math/Vector.hpp>
#include <mill/traj.hpp>
#include <toml.hpp>
#include <numeric>

namespace mill
{

const char* mode_calc_autocorrelation_usage() noexcept
{
    return "usage: mill calc autocorrelation {filename} [--column=(0 by default)] [--output=(\"mill_autocorrelation.dat\" by default)]\n"
           "       calculates autocorrelation using a column (0-indexed) of a space-separated file.\n"
           "       line starts with `#` will be skipped as a comment.\n"
           ;
}

int mode_calc_autocorrelation(std::deque<std::string_view> args)
{
    const auto col = pop_argument<std::size_t>(args, "column").value_or(0);
    const auto out = pop_argument<std::string>(args, "output")
                     .value_or("mill_autocorrelation.dat");

    if(args.empty())
    {
        log::error("mill calc autocorrelation: too few arguments.");
        log::error(mode_calc_autocorrelation_usage());
        return 1;
    }

    const std::string fname(args.front());
    if(fname == "help")
    {
        log::info(mode_calc_autocorrelation_usage());
        return 0;
    }

    std::vector<double> traj;

    std::size_t n_line=0;
    std::ifstream ifs(fname);
    if(not ifs.good())
    {
        log::fatal("file open error: ", out);
    }

    log::info("reading ", fname);
    while(not ifs.eof())
    {
        n_line += 1;

        std::string line;
        std::getline(ifs, line);
        if(line.empty() || line.at(0) == '#')
        {
            continue;
        }

        std::string buf;
        std::istringstream iss(line);
        for(std::size_t i=0; i<=col; ++i)
        {
            iss >> buf;
        }
        if(iss.fail() || buf.empty())
        {
            log::fatal("line ", n_line, " does not have ", col, " column.");
        }
        traj.push_back(std::stod(buf));
    }
    log::info("done.");

    std::ofstream ofs(out);
    if(not ofs.good())
    {
        log::fatal("file open error: ", out);
    }
    for(std::size_t window=1; window<traj.size(); ++window)
    {
        std::size_t num_sample = 0;
        double      sum        = 0.0;
        for(std::size_t i=window; i < traj.size(); ++i)
        {
            num_sample += 1;
            sum += traj.at(i) * traj.at(i - window);
        }
        ofs << window << ' ' << sum / static_cast<double>(num_sample) << '\n';
    }
    return 0;
}

} // mill
