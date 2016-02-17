#include "DCDReader.hpp"
#include "DCDWriter.hpp"
using namespace coffeemill;

int main(int argc, char *argv[])
{
    // compile with g++
    // $ g++ -std=c++11 -o dcdtest DCDtest.cpp
    // this outputs 2 files
    // FIXME: later I will write boost unit test
    if(argc != 2)
    {
        std::cerr << "Usage: ./testdcd <filename>.txt" << std::endl;
        return 1;
    }
    std::cout << "debug: reading start" << std::endl;
    std::string filename(argv[1]);
    DCDReader reader(filename);
    try
    {
        reader.read_file();
    }
    catch (std::exception& excpt)
    {
        std::cerr << "exception throwed!" << std::endl;
        std::cerr << excpt.what() << std::endl;
        return 1;
    }
    std::cout << "debug: reading end succesfully" << std::endl;

    std::cout << "debug: writing all trajectory once: start" << std::endl;
    std::string outfile("debuglog.dcd");
    DCDWriter writer(outfile);
    writer.set_istart(reader.get_istart());
    writer.set_nstep_save(reader.get_nstep_save());
    writer.set_nstep(reader.get_nstep());
    writer.set_nunit(reader.get_nunit());
    writer.set_npart(reader.get_npart());
    writer.set_delta_t(reader.get_delta_t());
    writer.set_header(reader.get_header());
    try
    {
        writer.write_file();
    }
    catch(std::exception& excpt)
    {
        std::cerr << "exception throwed!" << std::endl;
        std::cerr << excpt.what() << std::endl;
        return 1;
    }
    std::cout << "debug: writing all trajectory once: end succesfully" << std::endl;

    std::cout << "debug: writing step by step: start" << std::endl;
    std::string separate("write_separately.dcd");
    DCDWriter sepwriter(separate);
    sepwriter.set_nset(reader.get_nset());// write nset if you write step by step!
    sepwriter.set_istart(reader.get_istart());
    sepwriter.set_nstep_save(reader.get_nstep_save());
    sepwriter.set_nstep(reader.get_nstep());
    sepwriter.set_nunit(reader.get_nunit());
    sepwriter.set_npart(reader.get_npart());
    sepwriter.set_delta_t(reader.get_delta_t());
    sepwriter.set_header(reader.get_header());

    try
    {
        sepwriter.write_header();
        for(auto iter = reader.cbegin(); iter != reader.cend(); ++iter)
        {
            sepwriter.write_SnapShot(*iter);
        }
    }
    catch(std::exception& excpt)
    {
        std::cerr << "exception throwed!" << std::endl;
        std::cerr << excpt.what() << std::endl;
        return 1;
    }
    std::cout << "debug: writing step by step: end successfully" << std::endl;

    std::cout << "debug: reading the output file : start" << std::endl;
    DCDReader outreader(outfile);
    outreader.read_file();
    DCDReader sepreader(separate);
    sepreader.read_file();
    std::cout << "debug: reading the output file : end successfully" << std::endl;

    return 0;
}
