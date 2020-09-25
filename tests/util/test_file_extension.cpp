#define BOOST_TEST_MODULE "test_util_file_extension"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include <mill/util/file_extension.hpp>

BOOST_AUTO_TEST_CASE(test_extension)
{
    using namespace std::literals::string_view_literals;

    BOOST_TEST(mill::extension_of("hoge.txt"sv)  == ".txt"sv);
    BOOST_TEST(mill::extension_of("piyo.dat"sv)  == ".dat"sv);
    BOOST_TEST(mill::extension_of("fuga.xlsx"sv) == ".xlsx"sv);
}
