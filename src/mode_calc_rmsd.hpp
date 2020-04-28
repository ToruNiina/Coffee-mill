#ifndef COFFEE_MILL_CALC_RMSD
#define COFFEE_MILL_CALC_RMSD
#include <mill/math/RMSDCalculator.hpp>
#include <mill/dcd/DCDReader.hpp>
#include <mill/pdb/PDBReader.hpp>
#include <mill/xyz/XYZReader.hpp>
#include <fstream>

namespace mill
{

inline const char* mode_calc_rmsd_usage() noexcept
{
    return "usage: mill calc rmsd [traj file] [ref file]\n"
           "    files can be a pdb|xyz|dcd file.\n";
}

//! argv = {"impose", {args...}}
template<typename vectorT>
int mode_calc_rmsd(int argument_c, const char** argument_v)
{
    if(argument_c < 3)
    {
        mill::log::error("mill calc rmsd: too few arguments.\n");
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
        mill::log::error("mill calc rmsd: unknown file format: ", fname, '\n');
        std::cerr << mode_calc_rmsd_usage() << std::endl;
        return 1;
    }

    std::vector<std::vector<vectorT>> traj;

    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {
        mill::log::info("mill calc rmsd: reading ", fname, " as a DCD file...\n");
        DCDReader<vectorT> reader;
        auto dcddata = reader.read(fname);
        traj = std::move(dcddata.traj());
        mill::log::info("mill calc rmsd: done. ", fname, " has ", traj.size(),
                        " snapshots.\n");
    }
    else if(fname.substr(fname.size() - 4, 4) == ".xyz")
    {
        mill::log::info("mill calc rmsd: reading ", fname, " as a XYZ file...\n");
        XYZReader reader(fname);
        for(const auto& snapshot : reader.read())
        {
            std::vector<vectorT> ss;
            ss.reserve(snapshot.size());
            for(const auto& particle : snapshot)
            {
                ss.push_back(particle.position());
            }
            mill::log::debug("read snapshot ", snapshot.at("comment").as_string(), "\n");
            traj.push_back(std::move(ss));
        }
        mill::log::info("mill calc rmsd: done. ", fname, " has ", traj.size(),
                        " snapshots.\n");
    }
    else
    {
        mill::log::error("mill calc rmsd: unknown file format: ", fname, '\n');
        std::cerr << mode_calc_rmsd_usage() << std::endl;
        return 1;
    }

    const std::string refname(argument_v[2]);
    std::vector<vectorT> ref;

    if(refname.substr(refname.size() - 4, 4) == ".pdb")
    {
        mill::log::error("mill calc rmsd: unknown file format: ", refname, '\n');
        return 1;
    }
    else if(refname.substr(refname.size() - 4, 4) == ".xyz")
    {
        mill::log::info("mill calc rmsd: reading ", refname, " as a XYZ file...\n");
        XYZReader reader(refname);
        for(const auto& particle : reader.read_frame())
        {
            ref.push_back(particle.position());
        }
        mill::log::info("mill calc rmsd: done. reference structure has ",
                        ref.size(), " particles.\n");
    }
    else
    {
        mill::log::error("mill calc rmsd: invalid argument: ", refname, '\n');
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
