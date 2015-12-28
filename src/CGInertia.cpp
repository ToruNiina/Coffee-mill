#include "../includes/AxisInertia.hpp"
#include "../includes/pdb/CGReader.hpp"
#include "../includes/para/MassConst.hpp"
using namespace coffeemill;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cerr << "Usage: ./inertia <filename>.pdb [chain IDs]" << std::endl;
        std::cerr << "     : cg pdb only" << std::endl;
        return 1;
    }

    std::string filename(argv[1]);
    CGReader reader(filename);
    reader.read_file();

    CGMdlSptr model = reader.get_model(0);

    std::vector<CGChnSptr> chains(model->get_data());

    std::size_t particle_num(0);
    std::string chainIDs(argv[2]);
    std::transform(chainIDs.cbegin(), chainIDs.cend(), chainIDs.begin(), toupper);

    for(auto iter = chains.begin(); iter != chains.end();)
        if(chainIDs.find((*iter)->get_chainID()) == std::string::npos)
            chains.erase(iter);
        else
            ++iter;

    for(auto iter = chains.begin(); iter != chains.end(); ++iter)
        particle_num += (*iter)->size();

    std::vector<std::pair<Realvec, double>> particles(particle_num);
    int atom_index(0);
    for(auto chain_iter = chains.begin(); chain_iter != chains.end();
        ++chain_iter)
    {
        for(auto atom_iter = (*chain_iter)->begin();
            atom_iter != (*chain_iter)->end(); ++atom_iter)
        {
            std::string element = (*atom_iter)->get_beadname();
            std::size_t pos;
            while((pos = element.find_first_of(" ")) != std::string::npos)
                element.erase(pos, 1);
            Realvec position((*atom_iter)->get_coordx(),
                             (*atom_iter)->get_coordy(),
                             (*atom_iter)->get_coordz());
            particles.at(atom_index) = std::make_pair(position, 1e0);
//                     AtomMass::getInstance().get_mass_atom(element[0]));
            ++atom_index;
        }
    }

    AxisInertia inertia(particles);

    Realvec axisline1 = inertia.get_CoM() + 1e2 * inertia.get_axis(0);
    Realvec axisline2 = inertia.get_CoM() + 1e2 * inertia.get_axis(1);
    Realvec axisline3 = inertia.get_CoM() + 1e2 * inertia.get_axis(2);

    std::cout << "Center of Mass: " << inertia.get_CoM() << std::endl;
    std::cout << "Axis 1        : " << inertia.get_axis(0) << std::endl;
    std::cout << "Axis 2        : " << inertia.get_axis(1) << std::endl;
    std::cout << "Axis 3        : " << inertia.get_axis(2) << std::endl;
    std::cout << "Axis 1 line   : " << axisline1 << std::endl;
    std::cout << "Axis 2 line   : " << axisline2 << std::endl;
    std::cout << "Axis 3 line   : " << axisline3 << std::endl;

    return 0;
}
