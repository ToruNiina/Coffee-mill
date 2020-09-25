#define BOOST_TEST_MODULE "test_util_string"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include <mill/util/string.hpp>

BOOST_AUTO_TEST_CASE(remove_indent)
{
    {
        const std::string str(" test");
        const std::string ret = mill::remove_indent(str);
        BOOST_TEST(ret == "test");
    }
    {
        const std::string str("\ttest");
        const std::string ret = mill::remove_indent(str);
        BOOST_TEST(ret == "test");
    }
    {
        const std::string str("  test");
        const std::string ret = mill::remove_indent(str);
        BOOST_TEST(ret == "test");
    }
    {
        const std::string str("\t\ttest");
        const std::string ret = mill::remove_indent(str);
        BOOST_TEST(ret == "test");
    }
}

BOOST_AUTO_TEST_CASE(remove_all)
{
    {
        const std::string str("test");
        const std::string ret = mill::remove_all('t', str);
        BOOST_TEST(ret == "es");
    }
}
