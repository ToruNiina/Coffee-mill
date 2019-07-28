#ifndef COFFEE_MILL_UTIL_CLAMP_HPP
#define COFFEE_MILL_UTIL_CLAMP_HPP
#include <algorithm>

namespace mill
{

template<typename T>
inline T clamp(const T& x, const T& low, const T& high) noexcept
{
    return std::max(low, std::min(high, x));
}

} // mill
#endif// COFFEE_MILL_UTIL_CLAMP_HPP
