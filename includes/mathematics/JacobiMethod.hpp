#ifndef COFFEE_MILL_JACOBI_METHOD_H
#define COFFEE_MILL_JACOBI_METHOD_H
#include <utility>
#include <algorithm>
#include <cmath>
#include "Matrix.hpp"
#include "Vector.hpp"

namespace coffeemill
{
    template <size_t S>
    class JacobiSolver
    {
        public:

            JacobiSolver()
                : solved(false)
            {}

            template<class M, typename std::enable_if<
                is_MatrixExpression<typename M::value_trait>::value&&
                is_SameSize<M::row, S>::value&&
                is_SameSize<M::col, S>::value
                >::type*& = enabler>
            JacobiSolver(const M& m)
                :solved(false), matrix(m)
            {
                solve();
            }

            ~JacobiSolver(){}

            template<class M, typename std::enable_if<
                is_MatrixExpression<typename M::value_trait>::value&&
                is_SameSize<M::row, S>::value&&
                is_SameSize<M::col, S>::value
                >::type*& = enabler>
            void set_matrix(const M& m)
            {
                solved = false;
                matrix = m;
            }

            double get_eigenvalue(const int i)
            {
                if(!solved) solve();
                return eigenvalue[i];
            }

            RealVector<S> get_eigenvec(const int i)
            {
                if(!solved) solve();
                return eigenvector[i];
            }

            std::pair<double, RealVector<S> > get_eigenpair(const int i)
            {
                if(!solved) solve();
                return std::make_pair(eigenvalue[i], eigenvector[i]);
            }

            std::pair<double, RealVector<S> > get_maxeigenpair();
            std::pair<double, RealVector<S> > get_mineigenpair();

            void solve();

        private:

            bool is_symmetric(const RealMatrix<S,S>& m) const;
            std::pair<int, int>
                get_maxindex(const RealMatrix<S,S>& m) const;
            double get_max_relative_tolerance(const RealMatrix<S,S>& ta,
                                              const RealMatrix<S,S>& tm) const;

        private:

            bool solved;
            static const double ABS_TOLERANCE;
            static const double REL_TOLERANCE;
            static const int MAX_LOOP;
            std::array<double, S> eigenvalue;
            std::array<RealVector<S>, S> eigenvector;
            RealMatrix<S,S> matrix;
    };

    template<size_t S>
    const int JacobiSolver<S>::MAX_LOOP = 10000;
    template<size_t S>
    const double JacobiSolver<S>::ABS_TOLERANCE = 1e-10;
    template<size_t S>
    const double JacobiSolver<S>::REL_TOLERANCE = 1e-12;

