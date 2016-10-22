#define BOOST_TEST_MODULE "test_eigen"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "Matrix.hpp"
#include "Vector.hpp"
#include "EigenSolver.hpp"

#include <random>
#include <iostream>
constexpr static unsigned int seed = 32479327;
constexpr static std::size_t N = 1000;

inline double
determinant(const mill::Matrix<double, 3, 3>& mat)
{
    return mat(0,0) * mat(1,1) * mat(2,2) +
           mat(1,0) * mat(2,1) * mat(0,2) +
           mat(2,0) * mat(0,1) * mat(1,2) - 
           mat(0,0) * mat(2,1) * mat(1,2) - 
           mat(2,0) * mat(1,1) * mat(0,2) - 
           mat(1,0) * mat(0,1) * mat(2,2);
}

BOOST_AUTO_TEST_CASE(eigenvalues)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(0., 1.0);
    mill::JacobiEigenSolver solver;
    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        mill::Matrix<double, 3, 3> mat;
        double det = 0.;
        while(det == 0.)
        {
            for(std::size_t i=0; i<3; ++i)
                for(std::size_t j=i; j<3; ++j)
                    if(i==j)
                        mat(i,j) = uni(mt);
                    else
                        mat(i,j) = mat(j,i) = uni(mt);
            det = determinant(mat);
        }

        auto ev = solver.solve(mat);

        for(std::size_t i=0; i<3; ++i)
        {
            mill::Matrix<double, 3, 3> M = mat;

            for(std::size_t j=0; j<3; ++j)
                M(j, j) -= ev.at(i).first;
            
            const mill::Vector<double, 3> vec = M * ev.at(i).second;
            BOOST_CHECK_SMALL(vec[0], 1e-7);
            BOOST_CHECK_SMALL(vec[1], 1e-7);
            BOOST_CHECK_SMALL(vec[2], 1e-7);
        }
    }
}



