#include <mill/util/logger.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/math/BestFitStructure.hpp>
#include <mill/traj.hpp>
#include "mode_traj_impose.hpp"
#include <toml/toml.hpp>
#include <iostream>

namespace mill
{

const char* mode_traj_impose_usage() noexcept
{
    return "usage: mill traj impose [parameters...]\n"
           "    $ mill traj impose traj.dcd\n"
           "      impose all the snapshots in traj.dcd by minimizing RMSD\n"
           "    $ mill traj impose input.toml\n"
           "    # example.toml:\n"
           "    input  = \"data/traj.dcd\"\n"
           "    output = \"data/traj_imposed.xyz\" # you can convert it here\n"
           "    except = [ # ignore some of the particles when calculating RMSD\n"
           "      # Take care, the index starts from zero!\n"
           "      [  0,  99], # ignore 1st to 100-th particles\n"
           "      [400, 500], # also ignore 400-th to 500-th particles\n"
           "    ]\n";
}

std::vector<Snapshot::vector_type>
remove_except_elements(const Snapshot& frame,
        const std::vector<std::array<std::int64_t, 2>>& except)
{
    std::vector<Snapshot::vector_type> retval;
    retval.reserve(frame.size());

    std::int64_t index = 0;
    for(const auto& particle : frame)
    {
        ++index;
        if(std::find_if(except.begin(), except.end(), [=](const auto& e){
                return e.front() <= index && index <= e.back();
            }) != except.end())
        {
            continue;
        }
        retval.push_back(particle.position());
    }
    return retval;
}

int mode_traj_impose(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("error: mill dcd impose: too few arguments");
        log::error(mode_traj_impose_usage());
        return 1;
    }

    const auto fname = args.front();
    if(fname == "help")
    {
        log::info(mode_traj_impose_usage());
        return 0;
    }
    args.pop_front();

    std::string input;
    std::string output;
    std::vector<std::array<std::int64_t, 2>> except_particle{};

    if(extension_of(fname) == ".toml")
    {
        const auto data = toml::parse(std::string(fname));

        input  = toml::find<std::string>(data, "input");
        output = toml::find<std::string>(data, "output");
        except_particle = toml::find<std::vector<std::array<std::int64_t, 2>>>(data, "except");
    }
    else
    {
        input  = std::string(fname);
        output = std::string(base_name_of(fname)) + std::string("_imposed") +
                 std::string(extension_of(fname));
    }

    auto r = reader(input);
    auto w = writer(output);

    w.write_header(r.read_header());

    BestFit<double> bestfit;
    // set initial frame as a reference frame
    {
        const auto initial_frame = *(r.read_frame());
        bestfit.set_reference(remove_except_elements(initial_frame, except_particle));
        w.write_frame(initial_frame);
    }

    for(auto frame : r)
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
        w.write_frame(frame);
    }
    return 0;
}

} // mill
