#ifndef COFFEE_MILL_EXPRESSION_H
#define COFFEE_MILL_EXPRESSION_H
#include <type_traits>

namespace coffeemill
{
    extern void* enabler;

    class Matrix33{};
    class Matrix33Exp{};
    class Matrix44{};
    class Matrix44Exp{};

    class Vector{};
    class VectorExp{};

    template <typename T>
    class is_Matrix33Expression:public std::false_type{};
    template <>
    class is_Matrix33Expression<Matrix33>:public std::true_type{};
    template <>
    class is_Matrix33Expression<Matrix33Exp>: public std::true_type{};

    template <typename T>
    class is_Matrix44Expression:public std::false_type{};
    template <>
    class is_Matrix44Expression<Matrix44>:public std::true_type{};
    template <>
    class is_Matrix44Expression<Matrix44Exp>: public std::true_type{};

    template <typename T>
    class is_VectorExpression:public std::false_type{};
    template <>
    class is_VectorExpression<Vector>:public std::true_type{};
    template <>
    class is_VectorExpression<VectorExp>:public std::true_type{};

    template <typename T>
    class is_ScalarType: public std::false_type{};
    template <>
    class is_ScalarType<double>: public std::true_type{};

}
#endif //COFFEE_MILL_MATRIX_EXPRESSION_H
