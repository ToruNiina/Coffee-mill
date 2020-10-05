#ifndef COFFEE_MILL_MATH_VECTOR_HPP
#define COFFEE_MILL_MATH_VECTOR_HPP
#include "Matrix.hpp"
#include <algorithm>
#include <cmath>

namespace mill
{

template<typename realT, std::size_t N>
using Vector = Matrix<realT, N, 1>;

// for vector 3d
template<typename realT>
realT dot_product(const Vector<realT, 3>& lhs, const Vector<realT, 3>& rhs) noexcept
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

template<typename realT>
Vector<realT, 3>
cross_product(const Vector<realT, 3>& lhs, const Vector<realT, 3>& rhs) noexcept
{
    return Vector<realT, 3>(lhs[1] * rhs[2] - lhs[2] * rhs[1],
                            lhs[2] * rhs[0] - lhs[0] * rhs[2],
                            lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}

template<typename realT>
realT scalar_triple_product(const Vector<realT, 3>& lhs,
        const Vector<realT, 3>& mid, const Vector<realT, 3>& rhs) noexcept
{
    return (lhs[1] * mid[2] - lhs[2] * mid[1]) * rhs[0] +
           (lhs[2] * mid[0] - lhs[0] * mid[2]) * rhs[1] +
           (lhs[0] * mid[1] - lhs[1] * mid[0]) * rhs[2];
}

template<typename realT>
realT length_sq(const Vector<realT, 3>& lhs) noexcept
{
    return lhs[0] * lhs[0] + lhs[1] * lhs[1] + lhs[2] * lhs[2];
}

template<typename realT>
realT length(const Vector<realT, 3>& lhs) noexcept
{
    return std::sqrt(length_sq(lhs));
}

template<typename realT>
Vector<realT, 3> regularize(const Vector<realT, 3>& v) noexcept
{
    return v * (1.0 / length(v));
}

template<typename realT>
realT angle(const Vector<realT, 3>& lhs, const Vector<realT, 3>& rhs) noexcept
{
    const auto l_reg = regularize(lhs);
    const auto r_reg = regularize(rhs);
    return std::acos(std::clamp<realT>(dot_product(l_reg, r_reg), -1, 1));
}

} // mill
#endif /* COFFEE_MILL_VECTOR */
