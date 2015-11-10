#ifndef ARABICA_REALVEC_H
#define ARABICA_REALVEC_H
#include <array>
#include <iostream>
#include <iomanip>
#include "VectorExTen.hpp"

namespace arabica
{
    class Realvec
    {
        public:

            std::array<double, 3> values;

        public:

            Realvec()
                : values({0e0, 0e0, 0e0})
            {
                ;
            }

            Realvec(double _x, double _y, double _z)
                : values({_x, _y, _z})
            {
                ;
            }

            Realvec(const Realvec& v)
                : values(v.values)
            {
                ;
            }

            template<class E>
            Realvec(const E& exp)
            {
                values[0] = exp[0];
                values[1] = exp[1];
                values[2] = exp[2];
            }

            ~Realvec(){}

            double& operator[](int i)
            {
                return values[i];
            }

            double operator[](int i) const
            {
                return values[i];
            }

            template <class E>
            Realvec& operator=(const E& rhs)
            {
                values[0] = rhs[0];
                values[1] = rhs[1];
                values[2] = rhs[2];
                return *this;
            }

            template <class E>
            Realvec& operator+=(const E& rhs)
            {
                *this = add<Realvec, E>(*this, rhs);
                return *this;
            }

            template <class E>
            Realvec& operator-=(const E& rhs)
            {
                *this = sub<Realvec, E>(*this, rhs);
                return *this;
            }

            Realvec& operator*=(const double rhs)
            {
                *this = mul<Realvec>(*this, rhs);
                return *this;
            }

            Realvec& operator/=(const double rhs)
            {
                *this = div<Realvec>(*this, rhs);
                return *this;
            }

    };

    template <class L, class R>
    add<L, R> operator+(const L& lhs, const R& rhs)
    {
        return add<L,R>(lhs, rhs);
    }

    template <class L, class R>
    sub<L, R> operator-(const L& lhs, const R& rhs)
    {
        return sub<L,R>(lhs, rhs);
    }

    //TODO: if rhs is not double type, return error.
    template <class L, class R>
    Realvec operator*(const L& lhs, const R& rhs)
    {
        return mul<L>(lhs, rhs);
    }

    //TODO: if rhs is not double type, return error.
    template <class L, class R>
    Realvec operator/(const L& lhs, const R& rhs)
    {
        return div<L>(lhs, rhs);
    }

    std::ostream& operator<<(std::ostream& os, const Realvec& rv)
    {
        os << std::setprecision(16) << rv.values[0] << " ";
        os << std::setprecision(16) << rv.values[1] << " ";
        os << std::setprecision(16) << rv.values[2];
        return os;
    }
}
#endif //ARABICA_REALVEC_H
