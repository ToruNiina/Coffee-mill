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
int mode_calc_rmsd(int argument_c, const char** argument_v)
{
    using vector_type = Vector<double, 3>;
    if(argument_c < 3)
    {
        log::error("mill calc rmsd: too few arguments.");
        log::error(mode_calc_rmsd_usage());
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        log::info(mode_calc_rmsd_usage());
        return 0;
    }

    if(fname.size() < 5)
    {
        log::error("mill calc rmsd: unknown file format: ", fname);
        log::error(mode_calc_rmsd_usage());
        return 1;
    }

    std::vector<std::vector<vector_type>> traj;

    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {
        log::info("mill calc rmsd: reading ", fname, " as a DCD file...");
        DCDReader reader(fname);
        for(const auto& snapshot : reader)
        {
            std::vector<vector_type> ss; ss.reserve(snapshot.size());
            for(const auto& particle : snapshot)
            {
                ss.push_back(particle.position());
            }
            traj.push_back(std::move(ss));
        }
        log::info("mill calc rmsd: done. ", fname, " has ", traj.size(),
                        " snapshots.");
    }
    else if(fname.substr(fname.size() - 4, 4) == ".xyz")
    {
        log::info("mill calc rmsd: reading ", fname, " as a XYZ file...");
        XYZReader reader(fname);
        for(const auto& snapshot : reader)
        {
            std::vector<vector_type> ss;
            ss.reserve(snapshot.size());
            for(const auto& particle : snapshot)
            {
                ss.push_back(particle.position());
            }
            log::debug("read snapshot ", snapshot.at("comment").as_string());
            traj.push_back(std::move(ss));
        }
        log::info("mill calc rmsd: done. ", fname, " has ", traj.size(),
                        " snapshots.");
    }
    else
    {
        log::error("mill calc rmsd: unknown file format: ", fname);
        log::error(mode_calc_rmsd_usage());
        return 1;
    }

    const std::string refname(argument_v[2]);
    std::vector<vector_type> ref;

    if(refname.substr(refname.size() - 4, 4) == ".pdb")
    {
        log::error("mill calc rmsd: unknown file format: ", refname);
        return 1;
    }
    else if(refname.substr(refname.size() - 4, 4) == ".xyz")
    {
        log::info("mill calc rmsd: reading ", refname, " as a XYZ file...");
        XYZReader reader(refname);
        if(const auto first_frame = reader.read_frame())
        {
            for(const auto& particle : *first_frame)
            {
                ref.push_back(particle.position());
            }
        }
        log::info("mill calc rmsd: done. reference structure has ",
                        ref.size(), " particles.");
    }
    else
    {
        log::error("mill calc rmsd: invalid argument: ", refname);
        log::error(mode_calc_rmsd_usage());
        return 1;
    }

    BestFit<typename scalar_type_of<vector_type>::type> bestfit;
    bestfit.set_reference(ref);

    std::ofstream ofs("mill_rmsd.dat");
    ofs << "#t rmsd\n";
    for(std::size_t i=0; i<traj.size(); ++i)
    {
        ofs << i << ' ' << rmsd(ref, bestfit.fit(traj[i])) << '\n';
//         ofs << i << ' ' << rmsd(ref, traj[i]) << '\n';
    }
    return 0;
}

}//mill
#endif /* COFFEE_MILL_CALC_RMSD */
