#ifndef COFFEE_MILL_CYLINDORICAL_COORDINATE
#define COFFEE_MILL_CYLINDORICAL_COORDINATE
#include "mathematics/LinearAlgebra.hpp"
#include "mathematics/VectorRotation.hpp"
#include <cassert>

namespace coffeemill
{
    class CylindoricalCoordinate
    {
        public:
            CylindoricalCoordinate(const Realvec& c,
                                   const Realvec& r,
                                   const Realvec& z)
                : center(c), r_axis(r/length(r)), z_axis(z/length(z))
            {
                assert(dot_prod(r_axis, z_axis) < 1e-12);
            }
            ~CylindoricalCoordinate(){};

            std::array<double, 3> translate(const Realvec& position);
            Realvec translate(const std::array<double, 3>& position);

        private:
            const Realvec center;
            const Realvec r_axis;
            const Realvec z_axis;
    };

    std::array<double, 3> CylindoricalCoordinate::translate(
                                                    const Realvec& position)
    {
        Realvec relative_position = position - center;
        double z = dot_prod(relative_position, z_axis);
        Realvec planner = relative_position - (z*z_axis);
        double r = length(planner);
        double theta = acos(dot_prod(planner, r_axis)/r);

        if(dot_prod(cross_prod(planner, r_axis), z_axis) > 0e0)
            theta *= -1e0;

        return std::array<double, 3>({{r, theta, z}});
    }

    Realvec CylindoricalCoordinate::translate(const std::array<double, 3>&
                                                                 position)
    {
        return center
             + rotation(position[1], z_axis, position[0] * r_axis)
             + position[2] * z_axis;
    }

}

#endif//COFFEE_MILL_CYLINDORICAL_COORDINATE
