#ifndef COFFEE_MILL_MATRIX_4x4
#define COFFEE_MILL_MATRIX_4x4
#include <array>
#include <iostream>
#include "Matrix44Exp.hpp"

namespace coffeemill
{
    class RealMatrix44
    {
        public:

            typedef Matrix44 value_trait;

        public:

            RealMatrix44()
            {
                for(int i(0); i<4; ++i)
                    for(int j(0); j<4; ++j)
                        values[i][j] = 0e0;
            }

            // Identity matrix
            RealMatrix44(const double d)
            {
                for(int i(0); i<4; ++i)
                    for(int j(0); j<4; ++j)
                    {
                        if(i == j)
                            values[i][i] = d;
                        else
                            values[i][j] = 0e0;
                    }
            }

            template<class E,
                     typename std::enable_if<is_Matrix44Expression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix44(const E& mat)
            {
                for(int i(0); i<4; ++i)
                    for(int j(0); j<4; ++j)
                       values[i][j] = mat(i,j);
            }

            template<class E,
                     typename std::enable_if<is_Matrix44Expression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix44& operator=(const E& mat)
            {
                for(int i(0); i<4; ++i)
                    for(int j(0); j<4; ++j)
                       values[i][j] = mat(i,j);
                return *this;
            }

            template<class E,
                     typename std::enable_if<is_Matrix44Expression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix44& operator+=(const E& mat)
            {
                *this = Matrix44Add<RealMatrix44, E>(*this, mat);
                return *this;
            }

            template<class E,
                     typename std::enable_if<is_Matrix44Expression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix44& operator-=(const E& mat)
            {
                *this = Matrix44Sub<RealMatrix44, E>(*this, mat);
                return *this;
            }

            template<class E,
                     typename std::enable_if<is_Matrix44Expression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix44& operator*=(const E& mat)
            {
                *this = Matrix44Mul<RealMatrix44, E>(*this, mat);
                return *this;
            }

            template<class E,
                     typename std::enable_if<is_ScalarType<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix44& operator*=(const double rhs)
            {
                *this = Matrix44SclMul<RealMatrix44>(rhs, *this);
                return *this;
            }

            template<class E,
                     typename std::enable_if<is_ScalarType<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix44& operator/=(const double rhs)
            {
                *this = Matrix44SclDiv<RealMatrix44>(rhs, *this);
                return *this;
            }

            double operator()(const int i, const int j) const
            {
                return values[i][j];
            }

            double& operator()(const int i, const int j)
            {
                return values[i][j];
            }

        private:

            std::array<std::array<double, 4>, 4> values;
    };

    std::ostream& operator<<(std::ostream& os, const RealMatrix44& mat)
    {
        os << mat(0,0) << " " << mat(0,1) << " "
           << mat(0,2) << " " << mat(0,3) << std::endl;
        os << mat(1,0) << " " << mat(1,1) << " "
           << mat(1,2) << " " << mat(1,3) << std::endl;
        os << mat(2,0) << " " << mat(2,1) << " "
           << mat(2,2) << " " << mat(2,3) << std::endl;
        os << mat(3,0) << " " << mat(3,1) << " "
           << mat(3,2) << " " << mat(3,3);
        return os;
    }
}

#endif//COFFEE_MILL_MATRIX_4x4
