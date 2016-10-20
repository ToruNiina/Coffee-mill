#define BOOST_TEST_MODULE "test_matrix"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "Vector.hpp"

#include <random>
constexpr static unsigned int seed = 32479327;
constexpr static std::size_t N = 10000;

BOOST_AUTO_TEST_CASE(add_vector_3d)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(-1.0, 1.0);
    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        const mill::Vector<double, 3> lhs(uni(mt), uni(mt), uni(mt));
        const mill::Vector<double, 3> rhs(uni(mt), uni(mt), uni(mt));

        mill::Vector<double, 3> vec = lhs + rhs;
        BOOST_CHECK_EQUAL(vec[0], lhs[0] + rhs[0]);
        BOOST_CHECK_EQUAL(vec[1], lhs[1] + rhs[1]);
        BOOST_CHECK_EQUAL(vec[2], lhs[2] + rhs[2]);
    }
}

BOOST_AUTO_TEST_CASE(sub_vector_3d)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(-1.0, 1.0);
    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        const mill::Vector<double, 3> lhs(uni(mt), uni(mt), uni(mt));
        const mill::Vector<double, 3> rhs(uni(mt), uni(mt), uni(mt));

        mill::Vector<double, 3> vec = lhs - rhs;
        BOOST_CHECK_EQUAL(vec[0], lhs[0] - rhs[0]);
        BOOST_CHECK_EQUAL(vec[1], lhs[1] - rhs[1]);
        BOOST_CHECK_EQUAL(vec[2], lhs[2] - rhs[2]);
    }
}

BOOST_AUTO_TEST_CASE(mul_vector_3d)
{
    std::mt19937 mt(seed);
    std::uniform_real_distribution<double> uni(-1.0, 1.0);
    for(std::size_t test_times=0; test_times<N; ++test_times)
    {
        const mill::Vector<double, 3> lhs(uni(mt), uni(mt), uni(mt));
        const double scl = uni(mt);

        mill::Vector<double, 3> vec = lhs * scl;
        BOOST_CHECK_EQUAL(vec[0], lhs[0] * scl);
        BOOST_CHECK_EQUAL(vec[1], lhs[1] * scl);
        BOOST_CHECK_EQUAL(vec[2], lhs[2] * scl);
    }
}
