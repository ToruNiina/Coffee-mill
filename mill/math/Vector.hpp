#ifndef COFFEE_MILL_VECTOR
#define COFFEE_MILL_VECTOR
#include "Matrix.hpp"
#include <mill/util/scalar_type_of.hpp>
#include <algorithm>
#include <cmath>

namespace mill
{

template<typename realT, std::size_t N>
using Vector = Matrix<realT, N, 1>;

// for vector 3d
template<typename coordT>
inline typename scalar_type_of<coordT>::type
dot_product(const coordT& lhs, const coordT& rhs)
{
    return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
}

template<typename coordT>
inline coordT
cross_product(const coordT& lhs, const coordT& rhs)
{
    return coordT(lhs[1] * rhs[2] - lhs[2] * rhs[1],
                  lhs[2] * rhs[0] - lhs[0] * rhs[2],
                  lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}

template<typename coordT>
inline typename scalar_type_of<coordT>::type
scalar_triple_product(const coordT& lhs, const coordT& mid, const coordT& rhs)
{
    return (lhs[1] * mid[2] - lhs[2] * mid[1]) * rhs[0] +
           (lhs[2] * mid[0] - lhs[0] * mid[2]) * rhs[1] +
           (lhs[0] * mid[1] - lhs[1] * mid[0]) * rhs[2];
}

template<typename coordT>
inline typename scalar_type_of<coordT>::type
length_sq(const coordT& lhs)
{
    return lhs[0] * lhs[0] + lhs[1] * lhs[1] + lhs[2] * lhs[2];
}

template<typename coordT>
inline typename scalar_type_of<coordT>::type
length(const coordT& lhs)
{
    return std::sqrt(length_sq(lhs));
}

template<typename coordT>
inline typename scalar_type_of<coordT>::type
regularize(const coordT& v)
{
    return v / length(v);
}

template<typename coordT>
inline typename scalar_type_of<coordT>::type
angle(const coordT& lhs, const coordT& rhs)
{
    using real_type = typename scalar_type_of<coordT>::type;
    const auto l_reg = regularize(lhs);
    const auto r_reg = regularize(rhs);
    return std::acos(std::clamp<real_type>(dot_product(l_reg, r_reg), -1, 1));
}

} // mill
#endif /* COFFEE_MILL_VECTOR */
