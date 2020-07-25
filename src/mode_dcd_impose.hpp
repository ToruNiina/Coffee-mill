#ifndef COFFEE_MILL_DCD_IMPOSE
#define COFFEE_MILL_DCD_IMPOSE
#include <mill/math/BestFitStructure.hpp>
#include <mill/dcd/DCDReader.hpp>
#include <mill/dcd/DCDWriter.hpp>
#include <mill/pdb/PDBReader.hpp>
#include <mill/pdb/PDBWriter.hpp>
#include <fstream>
#include <toml/toml.hpp>

namespace mill
{

inline const char* dcd_impose_usage() noexcept
{
    return "usage: mill dcd impose [parameters...]\n"
           "    $ mill dcd impose traj.dcd\n"
           "      impose all the snapshots in traj.dcd by minimizing RMSD\n"
           "    $ mill dcd impose input.toml\n"
           "    # example.toml:\n"
           "    input  = \"data/traj.dcd\"\n"
           "    output = \"data/traj_imposed.dcd\"\n"
           "    except = [ # ignore some of the particles when calculating RMSD\n"
           "      [  1, 100], # ignore 1st to 100-th particles\n"
           "      [400, 500], # also ignore 400-th to 500-th particles\n"
           "    ]\n";
}

inline std::vector<Snapshot::vector_type>
remove_except_elements(const Snapshot& frame,
        const std::vector<std::array<std::int64_t, 2>>& exception)
{
    std::vector<Snapshot::vector_type> retval;
    retval.reserve(frame.size());
    std::int64_t index = 0;
    for(const auto& particle : frame)
    {
        ++index;
        if(std::find_if(exception.begin(), exception.end(),
            [=](const std::array<std::int64_t, 2>& e){
                return e.front() <= index && index <= e.back();
            }) != exception.end())
        {
            continue;
        }
        retval.push_back(particle.position());
    }
    return retval;
}

//! argv = {"impose", {args...}}
inline int mode_dcd_impose(int argument_c, const char** argument_v)
{
    using vector_type = DCDReader::vector_type;
    if(argument_c == 1)
    {
        log::error("error: mill dcd impose: too few arguments");
        log::error(dcd_impose_usage());
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        log::info(dcd_impose_usage());
        return 0;
    }
    if(fname.size() < 5)
    {
        log::error("mill dcd impose: invalid argument : ", fname);
        log::error(dcd_impose_usage());
        return 1;
    }

    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {
        const std::string outname =
            fname.substr(0, fname.size() - 4) + "_imposed.dcd";

        DCDReader reader(fname);
        DCDWriter writer(outname);
        writer.write_header(reader.read_header());

        const auto first_frame = *reader.read_frame();

        std::vector<vector_type> ref;
        for(const auto& p : first_frame)
        {
            ref.push_back(p.position());
        }
        BestFit<typename scalar_type_of<vector_type>::type> bestfit;
        bestfit.set_reference(ref);

        writer.write_frame(first_frame);
        for(auto frame : reader)
        {
            std::vector<vector_type> coord;
            for(const auto& p : frame)
            {
                coord.push_back(p.position());
            }
            std::size_t i=0;
            for(const auto& p : bestfit.fit(coord))
            {
                frame[i].position() = p;
                ++i;
            }
            writer.write_frame(frame);
        }
        return 0;
    }
    else if(fname.substr(fname.size() - 5, 5) == ".toml")
    {
        const auto data = toml::parse(fname);

        const auto input    = toml::find<std::string>(data, "input");
        const auto output   = toml::find<std::string>(data, "output");
        const auto except_particle = toml::find<
            std::vector<std::array<std::int64_t, 2>>>(data, "except");

        DCDReader reader(input);
        DCDWriter writer(output);

        writer.write_header(reader.read_header());

        const auto& first_frame = *reader.read_frame();

        BestFit<typename scalar_type_of<vector_type>::type> bestfit;
        bestfit.set_reference(remove_except_elements(first_frame, except_particle));

        for(auto frame : reader)
        {
            const auto core = remove_except_elements(frame, except_particle);
            const auto R  = bestfit.rotational_matrix(core);
            const auto dx = bestfit.zeroing_vector(core);
            for(auto& particle : frame)
            {
                particle.position() -= dx;
            }
            for(auto& particle : frame)
            {
                const auto tmp(R * particle.position());
                particle.position() = tmp;
            }
            writer.write_frame(frame);
        }
        return 0;
    }
    else
    {
        log::error("mill dcd impose: invalid argument : ", fname);
        log::error(dcd_impose_usage());
        return 1;
    }
}

}//mill
#endif /* COFFEE_MILL_DCD_IMPOSE */
