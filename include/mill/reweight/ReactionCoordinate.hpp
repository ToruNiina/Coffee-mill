#ifndef COFFEE_MILL_REWEIGHT_REACTION_COORDINATE_HPP
#define COFFEE_MILL_REWEIGHT_REACTION_COORDINATE_HPP
#include <mill/reweight/ProbabilityDensity.hpp>
#include <mill/common/Trajectory.hpp>
#include <mill/common/Snapshot.hpp>
#include <mill/util/logger.hpp>
#include <mill/math/Vector.hpp>
#include <vector>
#include <cmath>

namespace mill
{

class ReactionCoordinate
{
  public:
    using snapshot_type = Snapshot;
    using real_type     = snapshot_type::real_type;
    using vector_type   = snapshot_type::vector_type;

    virtual ~ReactionCoordinate() = default;
    virtual real_type operator()(const snapshot_type&) const = 0;
    virtual real_type weight(const real_type) const = 0;
};

class ReactionDistance final : public ReactionCoordinate
{
  public:
    using base_type   = ReactionCoordinate;
    using real_type   = base_type::real_type;
    using vector_type = base_type::vector_type;

    ReactionDistance(const std::size_t from, const std::size_t to) noexcept
        : from_(from), to_(to)
    {}
    ~ReactionDistance() override = default;

    real_type operator()(const snapshot_type& frame) const override
    {
        return length(frame.at(to_).position() - frame.at(from_).position());
    }
    real_type weight(const real_type x) const override
    {
        return x * x;
    }

  private:
    std::size_t from_, to_;
};

inline Histogram<double, double>
make_histogram(const Trajectory& traj, const ReactionCoordinate& rc,
               const std::size_t bins = 100)
{
    double min_x =  std::numeric_limits<double>::max();
    double max_x = -std::numeric_limits<double>::max();
    std::vector<double> samples; samples.reserve(traj.size());

    for(const auto& frame : traj)
    {
        const auto x = rc(frame);
        min_x = std::min(min_x, x);
        max_x = std::max(max_x, x);
        samples.push_back(x);
    }

    const auto dx = (max_x - min_x) / (bins - 1);
    Histogram<double, double> hist(bins, min_x - dx * 0.5, max_x + dx * 0.5);
    for(const auto& s : samples)
    {
        hist.at(s) += 1.0;
    }

    for(std::size_t i=0; i<bins; ++i)
    {
        const double x = hist.start() + dx * (i+0.5);
        hist.at(x) /= rc.weight(x);
    }
    return hist;
}

} // mill
#endif// COFFEE_MILL_MATH_REACTION_COORDINATE_HPP
