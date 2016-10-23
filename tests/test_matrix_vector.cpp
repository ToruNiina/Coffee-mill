#define BOOST_TEST_MODULE "test_matrix_vector"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "math/Vector.hpp"

#include <random>
constexpr static unsigned int seed = 32479327;
constexpr static std::size_t N = 10000;

BOOST_AUTO_TEST_CASE(multiple_unit_matrix_and_vector)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(-1.0, 1.0);
    const mill::Matrix<double, 3, 3> E(1., 0., 0., 0., 1., 0., 0., 0., 1.);

    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        const mill::Vector<double, 3> lhs(uni(mt), uni(mt), uni(mt));
        const mill::Vector<double, 3> same = E * lhs;

        BOOST_CHECK_EQUAL(lhs[0], same[0]);
        BOOST_CHECK_EQUAL(lhs[1], same[1]);
        BOOST_CHECK_EQUAL(lhs[2], same[2]);
    }

    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        const mill::Vector<double, 3> lhs(uni(mt), uni(mt), uni(mt));
        const mill::Matrix<double, 1, 3> same = mill::transpose(lhs) * E;
        const mill::Vector<double, 3> rhs = mill::transpose(same);

        BOOST_CHECK_EQUAL(lhs[0], rhs[0]);
        BOOST_CHECK_EQUAL(lhs[1], rhs[1]);
        BOOST_CHECK_EQUAL(lhs[2], rhs[2]);
    }
}

BOOST_AUTO_TEST_CASE(multiple_matrix_and_vector)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(-1.0, 1.0);

    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        const double coef = uni(mt);
        const mill::Matrix<double, 3, 3> M(coef, 0., 0., 0., coef, 0., 0., 0., coef);
        const mill::Vector<double, 3> lhs(uni(mt), uni(mt), uni(mt));
        const mill::Vector<double, 3> same = M * lhs;

        BOOST_CHECK_EQUAL(lhs[0] * coef, same[0]);
        BOOST_CHECK_EQUAL(lhs[1] * coef, same[1]);
        BOOST_CHECK_EQUAL(lhs[2] * coef, same[2]);
    }
}
