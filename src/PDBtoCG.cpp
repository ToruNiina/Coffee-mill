#include "../includes/PDBtoCG.hpp"
#include "../includes/pdb/PDBReader.hpp"
using namespace coffeemill;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage  : ./pdbtocg <filename>.pdb" << std::endl;
        return 1;
    }

    std::string filename(argv[1]);
    PDBReader reader(filename);
    reader.read_file();

    std::string outputfile("CG_" + filename);
    PDBtoCG exchanger;
    exchanger.set_pdbdata(reader.get_chains());
    exchanger.convert();
    exchanger.output(outputfile);

    return 0;
}
