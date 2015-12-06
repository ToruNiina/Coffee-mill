/*****************************************
 *  Ninfo Splitter                       *
 *  written by Toru Niina in Oct. 4 2015 *
 *****************************************/
#include "../includes/NinfoSplitter.hpp"

using namespace coffeemill;

int main(int argc, char *argv[])
{
    if(!(argc == 2 || argc == 4) )
    {
        std::cout << "Ninfo Splitter"                                                     << std::endl;
        std::cout << "Usage: $./split [filename].ninfo [OPTIONS]"                         << std::endl;
        std::cout << "Outputs: [filename]_[iunit1]_[iunit2].ninfo & native_info_simN.inp" << std::endl;
        std::cout << "Option:                                                  "          << std::endl;
        std::cout << "-sim [const int]: define the number of native_info_sim in output"   << std::endl;
        std::cout << "                  inp file. (default: 1)"                           << std::endl;
        return 0;
    }

    std::string filename(argv[1]);
    if( filename.substr(filename.size()-6, 6) != "\x2eninfo" )
    {
        std::cout << "your input filename is : " << argv[1] << std::endl;
        std::cout << "it may not be ninfo file. Please check." << std::endl;
    }

    int N(1);
    if(argc == 4)
    {
        std::string op(argv[2]);
        if(op != "--sim" && op != "-sim")
        {
            std::cout << "invalid option" << std::endl;
            return -1;
        }
        N = std::stoi(std::string(argv[3]));
    }
    
    NinfoReader reader(filename);

    NinfoData data(reader.read_file());

    NinfoSplitter splitter(filename, data, N);

    std::vector<WriterSptr> writers(splitter.split());

    for(std::vector<WriterSptr>::iterator iter = writers.begin(); iter != writers.end(); ++iter)
    {
        (*iter)->write();
    }
    return 0;
}

