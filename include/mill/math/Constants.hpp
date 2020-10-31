#ifndef COFFEE_MILL_MATH_CONSTANT_HPP
#define COFFEE_MILL_MATH_CONSTANT_HPP

namespace mill
{
namespace constants
{

template <typename T>
inline constexpr T pi_v = 3.141592653589793238;
inline constexpr double pi = pi_v<double>;

template <typename T>
inline constexpr T tau_v = pi_v<T> * 2;
inline constexpr double tau = tau_v<double>;


} // constants
} // mill
#endif//COFFEE_MILL_MATH_CONSTANT_HPP
