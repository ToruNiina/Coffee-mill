#ifndef COFFEE_MILL_VECTOR_ROTATION
#define COFFEE_MILL_VECTOR_ROTATION
#include <boost/math/quaternion.hpp>
#include "Vector3.hpp"

namespace coffeemill
{
    using Realquat = boost::math::quaternion<double>;

    Realvec rotation(const double angle, const Realvec& axis, const Realvec& target)
    {
        const double sin_normalize(sin(angle * 0.5) / length(axis));
      
        const Realquat Q(cos(angle * 0.5), axis[0] * sin_normalize, 
                                           axis[1] * sin_normalize,
                                           axis[2] * sin_normalize);
        const Realquat P(0e0, target[0], target[1], target[2]);
        const Realquat S(Q * P * conj(Q));
      
        return Realvec(S.R_component_2(), S.R_component_3(), S.R_component_4());
    }
}
#endif//COFFEE_MILL_VECTOR_ROTATION
