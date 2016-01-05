#include "../includes/AxisSymmetry.hpp"
#include "../includes/AxisInertia.hpp"
#include "../includes/CylindoricalCoordinate.hpp"
#include "../includes/dcd/DCDReader.hpp"
using namespace coffeemill;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage : ./reinterpret <filename>.dcd" << std::endl;
        std::cerr << "      : this considers the file is already superimposed"
                  << std::endl;
        std::cerr << "      : this may not work correctly generally."
                  << std::endl;
    }

    std::string filename(argv[1]);
    DCDReader dcdreader(filename);
    dcdreader.read_file();

    std::vector<SnapShot> data(dcdreader.get_all_data());

    SnapShot initial(data[0]);

    if(initial.size() != 1860)
        throw std::invalid_argument("bead size != 1860");

    // Axis of Symmetry is obtained without tails
    AxisSymmetryNuc symaxis(initial);
    AxisSymmetryNuc::USE_CHAIN condition(AxisSymmetryNuc::USE_CHAIN::ALL);
    Realvec axis_symmetry = symaxis.get_axis(condition);

    // Axis of Inertia is obtained by using DNA chain only
    SnapShot initial_DNAonly(880);
    for(std::size_t index(0); index < 880; ++index)
    {
        initial_DNAonly.at(index) = initial.at(index);
    }

    AxisInertia inertia(initial);
    std::array<double, 3> principal_moments_of_inertia = inertia.get_values();
    int max_index(0);
    double max_moment(0e0);
    for(int i(0); i < 3; ++i)
    {
        if(max_moment < principal_moments_of_inertia.at(i))
        {
            max_moment = principal_moments_of_inertia.at(i);
            max_index = i;
        }
    }
    Realvec axis_inertia = inertia.get_axis(max_index);

    unsigned int number_of_beads(0);
    Realvec center(0e0);
    for(auto iter = initial.begin(); iter != initial.end(); ++iter)
    {
        center += (*iter);
        ++number_of_beads;
    }
    center /= static_cast<double>(number_of_beads);

    // define nucleosome coordinate
    CylindoricalCoordinate cylindcoord(center, axis_symmetry, axis_inertia);
    // type alias
    using Cylindorical = std::array<double, 3>;

//     std::ofstream dyad(filename + "dyad.out");

    // prepare output
    std::string outfilename(filename + ".out");
    std::ofstream outfile(outfilename);
    outfile << "# indexes are separated by two empty lines(for gnuplot)."
            << std::endl;
    outfile << "# each index corresponds to each snapshot."
            << std::endl;
    outfile << "# base , r , theta , z" << std::endl;

    std::string proteinfilename(filename + "_protein.out");
    std::ofstream outprotein(proteinfilename);
    outprotein << "# 881-1015  : C H3" << std::endl;
    outprotein << "# 1016-1117 : D H4" << std::endl;
    outprotein << "# 1118-1245 : E H2A" << std::endl;
    outprotein << "# 1246-1370 : F H2B" << std::endl;
    outprotein << "# 1371-1505 : G H3" << std::endl;
    outprotein << "# 1506-1607 : H H4" << std::endl;
    outprotein << "# 1608-1735 : I H2A" << std::endl;
    outprotein << "# 1736-1860 : J H2B" << std::endl;
    outprotein << "# indexes are separated by two empty lines(for gnuplot)."
               << std::endl;
    outprotein << "# each index corresponds to each snapshot."
               << std::endl;
    outprotein << "# imp(AminoAcids) , r , theta , z" << std::endl;

//     unsigned int t_step = 0;
    // dcd data processing
    for(auto snapiter = data.begin(); snapiter != data.end(); ++snapiter)
    {
        for(std::size_t DNAindex(0); DNAindex < 440; ++DNAindex)
        {
            //base only
            if(DNAindex % 3 != 1) continue;
            Realvec beadFw = snapiter->at(DNAindex);
            Realvec beadRv = snapiter->at(879 - DNAindex);
            Realvec beadMd = (beadFw + beadRv) / 2e0;

            Cylindorical cycoordFw = cylindcoord.translate(beadFw);
            Cylindorical cycoordRv = cylindcoord.translate(beadRv);
            Cylindorical cycoordMd = cylindcoord.translate(beadMd);

//             if(DNAindex == 220)
//             {
//                 dyad << t_step       << " "
//                      << cycoordFw[0] << " "
//                      << cycoordFw[1] << " "
//                      << cycoordFw[2] << " "
//                      << cycoordRv[0] << " "
//                      << cycoordRv[1] << " "
//                      << cycoordRv[2] << " "
//                      << cycoordMd[0] << " "
//                      << cycoordMd[1] << " "
//                      << cycoordMd[2] << std::endl;
//             }

            unsigned int resNum = (DNAindex - 1) / 3 + 1;
            outfile << resNum       << " "
                    << cycoordFw[0] << " "
                    << cycoordFw[1] << " "
                    << cycoordFw[2] << " "
                    << cycoordRv[0] << " "
                    << cycoordRv[1] << " "
                    << cycoordRv[2] << " "
                    << cycoordMd[0] << " "
                    << cycoordMd[1] << " "
                    << cycoordMd[2]
                    << std::endl;
        }

        for(std::size_t Histoneindex(880); Histoneindex < 1860; ++Histoneindex)
        {
            Realvec beadpos = snapiter->at(Histoneindex);
            Cylindorical cycoordpos = cylindcoord.translate(beadpos);

            outprotein << Histoneindex+1 << " "
                       << cycoordpos[0]  << " "
                       << cycoordpos[1]  << " "
                       << cycoordpos[2]  << std::endl;
        }

        outfile << std::endl;
        outfile << std::endl;
        outprotein << std::endl;
        outprotein << std::endl;
//         t_step++;
    }
    outfile.close();
    outprotein.close();

    return 0;
}
