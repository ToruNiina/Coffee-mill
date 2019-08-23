#define BOOST_TEST_MODULE "test_matrix"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "math/Matrix.hpp"

#include <random>
constexpr static unsigned int seed = 32479327;
constexpr static std::size_t N = 10000;

BOOST_AUTO_TEST_CASE(construct_matrix_3x3)
{
    {
        mill::Matrix<double, 3, 3> mat;
        BOOST_CHECK_EQUAL((mat(0, 0)), 0);
        BOOST_CHECK_EQUAL((mat(0, 1)), 0);
        BOOST_CHECK_EQUAL((mat(0, 2)), 0);
        BOOST_CHECK_EQUAL((mat(1, 0)), 0);
        BOOST_CHECK_EQUAL((mat(1, 1)), 0);
        BOOST_CHECK_EQUAL((mat(1, 2)), 0);
        BOOST_CHECK_EQUAL((mat(2, 0)), 0);
        BOOST_CHECK_EQUAL((mat(2, 1)), 0);
        BOOST_CHECK_EQUAL((mat(2, 2)), 0);
    }

    {
        mill::Matrix<double, 3, 3> mat(1, 2, 3, 4, 5, 6, 7, 8, 9);
        BOOST_CHECK_EQUAL((mat(0, 0)), 1);
        BOOST_CHECK_EQUAL((mat(0, 1)), 2);
        BOOST_CHECK_EQUAL((mat(0, 2)), 3);
        BOOST_CHECK_EQUAL((mat(1, 0)), 4);
        BOOST_CHECK_EQUAL((mat(1, 1)), 5);
        BOOST_CHECK_EQUAL((mat(1, 2)), 6);
        BOOST_CHECK_EQUAL((mat(2, 0)), 7);
        BOOST_CHECK_EQUAL((mat(2, 1)), 8);
        BOOST_CHECK_EQUAL((mat(2, 2)), 9);
    }

    {
        mill::Matrix<double, 3, 3> mat = {
            {1, 2, 3},
            {4, 5, 6},
            {7, 8, 9}
        };
        BOOST_CHECK_EQUAL((mat(0, 0)), 1);
        BOOST_CHECK_EQUAL((mat(0, 1)), 2);
        BOOST_CHECK_EQUAL((mat(0, 2)), 3);
        BOOST_CHECK_EQUAL((mat(1, 0)), 4);
        BOOST_CHECK_EQUAL((mat(1, 1)), 5);
        BOOST_CHECK_EQUAL((mat(1, 2)), 6);
        BOOST_CHECK_EQUAL((mat(2, 0)), 7);
        BOOST_CHECK_EQUAL((mat(2, 1)), 8);
        BOOST_CHECK_EQUAL((mat(2, 2)), 9);
    }
}


BOOST_AUTO_TEST_CASE(add_matrix_3x3)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(-1.0, 1.0);
    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        mill::Matrix<double, 3, 3> lhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                lhs(i, j) = uni(mt);

        mill::Matrix<double, 3, 3> rhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                rhs(i, j) = uni(mt);

        mill::Matrix<double, 3, 3> mat = lhs + rhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                BOOST_CHECK_EQUAL((mat(i, j)), (lhs(i, j) + rhs(i, j)));
    }
}

BOOST_AUTO_TEST_CASE(sub_matrix_3x3)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(-1.0, 1.0);
    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        mill::Matrix<double, 3, 3> lhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                lhs(i, j) = uni(mt);

        mill::Matrix<double, 3, 3> rhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                rhs(i, j) = uni(mt);

        mill::Matrix<double, 3, 3> mat = lhs - rhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                BOOST_CHECK_EQUAL((mat(i, j)), (lhs(i, j) - rhs(i, j)));
    }
}

BOOST_AUTO_TEST_CASE(scalar_mul_matrix_3x3)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(-1.0, 1.0);
    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        mill::Matrix<double, 3, 3> lhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                lhs(i, j) = uni(mt);
        const double scl = uni(mt);

        {
        mill::Matrix<double, 3, 3> mat = lhs * scl;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                BOOST_CHECK_EQUAL((mat(i, j)), (lhs(i, j) * scl));
        }

        {
        mill::Matrix<double, 3, 3> mat = scl * lhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                BOOST_CHECK_EQUAL((mat(i, j)), (scl * lhs(i, j)));
        }

    }
}

BOOST_AUTO_TEST_CASE(scalar_div_matrix_3x3)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(-1.0, 1.0);
    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        mill::Matrix<double, 3, 3> lhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                lhs(i, j) = uni(mt);
        const double scl = uni(mt);

        mill::Matrix<double, 3, 3> mat = lhs / scl;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<3; ++j)
                BOOST_CHECK_EQUAL((mat(i, j)), (lhs(i, j) / scl));
    }
}

BOOST_AUTO_TEST_CASE(multi_matrix_2x2_2x2)
{
    mill::Matrix<int, 2, 2> lhs;
    lhs(0,0) = 5;
    lhs(0,1) = 6;
    lhs(1,0) = 7;
    lhs(1,1) = 8;
    mill::Matrix<int, 2, 2> rhs;
    rhs(0,0) = 1;
    rhs(0,1) = 2;
    rhs(1,0) = 3;
    rhs(1,1) = 4;
    mill::Matrix<int, 2, 2> mat = lhs * rhs;
    BOOST_CHECK_EQUAL((mat(0,0)), 23);
    BOOST_CHECK_EQUAL((mat(0,1)), 34);
    BOOST_CHECK_EQUAL((mat(1,0)), 31);
    BOOST_CHECK_EQUAL((mat(1,1)), 46);
}

BOOST_AUTO_TEST_CASE(multi_matrix_3x4_4x2)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(-1.0, 1.0);
    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        mill::Matrix<double, 3, 4> lhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<4; ++j)
                lhs(i, j) = uni(mt);

        mill::Matrix<double, 4, 2> rhs;
        for(std::size_t i=0; i<4; ++i)
            for(std::size_t j=0; j<2; ++j)
                rhs(i, j) = uni(mt);

        mill::Matrix<double, 3, 2> mat = lhs * rhs;
        for(std::size_t i=0; i<3; ++i)
            for(std::size_t j=0; j<2; ++j)
            {
                double sum = 0.;
                for(std::size_t k=0; k<4; ++k)
                    sum += lhs(i, k) * rhs(k, j);
                BOOST_CHECK_EQUAL((mat(i, j)), sum);
            }
    }
}

