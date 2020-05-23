#define BOOST_TEST_MODULE "test_common_attribute"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "common/Attribute.hpp"
#include "math/Vector.hpp"

BOOST_AUTO_TEST_CASE(construct)
{
    using namespace std::literals::string_literals;
    using vector_type    = mill::Vector<double, 3>;
    using attribute_type = mill::Attribute;
    {
        const attribute_type attr;
        BOOST_TEST(attr.is_empty());
        BOOST_TEST(attr.empty());
    }
    {
        const attribute_type attr(true);
        BOOST_TEST(!attr.empty());
        BOOST_TEST(attr.is_boolean());
        BOOST_TEST(attr.as_boolean() == true);
    }
    {
        const attribute_type attr(42);
        BOOST_TEST(!attr.empty());
        BOOST_TEST(attr.is_integer());
        BOOST_TEST(attr.as_integer() == 42);
    }
    {
        const attribute_type attr(3.14);
        BOOST_TEST(!attr.empty());
        BOOST_TEST(attr.is_floating());
        BOOST_TEST(attr.as_floating() == 3.14);
    }
    {
        const attribute_type attr("foo"s);
        BOOST_TEST(!attr.empty());
        BOOST_TEST(attr.is_string());
        BOOST_TEST(attr.as_string() == "foo"s);
    }
    {
        const attribute_type attr(vector_type(1.0, 2.0, 3.0));
        BOOST_TEST(!attr.empty());
        BOOST_TEST(attr.is_vector());
        BOOST_TEST(attr.as_vector()[0] == 1.0);
        BOOST_TEST(attr.as_vector()[1] == 2.0);
        BOOST_TEST(attr.as_vector()[2] == 3.0);
    }
    {
        const attribute_type attr(std::vector<attribute_type>{
                attribute_type(42),
                attribute_type(3.14),
                attribute_type("foobar"s),
            });
        BOOST_TEST(!attr.empty());
        BOOST_TEST(attr.is_array());
        BOOST_TEST(attr.as_array().at(0).as_integer()  == 42);
        BOOST_TEST(attr.as_array().at(1).as_floating() == 3.14);
        BOOST_TEST(attr.as_array().at(2).as_string()   == "foobar"s);
    }
    {
        const attribute_type attr{
                attribute_type(42),
                attribute_type(3.14),
                attribute_type("foobar"s),
            };
        BOOST_TEST(!attr.empty());
        BOOST_TEST(attr.is_array());
        BOOST_TEST(attr.as_array().at(0).as_integer()  == 42);
        BOOST_TEST(attr.as_array().at(1).as_floating() == 3.14);
        BOOST_TEST(attr.as_array().at(2).as_string()   == "foobar"s);
    }

}
