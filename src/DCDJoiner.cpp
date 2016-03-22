#include "dcd/DCDReader.hpp"
#include "dcd/DCDWriter.hpp"
#include <sstream>
using namespace coffeemill;

int main(int argc, char *argv[])
{
    std::ostringstream usage;
    usage << "Usage  : ./dcdjoin <file1>.dcd ... <fileN>.dcd"
          << std::endl;
    usage << "         output file is joined.dcd" << std::endl;
    usage << "Note   : this class never minds number of particle or other values."
          << std::endl;

    if(argc < 3)
    {
        std::cerr << usage.str() << std::endl;
        return EXIT_FAILURE;
    }

    const std::string dcdfile1(argv[1]);
    if(dcdfile1.substr(dcdfile1.size()-3, 3) != "dcd")
    {
        std::cerr << "Usage : ./dcdjoin <file1>.dcd ... <fileN>.dcd"
                  << std::endl;
        return EXIT_FAILURE;
    }

    std::shared_ptr<DCDReader> dcdreader1(new DCDReader(dcdfile1));
    dcdreader1->read_file();
    std::vector<SnapShot> data(dcdreader1->get_all_data());

    for(int i=2; i < argc; ++i)
    {
        const std::string dcdfile(argv[i]);
        if(dcdfile.substr(dcdfile.size()-3, 3) != "dcd")
        {
            std::cerr << usage.str() << std::endl;
            return EXIT_FAILURE;
        }

        DCDReader dcdreader(dcdfile);
        dcdreader.read_file();
        const std::vector<SnapShot> latter_data(dcdreader.get_all_data());

        std::copy(latter_data.begin(),
                  latter_data.end(),
                  std::back_inserter(data));
    }

    DCDWriter writer("joined.dcd");
    writer.set_nset(data.size());
    writer.set_istart(dcdreader1->get_istart());
    writer.set_nstep_save(dcdreader1->get_nstep_save());
    writer.set_nstep(dcdreader1->get_nstep());
    writer.set_nunit(dcdreader1->get_nunit());
    writer.set_npart(dcdreader1->get_npart());
    writer.set_delta_t(dcdreader1->get_delta_t());

    std::vector<std::string> header;
    const std::string head("This file is generated by Coffee-mill/dcdjoiner.");
    header.push_back(head);
    writer.set_header(header);

    writer.set_all_data(data);

    writer.write_file();

    return EXIT_SUCCESS;
}
