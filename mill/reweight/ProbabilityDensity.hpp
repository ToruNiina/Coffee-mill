#ifndef COFFEE_MILL_REWEIGHT_PROBABILITY_DENSITY_HPP
#define COFFEE_MILL_REWEIGHT_PROBABILITY_DENSITY_HPP
#include "Histogram.hpp"
#include <mill/util/logger.hpp>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

namespace mill
{

class ProbabilityDensity
{
  public:

    using real_type       = double;
    using container_type  = std::vector<real_type>;
    using iterator        = typename container_type::iterator;
    using const_iterator  = typename container_type::const_iterator;

  public:

    template<typename T, typename U>
    explicit ProbabilityDensity(const Histogram<T, U>& hist)
        : start_(hist.start()), stop_(hist.stop()), dx_(hist.dx()),
          container_(hist.bins())
    {
        using count_type = typename Histogram<T, U>::count_type;
        const auto total = std::accumulate(hist.begin(), hist.end(),
                                           count_type(0));
        std::transform(hist.begin(), hist.end(), container_.begin(),
                [total](const auto& v) -> real_type {
                    return real_type(v) / total;
                });
    }

    real_type at(const real_type x) const
    {
        if(x < start_ || stop_ <= x)
        {
            return real_type(0);
        }
        return container_.at(x_to_idx(x));
    }

    real_type   start() const noexcept {return start_;}
    real_type   stop()  const noexcept {return stop_;}
    real_type   dx()    const noexcept {return dx_;}
    std::size_t bins()  const noexcept {return container_.size();}

    const_iterator begin()  const noexcept {return container_.begin();}
    const_iterator end()    const noexcept {return container_.end();}
    const_iterator cbegin() const noexcept {return container_.cbegin();}
    const_iterator cend()   const noexcept {return container_.cend();}

  private:

    std::size_t x_to_idx(const real_type x) const noexcept
    {
        return static_cast<std::size_t>(std::floor((x - start_) / dx_));
    }

  private:

    real_type start_;
    real_type stop_;
    real_type dx_;
    container_type container_;
};

} // mill
#endif// COFFEE_MILL_MATH_HISTOGRAM_HPP
