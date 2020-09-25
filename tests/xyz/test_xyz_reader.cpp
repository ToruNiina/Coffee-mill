#define BOOST_TEST_MODULE "test_xyz_reader"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include <mill/xyz/XYZReader.hpp>

BOOST_AUTO_TEST_CASE(read_xxx_functions)
{
    // generate test file
    {
        std::ofstream out("test.xyz");
        out << "3\n";
        out << "comment1\n";
        out << "A 1.00 2.00 3.00\n";
        out << "B 4.00 5.00 6.00\n";
        out << "C 7.00 8.00 9.00\n";
        out << "3\n";
        out << "comment2\n";
        out << "A 1.50 2.50 3.50\n";
        out << "B 4.50 5.50 6.50\n";
        out << "C 7.50 8.50 9.50\n";
    }
    {
        mill::XYZReader reader("test.xyz");
        const auto attr = reader.read_header();
        BOOST_TEST(attr.empty()); // XYZ does not have any header
    }
    {
        mill::XYZReader reader("test.xyz");
        const auto traj = reader.read();
        BOOST_TEST(traj.size() == 2u);
        BOOST_TEST(traj.at(0).size() == 3u);
        BOOST_TEST(traj.at(0).at("comment").as_string() == "comment1");

        BOOST_TEST(traj.at(0).at(0).position()[0] == 1.00);
        BOOST_TEST(traj.at(0).at(0).position()[1] == 2.00);
        BOOST_TEST(traj.at(0).at(0).position()[2] == 3.00);

        BOOST_TEST(traj.at(0).at(1).position()[0] == 4.00);
        BOOST_TEST(traj.at(0).at(1).position()[1] == 5.00);
        BOOST_TEST(traj.at(0).at(1).position()[2] == 6.00);

        BOOST_TEST(traj.at(0).at(2).position()[0] == 7.00);
        BOOST_TEST(traj.at(0).at(2).position()[1] == 8.00);
        BOOST_TEST(traj.at(0).at(2).position()[2] == 9.00);

        BOOST_TEST(traj.at(1).size() == 3u);
        BOOST_TEST(traj.at(1).at("comment").as_string() == "comment2");

        BOOST_TEST(traj.at(1).at(0).position()[0] == 1.50);
        BOOST_TEST(traj.at(1).at(0).position()[1] == 2.50);
        BOOST_TEST(traj.at(1).at(0).position()[2] == 3.50);

        BOOST_TEST(traj.at(1).at(1).position()[0] == 4.50);
        BOOST_TEST(traj.at(1).at(1).position()[1] == 5.50);
        BOOST_TEST(traj.at(1).at(1).position()[2] == 6.50);

        BOOST_TEST(traj.at(1).at(2).position()[0] == 7.50);
        BOOST_TEST(traj.at(1).at(2).position()[1] == 8.50);
        BOOST_TEST(traj.at(1).at(2).position()[2] == 9.50);
    }
    {
        mill::XYZReader reader("test.xyz");
        const auto frame1 = *reader.read_frame(0);

        BOOST_TEST(frame1.at(0).position()[0] == 1.00);
        BOOST_TEST(frame1.at(0).position()[1] == 2.00);
        BOOST_TEST(frame1.at(0).position()[2] == 3.00);

        BOOST_TEST(frame1.at(1).position()[0] == 4.00);
        BOOST_TEST(frame1.at(1).position()[1] == 5.00);
        BOOST_TEST(frame1.at(1).position()[2] == 6.00);

        BOOST_TEST(frame1.at(2).position()[0] == 7.00);
        BOOST_TEST(frame1.at(2).position()[1] == 8.00);
        BOOST_TEST(frame1.at(2).position()[2] == 9.00);

        const auto frame2 = *reader.read_frame(1);

        BOOST_TEST(frame2.at(0).position()[0] == 1.50);
        BOOST_TEST(frame2.at(0).position()[1] == 2.50);
        BOOST_TEST(frame2.at(0).position()[2] == 3.50);

        BOOST_TEST(frame2.at(1).position()[0] == 4.50);
        BOOST_TEST(frame2.at(1).position()[1] == 5.50);
        BOOST_TEST(frame2.at(1).position()[2] == 6.50);

        BOOST_TEST(frame2.at(2).position()[0] == 7.50);
        BOOST_TEST(frame2.at(2).position()[1] == 8.50);
        BOOST_TEST(frame2.at(2).position()[2] == 9.50);
    }
    {
        mill::XYZReader reader("test.xyz");

        auto iter = reader.begin();
        {
            const bool valid = iter != reader.end();
            BOOST_TEST(valid);
        }
        const auto frame1 = *iter++;
        {
            const bool valid = iter != reader.end();
            BOOST_TEST(valid);
        }
        const auto frame2 = *iter++;
        {
            const bool valid = iter != reader.end();
            BOOST_TEST(not valid);
        }

        BOOST_TEST(frame1.at(0).position()[0] == 1.00);
        BOOST_TEST(frame1.at(0).position()[1] == 2.00);
        BOOST_TEST(frame1.at(0).position()[2] == 3.00);

        BOOST_TEST(frame1.at(1).position()[0] == 4.00);
        BOOST_TEST(frame1.at(1).position()[1] == 5.00);
        BOOST_TEST(frame1.at(1).position()[2] == 6.00);

        BOOST_TEST(frame1.at(2).position()[0] == 7.00);
        BOOST_TEST(frame1.at(2).position()[1] == 8.00);
        BOOST_TEST(frame1.at(2).position()[2] == 9.00);

        BOOST_TEST(frame2.at(0).position()[0] == 1.50);
        BOOST_TEST(frame2.at(0).position()[1] == 2.50);
        BOOST_TEST(frame2.at(0).position()[2] == 3.50);

        BOOST_TEST(frame2.at(1).position()[0] == 4.50);
        BOOST_TEST(frame2.at(1).position()[1] == 5.50);
        BOOST_TEST(frame2.at(1).position()[2] == 6.50);

        BOOST_TEST(frame2.at(2).position()[0] == 7.50);
        BOOST_TEST(frame2.at(2).position()[1] == 8.50);
        BOOST_TEST(frame2.at(2).position()[2] == 9.50);
    }
}
