#include "mode_calc_rmsd.hpp"

#include <mill/util/file_extension.hpp>
#include <mill/math/RMSDCalculator.hpp>
#include <mill/math/BestFitStructure.hpp>
#include <mill/pdb/PDBReader.hpp>
#include <mill/traj.hpp>
#include <fstream>

namespace mill
{

const char* mode_calc_rmsd_usage() noexcept
{
    return "usage: mill calc rmsd [traj file] [ref file]\n"
           "    files can be a pdb|xyz|dcd file.\n";
}

int mode_calc_rmsd(std::deque<std::string_view> args)
{
    using vector_type = Vector<double, 3>;
    if(args.empty())
    {
        log::error("mill calc rmsd: too few arguments.");
        log::error(mode_calc_rmsd_usage());
        return 1;
    }

    const auto fname = args.front();
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

    if(extension_of(fname) == ".dcd" || extension_of(fname) == ".xyz")
    {
        log::info("mill calc rmsd: reading ", fname);
        for(const auto& snapshot : read(fname))
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
    else
    {
        log::error("mill calc rmsd: unknown file format: ", fname);
        log::error(mode_calc_rmsd_usage());
        return 1;
    }

    const auto refname = args.at(1);
    std::vector<vector_type> ref;

    if(extension_of(refname) == ".pdb")
    {
        log::error("mill calc rmsd: unknown file format: ", refname);
        return 1;
    }
    else if(extension_of(refname) == ".xyz")
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
