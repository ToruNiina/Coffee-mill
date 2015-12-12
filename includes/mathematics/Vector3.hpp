#ifndef COFFEE_MILL_VECTOR_3_H
#define COFFEE_MILL_VECTOR_3_H
#include "Vector.hpp"

namespace coffeemill
{
    template <>
    class RealVector<3>
    {
        public:
            typedef Vector value_trait;
            constexpr static size_t size = 3;

        public:

            RealVector()
                : values({{0e0, 0e0, 0e0}})
            {}

            RealVector(double d)
                : values({{d, d, d}})
            {} 

            RealVector(double x, double y, double z)
                : values({{x, y, z}})
            {} 

            RealVector(const std::array<double, 3>& array)
                : values(array)
            {} 

            template<class E, 
                     typename std::enable_if<
                         is_VectorExpression<typename E::value_trait>::value&&
                         is_SameSize<E::size, 3>::value
                         >::type*& = enabler>
            RealVector(const E& exp)
            {
                (*this)[0] = exp[0];
                (*this)[1] = exp[1];
                (*this)[2] = exp[2];
            } 

            template<class E, 
                     typename std::enable_if<
                         is_VectorExpression<typename E::value_trait>::value&&
                         is_SameSize<E::size, 3>::value
                         >::type*& = enabler>
            RealVector& operator=(const E& exp)
            {
                (*this)[0] = exp[0];
                (*this)[1] = exp[1];
                (*this)[2] = exp[2];
                return *this;
            } 

            template<class E, 
                     typename std::enable_if<
                         is_VectorExpression<typename E::value_trait>::value&&
                         is_SameSize<E::size, 3>::value
                         >::type*& = enabler>
            RealVector& operator+=(const E& exp)
            {
                *this = VectorAdd<RealVector, E>(*this, exp);
                return *this;
            } 


            template<class E, 
                     typename std::enable_if<
                         is_VectorExpression<typename E::value_trait>::value&&
                         is_SameSize<E::size, 3>::value
                         >::type*& = enabler>
            RealVector& operator-=(const E& exp)
            {
                *this = VectorSub<RealVector, E>(*this, exp);
                return *this;
            } 

            template<class E, typename std::enable_if<
                         is_ScalarType<E>::value>::type*& = enabler>
            RealVector& operator*=(const E& exp)
            {
                *this = VectorSclMul<RealVector>(*this, exp);
                return *this;
            } 

            template<class E, typename std::enable_if<
                         is_ScalarType<E>::value>::type*& = enabler>
            RealVector& operator/=(const E& exp)
            {
                *this = VectorSclDiv<RealVector>(*this, exp);
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

    typedef RealVector<3> Realvec;
}

#endif//COFFEE_MILL_VECTOR_H
