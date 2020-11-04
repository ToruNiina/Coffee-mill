#include "mode_calc_rmsd.hpp"

#include <mill/util/file_extension.hpp>
#include <mill/util/cmdarg.hpp>
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
           "optinal args:\n"
           "    --align=(true|false) [by default, true]\n"
           "        If true (default), perform alignment by minimizing RMSD.\n"
           "        otherwise, it calculates RMSD without minimization.\n"
           "    --output=<filename>  [by default, \"mill_rmsd.dat\"]\n"
           "        output data file.\n";
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

    // remove unordered argumnets
    const auto do_align = pop_argument<bool       >(args, "align" ).value_or(true);
    const auto output   = pop_argument<std::string>(args, "output").value_or("mill_rmsd.dat");

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

    const auto refname = args.at(1);
    std::vector<vector_type> ref;
    {
        log::info("mill calc rmsd: reading ", refname, " as a reference file...");
        auto r = reader(refname);
        r.read_header();
        if(const auto first_frame = r.read_frame())
        {
            for(const auto& particle : *first_frame)
            {
                ref.push_back(particle.position());
            }
        }
        log::info("mill calc rmsd: done. reference structure has ",
                  ref.size(), " particles.");
    }

    std::ofstream ofs(output);
    ofs << "#t rmsd\n";
    if(do_align)
    {
        BestFit<double> bestfit;
        bestfit.set_reference(ref);

        std::size_t tstep = 0;
        for(const auto& frame : reader(fname))
        {
            std::vector<Vector<double, 3>> snapshot;
            snapshot.reserve(frame.size());
            for(const auto& particle : frame)
            {
                snapshot.push_back(particle.position());
            }
            ofs << tstep << ' ' << rmsd(ref, bestfit.fit(snapshot)) << '\n';
            ++tstep;
        }
    }
    else
    {
        std::size_t tstep = 0;
        for(const auto& frame : reader(fname))
        {
            std::vector<Vector<double, 3>> snapshot;
            snapshot.reserve(frame.size());
            for(const auto& particle : frame)
            {
                snapshot.push_back(particle.position());
            }
            ofs << tstep << ' ' << rmsd(ref, snapshot) << '\n';
            ++tstep;
        }
    }
    return 0;
}

}//mill
