#ifndef COFFEE_MILL_CALC_RMSD
#define COFFEE_MILL_CALC_RMSD
#include <mill/math/RMSDCalculator.hpp>
#include <mill/data/DCDReader.hpp>
#include <mill/data/PDBReader.hpp>
#include <mill/data/XYZReader.hpp>
#include <fstream>

namespace mill
{

inline const char* mode_calc_rmsd_usage() noexcept
{
    return "usage: mill calc rmsd [files...]\n"
           "    files can be a pdb|xyz|dcd file.\n";
}

//! argv = {"impose", {args...}}
template<typename vectorT>
int mode_calc_rmsd(int argument_c, const char** argument_v)
{
    if(argument_c < 3)
    {
        std::cerr << "error: mill calc rmsd: too few arguments\n";
        std::cerr << mode_calc_rmsd_usage() << std::endl;
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        std::cerr << mode_calc_rmsd_usage() << std::endl;
        return 0;
    }

    if(fname.size() < 5)
    {
        std::cerr << "error: mill calc rmsd: invalid argument : "
                  << fname << '\n';
        std::cerr << mode_calc_rmsd_usage() << std::endl;
        return 1;
    }

    std::vector<std::vector<vectorT>> traj;

    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {
        DCDReader<vectorT> reader;
        auto dcddata = reader.read(fname);
        traj = std::move(dcddata.traj());
    }
    else if(fname.substr(fname.size() - 4, 4) == ".xyz")
    {
        XYZReader<vectorT> reader(fname);
        const auto xyz_traj = reader.read();
        for(const auto& snapshot : xyz_traj)
        {
            std::vector<vectorT> ss;
            std::transform(snapshot.particles.begin(), snapshot.particles.end(),
                std::back_inserter(ss), [](const auto& np) {return np.second;});
            traj.push_back(std::move(ss));
        }
    }
    else
    {
        std::cerr << "error: mill calc rmsd: invalid argument : "
                  << fname << '\n';
        std::cerr << mode_calc_rmsd_usage() << std::endl;
        return 1;
    }

    const std::string refname(argument_v[2]);
    std::vector<vectorT> ref;

    if(refname.substr(refname.size() - 4, 4) == ".pdb")
    {
        std::cerr << "error: mill calc rmsd: sorry, pdb is now implementing!\n";
        return 1;
    }
    else if(refname.substr(refname.size() - 4, 4) == ".xyz")
    {
        XYZReader<vectorT> reader(refname);
        const auto snapshot = reader.read_frame().particles;

        std::vector<vectorT> ss;
        std::transform(snapshot.begin(), snapshot.end(), std::back_inserter(ss),
            [](const auto& np) {return np.second;});
        ref = std::move(ss);
    }
    else
    {
        std::cerr << "error: mill calc rmsd: invalid argument : "
                  << refname << '\n';
        std::cerr << mode_calc_rmsd_usage() << std::endl;
        return 1;
    }

    BestFit<typename scalar_type_of<vectorT>::type> bestfit;
    bestfit.set_reference(ref);

    std::ofstream ofs("mill_rmsd.dat");
    ofs << "#t rmsd\n";
    for(std::size_t i=0; i<traj.size(); ++i)
    {
        ofs << i << ' ' << rmsd(ref, bestfit.fit(traj[i])) << '\n';
//         ofs << i << ' ' << rmsd(ref, traj[i]) << '\n';
    }
    std::cerr << "done.\n";

    return 0;
}

}//mill
#endif /* COFFEE_MILL_CALC_RMSD */
