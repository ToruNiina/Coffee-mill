#ifndef COFFEE_MILL_JACOBI_4x4_H
#define COFFEE_MILL_JACOBI_4x4_H
#include <utility>
#include <algorithm>
#include <cmath>
#include "Matrix44.hpp"

namespace coffeemill
{
    class Jacobi44Solver
    {
        public:

            Jacobi44Solver()
                : solved(false)
            {}

            template<class M, typename std::enable_if<
                is_Matrix44Expression<typename M::value_trait>::value>::type*&
                = enabler>
            Jacobi44Solver(const M& m)
                :solved(false), matrix(m)
            {
                solve();
            }

            ~Jacobi44Solver(){}

            template<class M, typename std::enable_if<
                is_Matrix44Expression<typename M::value_trait>::value>::type*&
                = enabler>
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

            std::array<double, 4> get_eigenvec(const int i)
            {
                if(!solved) solve();
                return eigenvector[i];
            }

            std::pair<double, std::array<double, 4> > get_eigenpair(const int i)
            {
                if(!solved) solve();
                return std::make_pair(eigenvalue[i], eigenvector[i]);
            }

            std::pair<double, std::array<double, 4> >
                get_maxeigenpair(const int i);
            std::pair<double, std::array<double, 4> >
                get_mineigenpair(const int i);

            void solve();

        private:

            bool is_symmetric(const RealMatrix44& m) const;
            std::pair<int, int>
                get_maxindex(const RealMatrix44& m) const;
            double get_max_relative_tolerance(const RealMatrix44& ta,
                                              const RealMatrix44& tm) const;

        private:

            bool solved;
            static const double ABS_TOLERANCE;
            static const double REL_TOLERANCE;
            static const int MAX_LOOP;
            std::array<double, 4> eigenvalue;
            std::array<std::array<double, 4>,4> eigenvector;
            RealMatrix44 matrix;
    };

    const int Jacobi44Solver::MAX_LOOP = 10000;
    const double Jacobi44Solver::ABS_TOLERANCE = 1e-8;
    const double Jacobi44Solver::REL_TOLERANCE = 1e-14;

    void Jacobi44Solver::solve()
    {
        if(!is_symmetric(matrix))
        {
            std::cout << "Error  : "
                      << "cannot solve asymmetric matrix using Jacobi method"
                      << std::endl;
            return;
        }

        RealMatrix44 target(matrix);
        RealMatrix44 Ps(1e0);

        int num_Jacobi_loop(0);
        for(; num_Jacobi_loop < MAX_LOOP; ++num_Jacobi_loop)
        {
            if(!is_symmetric(target))
            {
                std::cout << "Error  : "
                          << "target is not symmetric" << std::endl;
                std::cout << target << std::endl;
                throw std::invalid_argument("asymmetric");
            }

            std::pair<int, int> index(get_maxindex(target));

            if(fabs(target(index.first, index.second)) < ABS_TOLERANCE)
                break;

            RealMatrix44 Ppri(1e0);

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

            RealMatrix44 Pinv(Ppri);
            Pinv(index.first, index.second) = -sin;
            Pinv(index.second, index.first) =  sin;

            RealMatrix44 temp(Pinv * target * Ppri);

            if(get_max_relative_tolerance(target, temp) < REL_TOLERANCE)
                break;

            target = temp;

            temp = Ps * Ppri;
            Ps = temp;
        }
//         std::cout << num_Jacobi_loop
//                   << " times" << std::endl;
        if(num_Jacobi_loop == MAX_LOOP)
            throw std::invalid_argument("cannot solve");

//         std::cout << "Ps" << std::endl;
//         std::cout << Ps << std::endl;
//         std::cout << std::endl;
//         std::cout << "target" << std::endl;
//         std::cout << target << std::endl;

        for(int i(0); i<4; ++i)
        {
            eigenvalue[i] = target(i,i);
            eigenvector[i]
                = std::array<double, 4>({Ps(0,i),Ps(1,i),Ps(2,i),Ps(3,i)});
        }

        solved = true;
        return;
    }

    std::pair<double, std::array<double, 4> >
        Jacobi44Solver::get_maxeigenpair(const int i)
    {
        double max(eigenvalue[0]);
        int index(0);
        for(int i(1); i<4; ++i)
        {
            if(max < eigenvalue[i])
            {
                max = eigenvalue[i];
                index = i;
            }
        }
        return std::make_pair(eigenvalue[index], eigenvector[index]);
    }

    std::pair<double, std::array<double, 4> >
        Jacobi44Solver::get_mineigenpair(const int i)
    {
        double min(eigenvalue[0]);
        int index(0);
        for(int i(1); i<4; ++i)
        {
            if(min > eigenvalue[i])
            {
                min = eigenvalue[i];
                index = i;
            }
        }
        return std::make_pair(eigenvalue[index], eigenvector[index]);
    }


    std::pair<int, int>
        Jacobi44Solver::get_maxindex(const RealMatrix44& m) const
    {
        double max = fabs(m(0,1));
        std::pair<int, int> index = std::make_pair(0,1);

        for(int i(0); i<4; ++i)
        {
            for(int j(i+1); j<4; ++j)
            {
                if(max < fabs(m(i,j)))
                {
                    max = fabs(m(i,j));
                    index = std::make_pair(i, j);
                }
            }
        }

//         std::cout << max << std::endl;
//         std::cout << index.first << ", " << index.second << std::endl;
//         std::cout << std::endl;
        return index;
    }

    double
        Jacobi44Solver::get_max_relative_tolerance(const RealMatrix44& ta, 
                                                   const RealMatrix44& tm) const
    {
        double max_reltol(0e0);
        for(int i(0); i<4; ++i)
        {
            double temp(fabs(ta(i,i) - tm(i,i)));
            if(temp > max_reltol) max_reltol = temp;
        }
        return max_reltol;
    }

    bool Jacobi44Solver::is_symmetric(const RealMatrix44& m) const
    {
        double TOL_EQUAL(1e-10);
        if(fabs(m(0,1) - m(1,0)) > TOL_EQUAL) return false;
        if(fabs(m(0,2) - m(2,0)) > TOL_EQUAL) return false;
        if(fabs(m(0,3) - m(3,0)) > TOL_EQUAL) return false;
        if(fabs(m(1,2) - m(2,1)) > TOL_EQUAL) return false;
        if(fabs(m(1,3) - m(3,1)) > TOL_EQUAL) return false;
        if(fabs(m(2,3) - m(3,2)) > TOL_EQUAL) return false;
        return true;
    }

}

#endif //COFFEE_MILL_JACOBI_4x4_H
