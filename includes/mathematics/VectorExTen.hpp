#ifndef ARABICA_VECTOR_EXPTEN
#define ARABICA_VECTOR_EXPTEN

namespace arabica
{
    template<class L, class R>
    class add
    {
        private:

            const L& l;
            const R& r;

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
    };

    template<class L, class R>
    class sub
    {
        private:

            const L& l;
            const R& r;

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
    };

    template<class L>
    class mul
    {
        private:

            const L& l;
            const double r;

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
    };

    template<class L>
    class div
    {
        private:

            const L& l;
            const double r;

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
    };

    template<class L, class R>
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

    template<class L, class R>
    outer<L, R> outer_prod(const L& lhs, const R& rhs)
    {
        return outer<L, R>(lhs, rhs);
    }

    template<class L>
    double length(const L& lhs)
    {
        return (lhs[0]*lhs[0] + lhs[1]*lhs[1] + lhs[2]*lhs[2]);
    }

}//end namespace arabica

#endif //ARABICA_VECTOR_EXPTEN
