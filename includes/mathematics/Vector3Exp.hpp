#ifndef COFFEE_MILL_VECTOR3_EXPRESSION_H
#define COFFEE_MILL_VECTOR3_EXPRESSION_H
#include <cmath>
#include "Expression.hpp"

namespace coffeemill
{
    template <class L, class R>
    class VectorAdd
    {
        public:
            typedef VectorExp value_trait;

            VectorAdd(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {}

            double operator[](const int i) const
            {
                return l[i] + r[i];
            }

        private:
            const L& l;
            const R& r;
    };
   
    template <class L, class R>
    class VectorSub
    {
        public:
            typedef VectorExp value_trait;

            VectorSub(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {}

            double operator[](const int i) const
            {
                return l[i] - r[i];
            }

        private:
            const L& l;
            const R& r;
    };

    template <class R>
    class VectorSclMul
    {
        public:
            typedef VectorExp value_trait;

            VectorSclMul(const double& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {}

            double operator[](const int i) const
            {
                return l * r[i];
            }

        private:
            const double l;
            const R& r;
    };

    template <class L>
    class VectorSclDiv
    {
        public:
            typedef VectorExp value_trait;

            VectorSclDiv(const L& lhs, const double& rhs)
                : l(lhs), r(rhs)
            {}

            double operator[](const int i) const
            {
                return l[i] / r;
            }

        private:
            const L& l;
            const double r;
    };

    template <class L, class R>
    class VectorCross
    {
        public:
            typedef VectorExp value_trait;

            VectorCross(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {}

            double operator[](const int i) const
            {
                return l[(i+1)%3] * r[(i+2)%3] - l[(i+2)%3] * r[(i+1)%3];
            }

        private:
            const L& l;
            const R& r;
    };

    template <class L, class R,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value&&
                  is_VectorExpression<typename R::value_trait>::value>::type*& = enabler>
    VectorAdd<L,R> operator+(const L& lhs, const R& rhs)
    {
        return VectorAdd<L,R>(lhs, rhs);
    }

    template <class L, class R,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value&&
                  is_VectorExpression<typename R::value_trait>::value>::type*& = enabler>
    VectorSub<L,R> operator-(const L& lhs, const R& rhs)
    {
        return VectorSub<L,R>(lhs, rhs);
    }

    template <class L, class R,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value&&
                  is_ScalarType<R>::value>::type*& = enabler>
    VectorSclMul<L> operator*(const L& lhs, const R& rhs)
    {
        return VectorSclMul<L>(rhs, lhs);
    }

    template <class L, class R,
              typename std::enable_if<
                  is_VectorExpression<typename R::value_trait>::value&&
                  is_ScalarType<L>::value>::type*& = enabler>
    VectorSclMul<R> operator*(const L& lhs, const R& rhs)
    {
        return VectorSclMul<R>(lhs, rhs);
    }

    template <class L, class R,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value&&
                  is_ScalarType<R>::value>::type*& = enabler>
    VectorSclDiv<L> operator/(const L& lhs, const R& rhs)
    {
        return VectorSclDiv<L>(lhs, rhs);
    }

    template <class L,
              typename std::enable_if<is_VectorExpression<
                  typename L::value_trait>::value>::type*& = enabler>
    double len_square(const L& l)
    {
        return l[0]*l[0] + l[1]*l[1] + l[2]*l[2];
    }

    template <class L,
              typename std::enable_if<is_VectorExpression<
                  typename L::value_trait>::value>::type*& = enabler>
    double length(const L& l)
    {
        return std::sqrt(len_square(l));
    }

    template <class L, class R,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value&&
                  is_VectorExpression<typename R::value_trait>::value>::type*& = enabler>
    double dot_prod(const L& lhs, const R& rhs)
    {
        return lhs[0] * rhs[0] + lhs[1] * rhs[1] + lhs[2] * rhs[2];
    }

    template <class L, class R,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value&&
                  is_VectorExpression<typename R::value_trait>::value>::type*& = enabler>
    VectorCross<L,R> cross_prod(const L& lhs, const R& rhs)
    {
        return VectorCross<L,R>(lhs, rhs);
    }

}

#endif //COFFEE_MILL_VECTOR3_EXPRESSION_H
