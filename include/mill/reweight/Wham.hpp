#ifndef COFFEE_MILL_REWEIGHT_WHAM_HPP
#define COFFEE_MILL_REWEIGHT_WHAM_HPP
#include <mill/util/logger.hpp>

#include "Histogram.hpp"
#include "ProbabilityDensity.hpp"
#include "PotentialFunction.hpp"
#include "ReactionCoordinate.hpp"

#include <vector>
#include <cmath>

namespace mill
{

class WHAMSolver
{
  public:

    WHAMSolver(const double kBT, const double tolerance,
               const std::size_t max_iteration)
        : kBT_(kBT), beta_(1.0 / kBT), tolerance_(tolerance), max_iteration_(max_iteration)
    {}

    ProbabilityDensity operator()(const std::vector<std::pair<
            std::vector<double>, std::unique_ptr<PotentialFunction>>>& trajs,
            const std::size_t bins = 100)
    {
        return this->reconstruct(this->solve_f(trajs), trajs, bins);
    }

    double kBT() const noexcept {return kBT_;}

  private:

    double relative_diff(const double lhs, const double rhs) const noexcept
    {
        return (lhs - rhs) / ((lhs + rhs) * 0.5);
    }

    std::vector<double> solve_f(const std::vector<std::pair<
            std::vector<double>, std::unique_ptr<PotentialFunction>>>& trajs);

    ProbabilityDensity reconstruct(const std::vector<double>& fs,
            const std::vector<std::pair<std::vector<double>,
                std::unique_ptr<PotentialFunction>>>& trajs,
            const std::size_t bins);

  private:
    double kBT_, beta_, tolerance_;
    std::size_t max_iteration_;
};

inline std::vector<double> WHAMSolver::solve_f(const std::vector<std::pair<
        std::vector<double>, std::unique_ptr<PotentialFunction>>>& trajs)
{
    const auto nwin = trajs.size();
    std::vector<double> expfs_prev(nwin, 1.0);

    // (l, i) -> l * nwin + i
    std::vector<std::vector<double>> cache_expW(nwin * nwin);
    for(std::size_t l = 0; l < nwin; ++l)
    {
        if(not trajs.at(l).second)
        {
            log::fatal("traj ", l, " does not have potential function");
        }

        const auto& Vl = *(trajs.at(l).second);
        for(std::size_t i = 0; i < nwin; ++i)
        {
            const auto& traj_i = trajs.at(i).first;
            std::vector<double> expWs_i(traj_i.size());
            for(std::size_t j=0; j < traj_i.size(); ++j)
            {
                expWs_i.at(j) = std::exp(-beta_ * Vl(traj_i.at(j)));
            }
            cache_expW.at(l * nwin + i) = expWs_i;
        }
    }
    log::info("expW cached");

    for(std::size_t iteration = 0; iteration < max_iteration_; ++iteration)
    {
        std::vector<double> expfs(trajs.size(), 0.0);

        for(std::size_t l = 0; l < nwin; ++l)
        {
            auto& expf = expfs.at(l);
            for(std::size_t i = 0; i < nwin; ++i)
            {
                const auto& expW   = cache_expW.at(l * nwin + i);
                const auto& traj_i = trajs.at(i).first;

                for(std::size_t j = 0; j < traj_i.size(); ++j)
                {
                    const double numer = expW.at(j);
                    double denom = 0.0;
                    for(std::size_t k = 0; k < nwin; ++k)
                    {
                        denom += trajs.at(k).first.size() *
                                 cache_expW.at(k * nwin + i).at(j) *
                                 expfs_prev.at(k);
                    }
                    expf += numer / denom;
                }
            }
        }

        for(auto& expf : expfs)
        {
            expf = 1.0 / expf;
        }

        double max_relative_diff = 0.0;
        for(std::size_t i=0; i<expfs.size(); ++i)
        {
            max_relative_diff = std::max(max_relative_diff,
                    relative_diff(expfs.at(i), expfs_prev.at(i)));
        }

        log::info(iteration, "-th iteration: diff = ", max_relative_diff);
        if(max_relative_diff < tolerance_)
        {
            log::info("parameter converged: ", expfs);
            return expfs;
        }
        expfs_prev = expfs;
    }
    log::fatal("WHAM does not converge after ", max_iteration_, " iteration.");
}

inline ProbabilityDensity WHAMSolver::reconstruct(const std::vector<double>& expfs,
    const std::vector<std::pair<std::vector<double>, std::unique_ptr<PotentialFunction>>>& trajs,
    const std::size_t bins)
{
    double start =  std::numeric_limits<double>::max();
    double stop  = -std::numeric_limits<double>::max();
    for(const auto& traj_rc : trajs)
    {
        for(const auto& x : traj_rc.first)
        {
            start = std::min(start, x);
            stop  = std::max(stop,  x);
        }
    }
    const double dx = (stop - start) / (bins - 1);
    start -= dx * 0.5;
    stop  += dx * 0.5;
    log::info("WHAM: range = [", start, ", ", stop, ") dx = ", dx);

    Histogram<double, double> unbiased(bins, start, stop);
    for(const auto& traj_rc : trajs)
    {
        for(const auto& x: traj_rc.first)
        {
            double denom = 0.0;
            for(std::size_t i=0; i<trajs.size(); ++i)
            {
                denom += trajs.at(i).first.size() * expfs.at(i) *
                         std::exp(-1.0 * this->beta_ * (*trajs.at(i).second)(x));
            }
            unbiased.at(x) += 1.0 / denom;
        }
    }
    return ProbabilityDensity(unbiased);
}


} // mill
#endif// COFFEE_MILL_MATH_HISTOGRAM_HPP
