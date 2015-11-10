#ifndef COFFEE_MILL_MATH_EXPRESSION
#define COFFEE_MILL_MATH_EXPRESSION
#include <type_traits>

namespace coffeemill
{
    struct Expression{};
    struct Vector{};

    template<typename T>
    struct is_Expression: public std::false_type
    {};

    template<>
    struct is_Expression<Expression>: public std::true_type
    {};

    template<>
    struct is_Expression<Vector>: public std::true_type
    {};

    template<typename T>
    struct is_Vector: public std::false_type
    {};

    template<>
    struct is_Vector<Vector>: public std::true_type
    {};

    template<typename T>
    struct is_Scalar: public std::false_type
    {};

    template<>
    struct is_Scalar<double>: public std::true_type
    {};

    extern void* enabler;

    template<class L, class R>
    class add
    {
        public:

            typedef Expression value_trait;

        public:

            add(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {
                ;
            }

            double operator[](int i) const 
            {
                return l[i] + r[i];
            }

        private:

            const L& l;
            const R& r;
    };

    template<class L, class R>
    class sub
    {
        public:
            
            typedef Expression value_trait;

        public:

            sub(const L& lhs, const R& rhs)
                : l(lhs), r(rhs)
            {
                ;
            }

            double operator[](int i) const
            {
                return l[i] - r[i];
            }

        private:

            const L& l;
            const R& r;
    };

    template<class L>
    class mul
    {
        public:

            typedef Expression value_trait;

        public:

            mul(const L& lhs, const double rhs)
                : l(lhs), r(rhs)
            {
                ;
            }

            double operator[](int i) const
            {
                return l[i]*r;
            }

        private:

            const L& l;
            const double r;
    };

    template<class L>
    class div
    {
        public:

            typedef Expression value_trait;

        public:

            div(const L& lhs, const double rhs)
                : l(lhs), r(rhs)
            {
                ;
            }

            double operator[](int i) const
            {
                return l[i]/r;
            }

        private:

            const L& l;
            const double r;
    };

    template<class L, class R,
             typename std::enable_if<is_Expression<typename L::value_trait>::value>::type*& = enabler,
             typename std::enable_if<is_Expression<typename R::value_trait>::value>::type*& = enabler>
    double inner_prod(const L& lhs, const R& rhs)
    {
        return (lhs[0]*rhs[0] + lhs[1]*rhs[1] + lhs[2]*rhs[2]);
    }

    template<class L, class R>
    class outer
    {
        private:
            const L& l;
            const R& r;

        public:

            outer(const L& lhs, const R& rhs)
                :l(lhs), r(rhs)
            {
                ;
            }

            double operator[](int i) const
            {
                return l[(i+1)%3] * r[(i+2)%3] - l[(i+2)%3] * r[(i+1)%3];
            }
    };

    template<class L, class R, typename std::enable_if<is_Expression<typename L::value_trait>::value>::type*& = enabler,
             typename std::enable_if<is_Expression<typename R::value_trait>::value>::type*& = enabler>
    outer<L, R> outer_prod(const L& lhs, const R& rhs)
    {
        return outer<L, R>(lhs, rhs);
    }

    template<class L, typename std::enable_if<is_Expression<typename L::value_trait>::value>::type*& = enabler>
    double length(const L& lhs)
    {
        return (lhs[0]*lhs[0] + lhs[1]*lhs[1] + lhs[2]*lhs[2]);
    }

}
#endif //COFFEE_MILL_MATH_EXPRESSION
