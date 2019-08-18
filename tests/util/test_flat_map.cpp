#define BOOST_TEST_MODULE "test_util_flat_map"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "util/flat_map.hpp"

BOOST_AUTO_TEST_CASE(construct)
{
    {
        const mill::flat_map<std::string, int> fm;
        BOOST_TEST(fm.empty());
        BOOST_TEST(fm.size() == 0u);
        BOOST_TEST(not fm.contains("foo"));
        BOOST_TEST(not fm.contains("bar"));
        BOOST_TEST(fm.count("foo") == 0u);
        BOOST_TEST(fm.count("bar") == 0u);
    }
    {
        const mill::flat_map<std::string, int> fm{{"foo", 42}, {"bar", 54}};
        BOOST_TEST(not fm.empty());
        BOOST_TEST(fm.size() == 2u);
        BOOST_TEST(fm.contains("foo"));
        BOOST_TEST(fm.contains("bar"));
        BOOST_TEST(fm.count("foo") == 1u);
        BOOST_TEST(fm.count("bar") == 1u);
        BOOST_TEST(fm.at("foo") == 42);
        BOOST_TEST(fm.at("bar") == 54);
    }
    {
        mill::flat_map<std::string, int> fm;
        BOOST_TEST(fm.empty());
        BOOST_TEST(fm.size() == 0u);

        fm = {{"foo", 42}, {"bar", 54}};
        BOOST_TEST(not fm.empty());
        BOOST_TEST(fm.size() == 2u);
        BOOST_TEST(fm.contains("foo"));
        BOOST_TEST(fm.contains("bar"));
        BOOST_TEST(fm.count("foo") == 1u);
        BOOST_TEST(fm.count("bar") == 1u);
        BOOST_TEST(fm.at("foo") == 42);
        BOOST_TEST(fm.at("bar") == 54);
    }
    {
        const std::vector<std::pair<std::string, int>> list = {{"foo", 42}, {"bar", 54}};
        const mill::flat_map<std::string, int> fm(list.begin(), list.end());
        BOOST_TEST(not fm.empty());
        BOOST_TEST(fm.size() == 2u);
        BOOST_TEST(fm.contains("foo"));
        BOOST_TEST(fm.contains("bar"));
        BOOST_TEST(fm.count("foo") == 1u);
        BOOST_TEST(fm.count("bar") == 1u);
        BOOST_TEST(fm.at("foo") == 42);
        BOOST_TEST(fm.at("bar") == 54);
    }
}

BOOST_AUTO_TEST_CASE(insertion)
{
    {
        mill::flat_map<std::string, int> fm;
        BOOST_TEST(fm.empty());
        BOOST_TEST(fm.size() == 0u);

        fm["foo"] = 42;
        fm.emplace("bar", 54);
        fm.insert(std::make_pair("baz", 69));

        BOOST_TEST(not fm.empty());
        BOOST_TEST(fm.size() == 3u);
        BOOST_TEST(fm.at("foo") == 42);
        BOOST_TEST(fm.at("bar") == 54);
        BOOST_TEST(fm.at("baz") == 69);
    }
}