    template<size_t S>
    void JacobiSolver<S>::solve()
    {
        if(!is_symmetric(matrix))
        {
            std::cout << "Error  : "
                      << "cannot solve asymmetric matrix using Jacobi method"
                      << std::endl;
            return;
        }

        RealMatrix<S,S> target(matrix);
        RealMatrix<S,S> Ps(1e0);

        int num_Jacobi_loop(0);
        for(; num_Jacobi_loop < MAX_LOOP; ++num_Jacobi_loop)
        {
            if(!is_symmetric(target))
            {
                std::cout << "Error  : "
                          << "target is not symmetric : "
                          << "loop " << num_Jacobi_loop << "times"
                          << std::endl;
                std::cout << target << std::endl;
                throw std::invalid_argument("asymmetric");
            }

            std::pair<int, int> index(get_maxindex(target));

            if(fabs(target(index.first, index.second)) < ABS_TOLERANCE)
                break;

            RealMatrix<S,S> Ppri(1e0);

            double alpha
                = (target(index.first, index.first) -
                   target(index.second, index.second)) / 2e0;
            double beta
                = -1e0 * target(index.first, index.second);
            double gamma
                = fabs(alpha) / sqrt(alpha * alpha + beta * beta);

            double sign(1e0);
            if(alpha * beta < 0e0) sign *= -1e0;

            double cos = sqrt((1e0 + gamma) / 2e0);
            double sin = sqrt((1e0 - gamma) / 2e0) * sign;

            Ppri(index.first, index.first)   = cos;
            Ppri(index.first, index.second)  = sin;
            Ppri(index.second, index.first) = -sin;
            Ppri(index.second, index.second) = cos;

            RealMatrix<S,S> temp(transpose(Ppri) * target * Ppri);
            temp(index.first, index.second) = 0e0;// typically
            temp(index.second, index.first) = 0e0;// 1e-15 * other element

            if(get_max_relative_tolerance(target, temp) < REL_TOLERANCE)
                break;

            target = temp;

            temp = Ps * Ppri;
            Ps = temp;
        }
//         std::cout << num_Jacobi_loop
//                   << " times" << std::endl;
        if(num_Jacobi_loop == MAX_LOOP)
        {
            std::cout << "Warning: Cannot solve on absolute tolerance"
                      << ABS_TOLERANCE << ", relative tolerance "
                      << REL_TOLERANCE << " in loop " << MAX_LOOP << "times"
                      << std::endl;
            std::cout << "P^-1AP = " << std::endl;
            std::cout << target << std::endl;
            std::cout << "P = " << std::endl;
            std::cout << Ps << std::endl;
            throw std::invalid_argument("cannot solve");
        }

//         std::cout << "Ps" << std::endl;
//         std::cout << Ps << std::endl;
//         std::cout << std::endl;
//         std::cout << "target" << std::endl;
//         std::cout << target << std::endl;

        for(size_t i(0); i<S; ++i)
        {
            eigenvalue[i] = target(i,i);
            std::array<double, S> evec;
            for(size_t j(0); j<S; ++j)
                evec[j] = Ps(j,i);

            eigenvector[i] = RealVector<S>(evec);
//                 = std::array<double, S>({{Ps(0,i),Ps(1,i),Ps(2,i),Ps(3,i)}});
        }

        solved = true;
        return;
    }

    template<size_t S>
    std::pair<double, RealVector<S> >
        JacobiSolver<S>::get_maxeigenpair()
    {
        double max(eigenvalue[0]);
        size_t index(0);
        for(size_t i(1); i<S; ++i)
        {
            if(max < eigenvalue[i])
            {
                max = eigenvalue[i];
                index = i;
            }
        }
        return std::make_pair(eigenvalue[index], eigenvector[index]);
    }

    template<size_t S>
    std::pair<double, RealVector<S> >
        JacobiSolver<S>::get_mineigenpair()
    {
        double min(eigenvalue[0]);
        size_t index(0);
        for(size_t i(1); i<S; ++i)
        {
            if(min > eigenvalue[i])
            {
                min = eigenvalue[i];
                index = i;
            }
        }
        return std::make_pair(eigenvalue[index], eigenvector[index]);
    }


    template<size_t S>
    std::pair<int, int>
        JacobiSolver<S>::get_maxindex(const RealMatrix<S,S>& m) const
    {
        double max = fabs(m(0,1));
        std::pair<int, int> index = std::make_pair(0,1);

        for(size_t i(0); i<S-1; ++i)
        {
            for(size_t j(i+1); j<S; ++j)
            {
                if(max < fabs(m(i,j)))
                {
                    max = fabs(m(i,j));
                    index = std::make_pair(static_cast<int>(i),
                                           static_cast<int>(j));
                }
            }
        }

//         std::cout << max << std::endl;
//         std::cout << index.first << ", " << index.second << std::endl;
//         std::cout << std::endl;
        return index;
    }

    template<size_t S>
    double JacobiSolver<S>::get_max_relative_tolerance
        (const RealMatrix<S,S>& ta,  const RealMatrix<S,S>& tm) const
    {
        double max_reltol(0e0);
        for(size_t i(0); i<S; ++i)
        {
            double temp(fabs(ta(i,i) - tm(i,i)));
            if(temp > max_reltol) max_reltol = temp;
        }
        return max_reltol;
    }

    template<size_t S>
    bool JacobiSolver<S>::is_symmetric(const RealMatrix<S,S>& m) const
    {
        double TOL_EQUAL(1e-10);
        for(size_t i(0); i<S-1; ++i)
        {
            for(size_t j(i+1); j<S; ++j)
            {
                if(fabs(m(i,j) - m(j,i)) > TOL_EQUAL) return false;
            }
        }
        return true;
    }

}

#endif //COFFEE_MILL_JACOBI_METHOD_H
