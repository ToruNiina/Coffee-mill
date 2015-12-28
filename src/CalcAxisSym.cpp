#include "../includes/AxisSymmetry.hpp"
#include "../includes/pdb/CGReader.hpp"
#include "../includes/para/MassConst.hpp"
using namespace coffeemill;

int main(int argc, char *argv[])
{
    if(argc != 3 && argc != 2)
    {
        std::cerr << "Usage  : ./symaxis <filename>.pdb [OPTION]" << std::endl;
        std::cerr << "Option : DNA, PRO or ALL(default ALL)" << std::endl;
        return 1;
    }

    AxisSymmetryNuc::USE_CHAIN condition;
    if(argc == 2)
    {
        condition = AxisSymmetryNuc::USE_CHAIN::ALL;
    }
    else
    {
        std::string opt(argv[2]);
        std::transform(opt.cbegin(), opt.cend(), opt.begin(), toupper);
        if(opt[0] == 'D')
            condition = AxisSymmetryNuc::USE_CHAIN::DNA;
        else if(opt[0] == 'P' || opt[0] == 'H')
            condition = AxisSymmetryNuc::USE_CHAIN::HISTONE;
        else if(opt[0] == 'A')
            condition = AxisSymmetryNuc::USE_CHAIN::ALL;
        else
            throw std::invalid_argument("invalid option : " + opt);
    }

    std::string filename(argv[1]);
    CGReader reader(filename);
    reader.read_file();

    CGMdlSptr model = reader.get_model(0);

    std::vector<CGChnSptr> chains(model->get_data());
    
    std::size_t index(0);
    for(auto iter = chains.begin(); iter != chains.end(); ++iter)
        index += (*iter)->size();
    if(index != 1860)
        throw std::invalid_argument("this program is for 1KX5 CGPDB only");

    std::vector<Realvec> particles(index);
    std::size_t atom_index(0);
    for(auto chain_iter = chains.begin(); chain_iter != chains.end();
        ++chain_iter)
    {
//         std::cout << "chain ID : " << (*chain_iter)->get_chainID() << std::endl;
        for(auto atom_iter = (*chain_iter)->begin();
            atom_iter != (*chain_iter)->end(); ++atom_iter)
        {
            Realvec position((*atom_iter)->get_coordx(),
                             (*atom_iter)->get_coordy(),
                             (*atom_iter)->get_coordz());
            particles.at(atom_index) = position;
            ++atom_index;
        }
    }
//     std::cout << "chain reading end" << std::endl;

    AxisSymmetryNuc symaxis(particles);
    Realvec CoM  = symaxis.get_CoM();
    Realvec axis = symaxis.get_axis(condition);

    std::cout << "Center of Mass : " << CoM << std::endl;
    std::cout << "Axis           : " << axis << std::endl;

    return 0;
}
