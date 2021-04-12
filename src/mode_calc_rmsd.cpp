#include "mode_calc_rmsd.hpp"

#include <mill/util/file_extension.hpp>
#include <mill/util/cmdarg.hpp>
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
           "        output data file.\n"
           "    --only=<begin>:<end>\n"
           "    --ref-only=<begin>:<end>\n"
           "        use [begin, end) region only.\n";
}

std::optional<std::pair<std::size_t, std::size_t>>
pop_range(std::deque<std::string_view>& args, std::string_view name)
{
    if(const auto arg = pop_argument<std::string>(args, name))
    {
        return std::make_optional<std::pair<std::size_t, std::size_t>>(
                std::stoull(arg->substr(0, arg->find(':'))),
                std::stoull(arg->substr(arg->find(':') + 1)));
    }
    else
    {
        return std::nullopt;
    }
}

double rmsd(const std::vector<Vector<double, 3>>& lhs,
            const std::vector<Vector<double, 3>>& rhs)
{
    const std::size_t s = lhs.size();
    if(s != rhs.size())
    {
        log::fatal("mill calc rmsd: size differs");
    }

    double sigma = 0.;
    for(std::size_t i=0; i<s; ++i)
    {
        sigma += length_sq(lhs[i] - rhs[i]);
    }
    return std::sqrt(sigma / static_cast<double>(s));
}

int mode_calc_rmsd(std::deque<std::string_view> args)
{
    using vector_type = Vector<double, 3>;

    // remove unordered argumnets
    const auto do_align = pop_argument<bool       >(args, "align" ).value_or(true);
    const auto output   = pop_argument<std::string>(args, "output").value_or("mill_rmsd.dat");

    const auto only     = pop_range(args, "only");
    const auto ref_only = pop_range(args, "ref-only");

    if(args.empty())
    {
        log::error("mill calc rmsd: too few arguments.");
        log::error(mode_calc_rmsd_usage());
        return 1;
    }

    log::debug("only     = ", only);
    log::debug("ref_only = ", ref_only);

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
            if(ref_only)
            {
                const auto [first, last] = *ref_only;
                if(first_frame->size() <= first || first_frame->size() < last)
                {
                    log::fatal("The range specified by --ref-only (", *ref_only,
                        ") exceeds the size of snapshot (", first_frame->size(),
                        ").");
                }
                for(std::size_t i=first; i<last; ++i)
                {
                    ref.push_back(first_frame->at(i).position());
                }
            }
            else
            {
                for(const auto& particle : *first_frame)
                {
                    ref.push_back(particle.position());
                }
            }
        }
        log::info("mill calc rmsd: done. reference structure has ",
                  ref.size(), " particles.");
    }

    std::ofstream ofs(output);
    ofs << "#t rmsd\n";
    if(do_align)
    {
        BestFit bestfit;
        bestfit.set_reference(ref);

        std::size_t tstep = 0;
        for(const auto& frame : reader(fname))
        {
            std::vector<Vector<double, 3>> snapshot;
            if(only)
            {
                const auto [first, last] = *only;
                if(frame.size() <= first || frame.size() < last)
                {
                    log::fatal("The range specified by --only (", *only,
                        ") exceeds the size of snapshot (", frame.size(), ").");
                }
                for(std::size_t i=first; i<last; ++i)
                {
                    snapshot.push_back(frame.at(i).position());
                }
            }
            else
            {
                snapshot.reserve(frame.size());
                for(const auto& particle : frame)
                {
                    snapshot.push_back(particle.position());
                }
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
            if(only)
            {
                const auto [first, last] = *only;
                if(frame.size() <= first || frame.size() < last)
                {
                    log::fatal("The range specified by --only (", *only,
                        ") exceeds the size of snapshot (", frame.size(), ").");
                }
                for(std::size_t i=first; i<last; ++i)
                {
                    snapshot.push_back(frame.at(i).position());
                }
            }
            else
            {
                snapshot.reserve(frame.size());
                for(const auto& particle : frame)
                {
                    snapshot.push_back(particle.position());
                }
            }
            ofs << tstep << ' ' << rmsd(ref, snapshot) << '\n';
            ++tstep;
        }
    }
    return 0;
}

}//mill
