/*****************************************
 *  Ninfo Splitter                       *
 *  written by Toru Niina in Oct. 4 2015 *
 *****************************************/
#include "NinfoSplitter.hpp"
using namespace coffeemill;

// @brief> split all-in-one ninfo file into one-by-one.
int main(int argc, char *argv[])
{
    if(!(argc == 2 || argc == 4) )
    {
        std::cout << "Ninfo Splitter" << std::endl;
        std::cout << "Usage: $./split [filename].ninfo [OPTIONS]" << std::endl;
        std::cout << "Outputs: [filename]_[iunit1]_[iunit2].ninfo & "
                  << "native_info_simN.inp" << std::endl;
        std::cout << "Option: " << std::endl;
        std::cout << "-sim [integer]: "
                  << "define the number of native_info_sim in output"
                  << std::endl;
        std::cout << "                inp file. (default: 1)"
                  << std::endl;
        return EXIT_FAILURE;
    }

    std::string filename(argv[1]);
    if(filename.substr(filename.size()-6, 6) != "\x2eninfo" )
    {
        std::cout << "input filename : " << argv[1]
                  << "may not be ninfo file." << std::endl;
        return EXIT_FAILURE;
    }

    int N(1);
    if(argc == 4)
    {
        const std::string option(argv[2]);
        if(option != "--sim" && option != "-sim")
        {
            std::cout << "invalid option: " << option << std::endl;
            return EXIT_FAILURE;
        }
        N = std::stoi(std::string(argv[3]));
    }
    
    NinfoReader reader(filename);

    NinfoData data(reader.read_file());

    NinfoSplitter splitter(filename, data, N);

    std::vector<WriterSptr> writers(splitter.split());

    for(auto iter = writers.begin(); iter != writers.end(); ++iter)
    {
        (*iter)->write();
    }
    return EXIT_SUCCESS;
}

