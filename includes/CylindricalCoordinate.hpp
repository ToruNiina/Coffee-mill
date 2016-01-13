#ifndef COFFEE_MILL_CYLINDRICAL_COORDINATE
#define COFFEE_MILL_CYLINDRICAL_COORDINATE
#include "mathematics/LinearAlgebra.hpp"
#include "mathematics/VectorRotation.hpp"
#include <cassert>

namespace coffeemill
{
    class CylindricalCoordinate
    {
        public:
            CylindricalCoordinate(const Realvec& c,
                                   const Realvec& r,
                                   const Realvec& z)
                : center(c), r_axis(r/length(r)), z_axis(z/length(z))
            {
                if(fabs(dot_prod(r_axis, z_axis)) > 1e-12)
                {
                    std::cout << "Warning: dotp(r, z) = "
                              << dot_prod(r_axis, z_axis)
                              <<  " is not zero. over tolerance" << std::endl;
                    double z_axis_r = dot_prod(r_axis, z_axis);
                    z_axis -= z_axis_r * r_axis;
                    z_axis /= length(z_axis);
                    assert(fabs(dot_prod(r_axis, z_axis)) < 1e-12);
                    assert(fabs(length(z_axis) - 1e0) < 1e-12);
                    std::cout << "new z axis = " << z_axis << std::endl;
                    std::cout << "length(z axis) = " << length(z_axis)
                              << std::endl;
                    std::cout << "dotp(z axis, r_axis) = "
                              << dot_prod(z_axis, r_axis) << std::endl;
                }
                assert(fabs(length(r_axis) - 1e0) < 1e-12);
                assert(fabs(length(z_axis) - 1e0) < 1e-12);
            }
            ~CylindricalCoordinate(){};

            std::array<double, 3> translate(const Realvec& position);
            Realvec translate(const std::array<double, 3>& position);

        private:
            const Realvec center;
            const Realvec r_axis;
            Realvec z_axis;
    };

    std::array<double, 3> CylindricalCoordinate::translate(
                                                    const Realvec& position)
    {
        Realvec relative_position = position - center;
        double z = dot_prod(relative_position, z_axis);
        Realvec planner = relative_position - (z*z_axis);
        double r = length(planner);
        double theta = acos(dot_prod(planner, r_axis)/r);

        if(dot_prod(cross_prod(planner, r_axis), z_axis) > 0e0)
            theta *= -1e0;

        return std::array<double, 3>({{r, theta * 180 / M_PI, z}});
    }

    Realvec CylindricalCoordinate::translate(const std::array<double, 3>&
                                                                 position)
    {
        return center
             + rotation(position[1], z_axis, position[0] * r_axis)
             + position[2] * z_axis;
    }

}

#endif//COFFEE_MILL_CYLINDRICAL_COORDINATE
