#ifndef COFFEE_MILL_LINEAR_ALGEBRA_3
#define COFFEE_MILL_LINEAR_ALGEBRA_3
#include "Vector3.hpp"
#include "Matrix33.hpp"

namespace coffeemill
{
    template<class M, class V>
    class MatVecMul
    {
        public:

            typedef VectorExp value_trait;

            MatVecMul(const M& lhs, const V& rhs)
                :mat(lhs), vec(rhs)
            {}

            double operator[](const int i) const
            {
                return mat(i,0) * vec[0] + mat(i,1) * vec[1] + mat(i,2) * vec[2];
            }

        private:

            const M& mat;//matrix
            const V& vec;//vector
    };

    template<class M, class V, typename std::enable_if<
             is_Matrix33Expression<typename M::value_trait>::value&&
             is_VectorExpression<typename V::value_trait>::value>::type*& = enabler>
    MatVecMul<M,V> operator*(const M& mat, const V& vec)
    {
        return MatVecMul<M,V>(mat, vec);
    }

    template<class V, class M>
    class VecMatMul
    {
        public:

            typedef VectorExp value_trait;

            VecMatMul(const V& lhs, const M& rhs)
                :vec(lhs), mat(rhs) 
            {}

            double operator[](const int i) const
            {
                return vec[0] * mat(0, i) + vec[1] * mat(1, i) + vec[2] * mat(2, i);
            }

        private:

            const V& vec;//vector
            const M& mat;//matrix
    };

    template<class V, class M, typename std::enable_if<
             is_Matrix33Expression<typename M::value_trait>::value&&
             is_VectorExpression<typename V::value_trait>::value>::type*& = enabler>
    VecMatMul<V,M> operator*(const V& vec, const M& mat)
    {
        return VecMatMul<V,M>(vec, mat);
    }

}

#endif //COFFEE_MILL_LINEAR_ALGEBRA_3
