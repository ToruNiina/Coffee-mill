#define BOOST_TEST_MODULE "test_util_range"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include "util/range.hpp"

BOOST_AUTO_TEST_CASE(test_enumerate)
{
    std::vector<std::string> v{"foo", "bar", "baz"};

    std::size_t ref_idx = 0;
    for(const auto& [idx, val] : mill::enumerate(v))
    {
        BOOST_TEST(idx == ref_idx);
        BOOST_TEST(val == v.at(ref_idx));
        ++ref_idx;
    }

    ref_idx = 0;
    for(const auto [idx, val] : mill::enumerate(v))
    {
        BOOST_TEST(idx == ref_idx);
        BOOST_TEST(val == v.at(ref_idx));
        ++ref_idx;
    }

    ref_idx = 0;
    for(auto&& [idx, val] : mill::enumerate(v))
    {
        BOOST_TEST(idx == ref_idx);
        BOOST_TEST(val == v.at(ref_idx));
        ++ref_idx;
    }

}

BOOST_AUTO_TEST_CASE(test_enumerate_sort)
{
    std::vector<std::string> v{"foo", "bar", "baz"};

    std::vector<std::pair<std::size_t, std::string>> sorted(3);

    const auto enm = mill::enumerate(v);
    std::partial_sort_copy(enm.begin(), enm.end(), sorted.begin(), sorted.end());

    BOOST_TEST(std::is_sorted(sorted.begin(), sorted.end()));
}
