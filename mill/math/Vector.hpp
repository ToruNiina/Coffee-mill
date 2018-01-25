#ifndef COFFEE_MILL_VECTOR
#define COFFEE_MILL_VECTOR
#include "Matrix.hpp"
#include <mill/util/scalar_type_of.hpp>
#include <boost/math/quaternion.hpp>
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
coordT
rotate(const typename scalar_type_of<coordT>::type angle,
       const coordT& axis, const coordT& target)
{
    using namespace boost::math;
    using quat = quaternion<typename scalar_type_of<coordT>::type>;

    const typename scalar_type_of<coordT>::type sin_normalize = 
        sin(angle * 0.5) / length(axis);

    const quat Q(cos(angle * 0.5), axis[0] * sin_normalize, 
                                   axis[1] * sin_normalize,
                                   axis[2] * sin_normalize);
    const quat P(0e0, target[0], target[1], target[2]);
    const quat S(Q * P * conj(Q));

    return coordT(S.R_component_2(), S.R_component_3(), S.R_component_4());
}

} // mill

#endif /* COFFEE_MILL_VECTOR */
