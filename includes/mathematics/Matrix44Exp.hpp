#ifndef COFFEE_MILL_MATRIX_4x4_EXP_H
#define COFFEE_MILL_MATRIX_4x4_EXP_H
#include "Expression.hpp"

namespace coffeemill
{
    template<class L, class R>
    class Matrix44Add
    {
        public:

            typedef Matrix44Exp value_trait;

            Matrix44Add(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {}

            double operator()(const int i, const int j) const
            {
                return l(i,j) + r(i,j);
            }

        private:

            const L& l;
            const R& r;
    };

    template<class L, class R>
    class Matrix44Sub
    {
        public:

            typedef Matrix44Exp value_trait;

            Matrix44Sub(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {}

            double operator()(const int i, const int j) const
            {
                return l(i,j) - r(i,j);
            }

        private:

            const L& l;
            const R& r;
    };

    template<class L, class R>
    class Matrix44Mul
    {
        public:

            typedef Matrix44Exp value_trait;

            Matrix44Mul(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {}

            double operator()(const int i, const int j) const
            {
                return l(i,0) * r(0,j) + l(i,1) * r(1,j) + l(i,2) * r(2,j) + l(i,3) * r(3,j);
            }

        private:

            const L& l;
            const R& r;
    };

    template<class R>
    class Matrix44SclMul
    {
        public:

            typedef Matrix44Exp value_trait;

            Matrix44SclMul(const double& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {}

            double operator()(const int i, const int j) const
            {
                return l * r(i,j);
            }

        private:

            const double l;
            const R& r;
    };

    template<class L>
    class Matrix44SclDiv
    {
        public:

            typedef Matrix44Exp value_trait;

            Matrix44SclDiv(const L& lhs, const double& rhs)
                : l(lhs), r(rhs)
            {}

            double operator()(const int i, const int j) const
            {
                return l(i,j) / r;
            }

        private:

            const L& l;
            const double r;
    };

    template<class L, class R,
             typename std::enable_if<
                 is_Matrix44Expression<typename L::value_trait>::value&&
                 is_Matrix44Expression<typename R::value_trait>::value>::type*& = enabler>
    Matrix44Add<L, R> operator+(const L& lhs, const R& rhs)
    {
        return Matrix44Add<L, R>(lhs, rhs);
    }

    template<class L, class R,
             typename std::enable_if<
                 is_Matrix44Expression<typename L::value_trait>::value&&
                 is_Matrix44Expression<typename R::value_trait>::value>::type*& = enabler>
    Matrix44Sub<L, R> operator-(const L& lhs, const R& rhs)
    {
        return Matrix44Sub<L, R>(lhs, rhs);
    }

    template<class L, class R,
             typename std::enable_if<
                 is_Matrix44Expression<typename L::value_trait>::value&&
                 is_Matrix44Expression<typename R::value_trait>::value>::type*& = enabler>
    Matrix44Mul<L, R> operator*(const L& lhs, const R& rhs)
    {
        return Matrix44Mul<L, R>(lhs, rhs);
    }

    // L is matrix
    template<class L, class R,
             typename std::enable_if<
                 is_Matrix44Expression<typename L::value_trait>::value&&
                 is_ScalarType<R>::value>::type*& = enabler>
    Matrix44SclMul<L> operator*(const L& lhs, const R& rhs)
    {
        return Matrix44SclMul<L>(rhs, lhs);
    }

    // R is matrix
    template<class L, class R,
             typename std::enable_if<
                 is_ScalarType<L>::value&&
                 is_Matrix44Expression<typename R::value_trait>::value>::type*& = enabler>
    Matrix44SclMul<R> operator*(const L& lhs, const R& rhs)
    {
        return Matrix44SclMul<R>(lhs, rhs);
    }

    template<class L, class R,
             typename std::enable_if<
                 is_Matrix44Expression<typename L::value_trait>::value&&
                 is_ScalarType<R>::value>::type*& = enabler>
    Matrix44SclDiv<L> operator/(const L& lhs, const R& rhs)
    {
        return Matrix44SclDiv<L>(lhs, rhs);
    }
}
#endif//COFFEE_MILL_EXPRESSION_H
