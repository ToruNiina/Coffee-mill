#ifndef COFFEE_MILL_VECTOR_3_H
#define COFFEE_MILL_VECTOR_3_H
#include <array>
#include <iostream>
#include "Vector3Exp.hpp"

namespace coffeemill
{
    class RealVector3
    {
        public:
            typedef Vector value_trait;

        public:

            RealVector3()
                : values({{0e0, 0e0, 0e0}})
            {}

            RealVector3(double d)
                : values({{d, d, d}})
            {} 

            RealVector3(double x, double y, double z)
                : values({{x, y, z}})
            {} 

            template<class E, 
                     typename std::enable_if<is_VectorExpression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealVector3(const E& exp)
            {
                (*this)[0] = exp[0];
                (*this)[1] = exp[1];
                (*this)[2] = exp[2];
            } 

            template<class E, 
                     typename std::enable_if<is_VectorExpression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealVector3& operator=(const E& exp)
            {
                (*this)[0] = exp[0];
                (*this)[1] = exp[1];
                (*this)[2] = exp[2];
                return *this;
            } 

            template<class E, 
                     typename std::enable_if<is_VectorExpression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealVector3& operator+=(const E& exp)
            {
                *this = VectorAdd<RealVector3, E>(*this, exp);
                return *this;
            } 


            template<class E, 
                     typename std::enable_if<is_VectorExpression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealVector3& operator-=(const E& exp)
            {
                *this = VectorSub<RealVector3, E>(*this, exp);
                return *this;
            } 

            template<class E, 
                     typename std::enable_if<is_ScalarType<E>::value>::type*& = enabler>
            RealVector3& operator*=(const E& exp)
            {
                *this = VectorSclMul<RealVector3>(*this, exp);
                return *this;
            } 

            template<class E, 
                     typename std::enable_if<is_ScalarType<E>::value>::type*& = enabler>
            RealVector3& operator/=(const E& exp)
            {
                *this = VectorSclDiv<RealVector3>(*this, exp);
                return *this;
            } 

            double operator[](const int i) const
            {
                return values[i];
            }

            double& operator[](const int i)
            {
                return values[i];
            }


        private:
            std::array<double, 3> values;
    };

    std::ostream& operator<<(std::ostream& os, const RealVector3& vec)
    {
        os << vec[0] << " " << vec[1] << " " << vec[2];
        return os;
    }
}

#endif//COFFEE_MILL_VECTOR_H
