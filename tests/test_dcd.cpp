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
#include <random>

BOOST_AUTO_TEST_CASE(test_read_and_write)
{
    const std::string fname("test.dcd");

    std::mt19937 mt;
    std::uniform_real_distribution<float> uni(0., 1.);

    std::vector<std::vector<mill::Vector<float, 3>>> trajectory(1);
    trajectory.front().resize(100);
    for(auto iter = trajectory.front().begin();
            iter != trajectory.front().end(); ++iter)
    {
        *iter = mill::Vector<float, 3>(uni(mt), uni(mt), uni(mt));
    }

    mill::DCDData<mill::Vector<float, 3>> data;
    data.nset() = 1;
    data.istart() = 0;
    data.nstep_save() = 100;
    data.nstep() = 100;
    data.nunit() = 1;
    data.verCHARMM() = 1;
    data.nparticle() = 100;
    data.delta_t() = 0.1;
    data.signeture() = "CORD";
    data.traj() = trajectory;
    data.append_comment("for test");

    std::ofstream ofs(fname);
    if(not ofs.good()) throw std::runtime_error("test.dcd couldn't open");
    mill::DCDWriter<mill::Vector<float, 3>> writer;
    writer.write_header(ofs, data.header());
    writer.write_snapshot(ofs, data.traj().front());
    ofs.close();

    mill::DCDReader<mill::Vector<float, 3>> reader;
    mill::DCDData<mill::Vector<float, 3>> readed = reader.read(fname);

    for(std::size_t i=0; i<readed.front().size(); ++i)
    {
        BOOST_CHECK_EQUAL(readed.front().at(i)[0], trajectory.front().at(i)[0]);
        BOOST_CHECK_EQUAL(readed.front().at(i)[1], trajectory.front().at(i)[1]);
        BOOST_CHECK_EQUAL(readed.front().at(i)[2], trajectory.front().at(i)[2]);
    }
}
