#ifndef COFFEE_MILL_REWEIGHT_HISTOGRAM_HPP
#define COFFEE_MILL_REWEIGHT_HISTOGRAM_HPP
#include <mill/util/logger.hpp>
#include <vector>
#include <cmath>

namespace mill
{

template<typename realT, typename countT = std::size_t>
class Histogram
{
  public:

    using real_type       = realT;
    using count_type      = countT;
    using container_type  = std::vector<count_type>;
    using iterator        = typename container_type::iterator;
    using const_iterator  = typename container_type::const_iterator;

  public:

    Histogram() = default;
    ~Histogram() = default;

    Histogram(const std::size_t bin,
              const real_type start, const real_type stop) noexcept
        : start_(start), stop_(stop), dx_((stop - start) / bin), container_(bin)
    {}

    count_type at(const real_type x) const
    {
        if(x < start_ || stop_ <= x)
        {
            return 0;
        }
        return container_.at(x_to_idx(x));
    }
    count_type& at(const real_type x)
    {
        if(x < start_ || stop_ <= x)
        {
            log::fatal("Histogram::at: x(", x, ") is out of range [",
                       start_, ", ", stop_, ").");
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
