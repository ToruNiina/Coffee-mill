#ifndef COFFEE_MILL_VECTOR_H
#define COFFEE_MILL_VECTOR_H
#include <array>
#include <iostream>
#include "VectorExp.hpp"

namespace coffeemill
{
    template<size_t N>
    class RealVector
    {
        public:
            typedef Vector value_trait;
            constexpr static size_t size = N;

        public:

            RealVector()
            {
                for(size_t i(0); i<size; ++i)
                    values[i] = 0e0;
            }

            RealVector(const double d)
            {
                for(size_t i(0); i<size; ++i)
                    values[i] = d;
            } 

            RealVector(const std::array<double, size>& v)
                : values(v)
            {} 

            template<class E, 
                     typename std::enable_if<is_VectorExpression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealVector(const E& exp)
            {
                (*this)[0] = exp[0];
                (*this)[1] = exp[1];
                (*this)[2] = exp[2];
            } 

            template<class E, 
                     typename std::enable_if<is_VectorExpression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealVector& operator=(const E& exp)
            {
                (*this)[0] = exp[0];
                (*this)[1] = exp[1];
                (*this)[2] = exp[2];
                return *this;
            } 

            template<class E, 
                     typename std::enable_if<is_VectorExpression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealVector& operator+=(const E& exp)
            {
                *this = VectorAdd<RealVector, E>(*this, exp);
                return *this;
            } 


            template<class E, 
                     typename std::enable_if<is_VectorExpression<
                         typename E::value_trait>::value>::type*& = enabler>
            RealVector& operator-=(const E& exp)
            {
                *this = VectorSub<RealVector, E>(*this, exp);
                return *this;
            } 

            template<class E, 
                     typename std::enable_if<is_ScalarType<E>::value>::type*& = enabler>
            RealVector& operator*=(const E& exp)
            {
                *this = VectorSclMul<RealVector>(*this, exp);
                return *this;
            } 

            template<class E, 
                     typename std::enable_if<is_ScalarType<E>::value>::type*& = enabler>
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
            std::array<double, N> values;
    };

    template<size_t N>
    std::ostream& operator<<(std::ostream& os, const RealVector<N>& vec)
    {
        for(size_t i(0); i<N; ++i)
            os << vec[i] << " ";
        return os;
    }
}

#endif//COFFEE_MILL_VECTOR_H
