#define BOOST_TEST_MODULE "test_common_boundary"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "common/BoundaryCondition.hpp"

BOOST_AUTO_TEST_CASE(unlimited)
{
    using namespace std::literals::string_literals;
    using vector_type             = mill::Vector<double, 3>;
    using boundary_condition_type = mill::BoundaryCondition<vector_type>;
    using unlimited_boundary_type = mill::UnlimitedBoundary<vector_type>;
    {
        boundary_condition_type b(unlimited_boundary_type{});

        BOOST_TEST(std::isinf(b.volume()));

        vector_type r(1.0, 2.0, 3.0);
        BOOST_TEST(b.adjust_position(r)[0] == r[0]);
        BOOST_TEST(b.adjust_position(r)[1] == r[1]);
        BOOST_TEST(b.adjust_position(r)[2] == r[2]);

        vector_type dr(1.0, 2.0, 3.0);
        BOOST_TEST(b.adjust_direction(dr)[0] == dr[0]);
        BOOST_TEST(b.adjust_direction(dr)[1] == dr[1]);
        BOOST_TEST(b.adjust_direction(dr)[2] == dr[2]);
    }
}

BOOST_AUTO_TEST_CASE(periodic)
{
    using namespace std::literals::string_literals;
    using vector_type             = mill::Vector<double, 3>;
    using boundary_condition_type = mill::BoundaryCondition<vector_type>;
    using periodic_boundary_type  = mill::CuboidalPeriodicBoundary<vector_type>;
    {
        vector_type upper( 2.0,  2.0,  2.0);
        vector_type lower(-1.0, -1.0, -1.0);

        boundary_condition_type b(periodic_boundary_type(lower, upper));

        BOOST_TEST(b.volume() == 27.0);

        vector_type r(1.0, 2.0, 3.0);
        BOOST_TEST(b.adjust_position(r)[0] ==  1.0);
        BOOST_TEST(b.adjust_position(r)[1] == -1.0);
        BOOST_TEST(b.adjust_position(r)[2] ==  0.0);

        vector_type dr(1.0, 2.0, 3.0);
        BOOST_TEST(b.adjust_direction(dr)[0] ==  1.0);
        BOOST_TEST(b.adjust_direction(dr)[1] == -1.0);
        BOOST_TEST(b.adjust_direction(dr)[2] ==  0.0);
    }
}
