#include "includes/SuperImposer.hpp"
#include "includes/dcd/DCDManager.hpp"

using namespace coffeemill;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Coffee-mill DCD SuperImposer" << std::endl;
        std::cout << "Usage: ./superimpose <filename>.dcd" << std::endl;
        std::cout << "Info : SuperImposer outputs dcd file "
                  << "containing superimposed structure" << std::endl;
        return 1;
    }

    std::string infile(argv[1]);
    DCDReader reader(infile);
    reader.read_file();

    std::vector<SnapShot> data(reader.get_all_data());
    std::vector<SnapShot> imposed(data.size());

    SuperImposer simposer;
    simposer.set_data(data.at(1), data.at(0));
    simposer.superimpose();
    imposed.at(0) = simposer.get_data2();//SuperImposer rotates data2

    for(size_t i(2); i<data.size(); ++i)
    {
        simposer.set_data_push(data.at(i));
        simposer.superimpose();
        imposed.at(i-1) = simposer.get_data2();
    }
    imposed.at(data.size()-1) = simposer.get_data1();

    std::string outfile("SupImp"+infile);
    DCDWriter writer(outfile);

    set_header_from_reader(&writer, &reader);
    writer.set_all_data(imposed);

    writer.write_file();

    return 0;
}
//(c)Toru Niina 2015 all rights reserved.
