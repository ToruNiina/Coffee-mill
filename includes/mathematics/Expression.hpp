#ifndef COFFEE_MILL_EXPRESSION_H
#define COFFEE_MILL_EXPRESSION_H
#include <type_traits>
#include <cstddef>

namespace coffeemill
{
    extern void* enabler;

    class Matrix{};
    class MatrixExp{};
    class Vector{};
    class VectorExp{};

    template <typename T>
    class is_MatrixExpression:public std::false_type{};
    template <>
    class is_MatrixExpression<Matrix>:public std::true_type{};
    template <>
    class is_MatrixExpression<MatrixExp>: public std::true_type{};

    template <typename T>
    class is_VectorExpression:public std::false_type{};
    template <>
    class is_VectorExpression<Vector>:public std::true_type{};
    template <>
    class is_VectorExpression<VectorExp>:public std::true_type{};

    template <size_t LN, size_t RN>
    class is_SameSize: public std::false_type{};
    template <size_t LN>
    class is_SameSize<LN, LN>: public std::true_type{};

    template <typename T>
    class is_ScalarType: public std::false_type{};
    template <>
    class is_ScalarType<double>: public std::true_type{};

}
#endif //COFFEE_MILL_MATRIX_EXPRESSION_H
