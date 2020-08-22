#ifndef COFFEE_MILL_REWEIGHT_POTENTIAL_FUNCTION_HPP
#define COFFEE_MILL_REWEIGHT_POTENTIAL_FUNCTION_HPP
#include "Histogram.hpp"
#include "ProbabilityDensity.hpp"
#include "ReactionCoordinate.hpp"
#include <mill/common/Snapshot.hpp>
#include <mill/util/logger.hpp>
#include <vector>
#include <cmath>

namespace mill
{

class PotentialFunction
{
  public:
    using snapshot_type = Snapshot;
    using real_type     = snapshot_type::real_type;
    using vector_type   = snapshot_type::vector_type;

    explicit PotentialFunction(std::unique_ptr<ReactionCoordinate> rc)
        : rc_(std::move(rc))
    {}
    virtual ~PotentialFunction() = default;
    virtual real_type operator()(const snapshot_type&) const          = 0;
    virtual real_type operator()(const real_type&)     const noexcept = 0;

    real_type rc(const snapshot_type& frame) const
    {
        return (*rc_)(frame);
    }

  protected:

    std::unique_ptr<ReactionCoordinate> rc_;
};

class HarmonicPotential final : public PotentialFunction
{
  public:
    using base_type = PotentialFunction;
    using real_type = typename base_type::real_type;

    HarmonicPotential(std::unique_ptr<ReactionCoordinate> rc,
                      const double k, const double x0)
        : base_type(std::move(rc)), k_(k), x0_(x0)
    {}
    ~HarmonicPotential() override = default;

    real_type operator()(const snapshot_type& frame) const override
    {
        const auto dx = (*this->rc_)(frame) - this->x0_;
        return this->k_ * dx * dx;
    }
    real_type operator()(const real_type& x) const noexcept override
    {
        const auto dx = x - x0_;
        return this->k_ * dx * dx;
    }

  private:

    double k_, x0_;
};

inline ProbabilityDensity
reweight(Histogram<double, double> hist, const PotentialFunction& pot,
         const double kBT = 0.59587)
{
    const auto beta = 1.0 / kBT;
    for(std::size_t i=0; i<hist.bins(); ++i)
    {
        const double x = hist.start() + hist.dx() * (i+0.5);
        hist.at(x) /= std::exp(-beta * pot(x));
    }
    return ProbabilityDensity(hist);
}

} // mill
#endif// COFFEE_MILL_MATH_REACTION_COORDINATE_HPP
