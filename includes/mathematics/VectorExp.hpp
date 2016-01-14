#ifndef COFFEE_MILL_VECTOR_EXPRESSION_H
#define COFFEE_MILL_VECTOR_EXPRESSION_H
#include <cmath>
#include "Expression.hpp"

namespace coffeemill
{
    template <class L, class R>
    class VectorAdd
    {
        public:
            typedef VectorExp value_trait;
            constexpr static size_t size = L::size;

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
            constexpr static size_t size = L::size;

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
            constexpr static size_t size = R::size;

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
            constexpr static size_t size = L::size;

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

    // 3D vector only...
    template <class L, class R>
    class VectorCross
    {
        public:
            typedef VectorExp value_trait;
            constexpr static size_t size = L::size;

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
                  is_VectorExpression<typename R::value_trait>::value&&
                  is_SameSize<L::size, R::size>::value
                  >::type*& = enabler>
    VectorAdd<L,R> operator+(const L& lhs, const R& rhs)
    {
        return VectorAdd<L,R>(lhs, rhs);
    }

    template <class L, class R,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value&&
                  is_VectorExpression<typename R::value_trait>::value&&
                  is_SameSize<L::size, R::size>::value
                  >::type*& = enabler>
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
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value
                  >::type*& = enabler>
    double len_square(const L& l)
    {
        double len(0e0);
        for(size_t i(0); i<L::size; ++i)
            len += l[i] * l[i];
        return len;
    }

    template <class L,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value
                  >::type*& = enabler>
    double length(const L& l)
    {
        return std::sqrt(len_square(l));
    }

    template <class L,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value
                  >::type*& = enabler>
    VectorSclDiv<L> normalize(const L& l)
    {
        return VectorSclDiv<L>(l, length(l));
    }

    template <class L, class R,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value&&
                  is_VectorExpression<typename R::value_trait>::value&&
                  is_SameSize<L::size, R::size>::value
                  >::type*& = enabler>
    double dot_prod(const L& lhs, const R& rhs)
    {
        double dot(0e0);
        for(size_t i(0); i<L::size; ++i)
            dot += lhs[i] * rhs[i];
        return dot;
    }

    //now, cross_product can be used for only 3D vector.
    template <class L, class R,
              typename std::enable_if<
                  is_VectorExpression<typename L::value_trait>::value&&
                  is_VectorExpression<typename R::value_trait>::value&&
                  is_SameSize<L::size, 3>::value&&
                  is_SameSize<R::size, 3>::value
                  >::type*& = enabler>
    VectorCross<L,R> cross_prod(const L& lhs, const R& rhs)
    {
        return VectorCross<L,R>(lhs, rhs);
    }

}

#endif //COFFEE_MILL_VECTOR3_EXPRESSION_H
