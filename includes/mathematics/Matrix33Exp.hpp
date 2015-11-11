#ifndef COFFEE_MILL_MATRIX_3x3_EXP_H
#define COFFEE_MILL_MATRIX_3x3_EXP_H
#include "Expression.hpp"

namespace coffeemill
{
    template<class L, class R>
    class Matrix33Add
    {
        public:

            typedef Matrix33Exp value_trait;

            Matrix33Add(const L& lhs, const R& rhs)
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
    class Matrix33Sub
    {
        public:

            typedef Matrix33Exp value_trait;

            Matrix33Sub(const L& lhs, const R& rhs)
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
    class Matrix33Mul
    {
        public:

            typedef Matrix33Exp value_trait;

            Matrix33Mul(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {}

            double operator()(const int i, const int j) const
            {
                return l(i,0) * r(0,j) + l(i,1) * r(1,j) + l(i,2) * r(2,j);
            }

        private:

            const L& l;
            const R& r;
    };

    template<class R>
    class Matrix33SclMul
    {
        public:

            typedef Matrix33Exp value_trait;

            Matrix33SclMul(const double& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {}

            double operator()(const int i, const int j) const
            {
                return l * r(i,j);
            }

        private:

            const double& l;
            const R& r;
    };

    template<class L>
    class Matrix33SclDiv
    {
        public:

            typedef Matrix33Exp value_trait;

            Matrix33SclDiv(const L& lhs, const double& rhs)
                : l(lhs), r(rhs)
            {}

            double operator()(const int i, const int j) const
            {
                return l(i,j) / r;
            }

        private:

            const L& l;
            const double& r;
    };

    template<class L, class R,
             typename std::enable_if<
                 is_Matrix33Expression<typename L::value_trait>::value&&
                 is_Matrix33Expression<typename R::value_trait>::value>::type*& = enabler>
    Matrix33Add<L, R> operator+(const L& lhs, const R& rhs)
    {
        return Matrix33Add<L, R>(lhs, rhs);
    }

    template<class L, class R,
             typename std::enable_if<
                 is_Matrix33Expression<typename L::value_trait>::value&&
                 is_Matrix33Expression<typename R::value_trait>::value>::type*& = enabler>
    Matrix33Sub<L, R> operator-(const L& lhs, const R& rhs)
    {
        return Matrix33Sub<L, R>(lhs, rhs);
    }

    template<class L, class R,
             typename std::enable_if<
                 is_Matrix33Expression<typename L::value_trait>::value&&
                 is_Matrix33Expression<typename R::value_trait>::value>::type*& = enabler>
    Matrix33Mul<L, R> operator*(const L& lhs, const R& rhs)
    {
        return Matrix33Mul<L, R>(lhs, rhs);
    }

//L is matrix
    template<class L, class R,
             typename std::enable_if<
                 is_Matrix33Expression<typename L::value_trait>::value&&
                 is_ScalarType<R>::value>::type*& = enabler>
    Matrix33SclMul<L> operator*(const L& lhs, const R& rhs)
    {
        return Matrix33SclMul<L>(rhs, lhs);
    }

//R is matrix
    template<class L, class R,
             typename std::enable_if<
                 is_ScalarType<L>::value&&
                 is_Matrix33Expression<typename R::value_trait>::value>::type*& = enabler>
    Matrix33SclMul<R> operator*(const L& lhs, const R& rhs)
    {
        return Matrix33SclMul<R>(lhs, rhs);
    }

    template<class L, class R,
             typename std::enable_if<
                 is_Matrix33Expression<typename L::value_trait>::value&&
                 is_ScalarType<R>::value>::type*& = enabler>
    Matrix33SclDiv<L> operator/(const L& lhs, const R& rhs)
    {
        return Matrix33SclDiv<L>(lhs, rhs);
    }
}
#endif//COFFEE_MILL_EXPRESSION_H
