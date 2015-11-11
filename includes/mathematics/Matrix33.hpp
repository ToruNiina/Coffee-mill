#ifndef COFFEE_MILL_MATRIX_3x3
#define COFFEE_MILL_MATRIX_3x3
#include <array>
#include <iostream>
#include "Matrix33Exp.hpp"

namespace coffeemill
{
    class RealMatrix33
    {
        public:

            typedef Matrix33 value_trait;

        public:

            RealMatrix33()
            {
                for(int i(0); i<3; ++i)
                    for(int j(0); j<3; ++j)
                        values[i][j] = 0e0;
            }

            // Identity matrix
            RealMatrix33(const double d)
            {
                for(int i(0); i<3; ++i)
                    for(int j(0); j<3; ++j)
                    {
                        if(i == j)
                            values[i][i] = d;
                        else
                            values[i][j] = 0e0;
                    }
            }

            template<class E,
                     typename std::enable_if<is_Matrix33Expression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix33(const E& mat)
            {
                for(int i(0); i<3; ++i)
                    for(int j(0); j<3; ++j)
                       (*this)(i,j) = mat(i,j);
            }

            template<class E,
                     typename std::enable_if<is_Matrix33Expression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix33& operator=(const E& mat)
            {
                for(int i(0); i<3; ++i)
                    for(int j(0); j<3; ++j)
                       (*this)(i,j) = mat(i,j);
                return *this;
            }

            template<class E,
                     typename std::enable_if<is_Matrix33Expression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix33& operator+=(const E& mat)
            {
                *this = Matrix33Add<RealMatrix33, E>(*this, mat);
                return *this;
            }

            template<class E,
                     typename std::enable_if<is_Matrix33Expression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix33& operator-=(const E& mat)
            {
                *this = Matrix33Sub<RealMatrix33, E>(*this, mat);
                return *this;
            }

            template<class E,
                     typename std::enable_if<is_Matrix33Expression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix33& operator*=(const E& mat)
            {
                *this = Matrix33Mul<RealMatrix33, E>(*this, mat);
                return *this;
            }

            template<class E,
                     typename std::enable_if<is_ScalarType<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix33& operator*=(const double rhs)
            {
                *this = Matrix33SclMul<RealMatrix33>(rhs, *this);
                return *this;
            }

            template<class E,
                     typename std::enable_if<is_ScalarType<
                         typename E::value_trait>::value>::type*& = enabler>
            RealMatrix33& operator/=(const double rhs)
            {
                *this = Matrix33SclDiv<RealMatrix33>(rhs, *this);
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

            std::array<std::array<double, 3>, 3> values;
    };

    std::ostream& operator<<(std::ostream& os, const RealMatrix33& mat)
    {
        os << mat(0,0) << " " << mat(0,1) << " " << mat(0,2) << std::endl;
        os << mat(1,0) << " " << mat(1,1) << " " << mat(1,2) << std::endl;
        os << mat(2,0) << " " << mat(2,1) << " " << mat(2,2);
        return os;
    }
}

#endif//COFFEE_MILL_MATRIX_3x3
