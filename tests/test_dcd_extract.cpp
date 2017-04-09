#define BOOST_TEST_MODULE "test_dcd_read_write"

#ifdef UNITTEST_FRAMEWORK_LIBRARY_EXIST
#include <boost/test/unit_test.hpp>
#else
#define BOOST_TEST_NO_LIB
#include <boost/test/included/unit_test.hpp>
#endif

#include <mill/data/DCDData.hpp>
#include <mill/data/DCDReader.hpp>
#include <mill/data/DCDWriter.hpp>
#include <mill/math/Vector.hpp>
#include <src/mill_dcd_extract.hpp>
#include <random>

BOOST_AUTO_TEST_CASE(test_dcd_extract)
{
    const std::string fname("test.dcd");
    /* make test.dcd */ {

        std::mt19937 mt;
        std::uniform_real_distribution<float> uni(0., 1.);

        std::vector<std::vector<mill::Vector<float, 3>>> trajectory(100);
        for(auto iter = trajectory.begin(); iter != trajectory.end(); ++iter)
        {
            *iter = std::vector<mill::Vector<float, 3>>(
                    1, mill::Vector<float, 3>(uni(mt), uni(mt), uni(mt)));
        }

        mill::DCDData<mill::Vector<float, 3>> data;
        data.nset() = 100;
        data.istart() = 0;
        data.nstep_save() = 1;
        data.nstep() = 100;
        data.nunit() = 1;
        data.verCHARMM() = 1;
        data.nparticle() = 1;
        data.delta_t() = 0.1;
        data.signeture() = "CORD";
        data.traj() = trajectory;
        data.append_comment("for test");

        mill::DCDWriter<mill::Vector<float, 3>> writer;
        writer.write(fname, data);
    }

    mill::DCDReader<mill::Vector<float, 3>> reader;
    mill::DCDData<mill::Vector<float, 3>> origin = reader.read(fname);

    char* arguments[4];
    arguments[0] = const_cast<char*>("extract");
    arguments[1] = const_cast<char*>("test.dcd");
    arguments[2] = const_cast<char*>("10");
    arguments[3] = const_cast<char*>("30");
    const int result = mill::mill_dcd_extract<mill::Vector<float, 3>>(4, arguments);
    BOOST_CHECK_EQUAL(0, result);

    mill::DCDData<mill::Vector<float, 3>>
        extracted(reader.read(std::string("test_10to30.dcd")));

    BOOST_CHECK_EQUAL(20,                  extracted.nset());
    BOOST_CHECK_EQUAL(origin.istart(),     extracted.istart());
    BOOST_CHECK_EQUAL(origin.nstep_save(), extracted.nstep_save());
    BOOST_CHECK_EQUAL(origin.nstep(),      extracted.nstep());
    BOOST_CHECK_EQUAL(origin.nunit(),      extracted.nunit());
    BOOST_CHECK_EQUAL(origin.verCHARMM(),  extracted.verCHARMM());
    BOOST_CHECK_EQUAL(origin.delta_t(),    extracted.delta_t());
    BOOST_CHECK_EQUAL(origin.signeture(),  extracted.signeture());

    for(std::size_t i=10; i<30; ++i)
    {
        BOOST_CHECK_EQUAL(origin.at(i).front()[0], extracted.at(i-10).front()[0]);
        BOOST_CHECK_EQUAL(origin.at(i).front()[1], extracted.at(i-10).front()[1]);
        BOOST_CHECK_EQUAL(origin.at(i).front()[2], extracted.at(i-10).front()[2]);
    }
}
