#include "mode_psf_gen.hpp"
#include <mill/common/Snapshot.hpp>
#include <mill/common/Topology.hpp>
#include <mill/psf/PSFWriter.hpp>
#include <mill/util/file_extension.hpp>
#include <mill/util/logger.hpp>
#include <toml/toml.hpp>

namespace mill
{

const char* mode_psf_gen_usage() noexcept
{
    return "usage: mill psf gen input.toml\n"
           "      generate psf file just for visualization from an input file of Mjolnir.\n";
}

int mode_psf_gen(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::error("mill psf gen: too few arguments");
        log::error(mode_psf_gen_usage());
        return 1;
    }

    const auto fname = args.at(1);
    if(fname == "help")
    {
        log::info(mode_psf_gen_usage());
        return 0;
    }

    if(extension_of(fname) != ".toml")
    {
        log::info(mode_psf_gen_usage());
        return 1;
    }

    const auto input = toml::parse(std::string(fname));

    const auto& particles = input.at("systems").at(0).at("particles").as_array();
    Snapshot frame(particles.size());
    for(std::size_t i=0; i<particles.size(); ++i)
    {
        frame.at(i) = Particle({{"name", toml::find<std::string>(particles.at(i), "name")}});
    }

    frame.topology() = Topology(particles.size());
    for(const auto& local : input.at("forcefields").at(0).at("local").as_array())
    {
        if(toml::find<std::string>(local, "interaction") != "BondLength")
        {
            continue;
        }
        if(toml::find<std::string>(local, "topology") != "bond")
        {
            continue;
        }
        for(const auto& param : local.at("parameters").as_array())
        {
            const auto [i, j] = toml::find<std::pair<std::size_t, std::size_t>>(param, "indices");
            frame.topology().add_edge(i, j, "bond");
        }
    }

    const auto file_name =
        toml::find<std::string>(input, "files", "output", "prefix") + ".psf";
    PSFWriter w(file_name);

    w.write_frame(frame);

    return 0;
}

} // mill
