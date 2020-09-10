#ifndef COFFEE_MILL_DCD_JOIN
#define COFFEE_MILL_DCD_JOIN
#include <mill/dcd/DCDReader.hpp>
#include <mill/dcd/DCDWriter.hpp>
#include <fstream>
#include <toml/toml.hpp>

#include <string_view>
#include <deque>

namespace mill
{

inline const char* dcd_join_usage() noexcept
{
    return "usage: mill dcd join [parameters...]\n"
           "  $ mill dcd join traj.dcd traj1.dcd traj2.dcd\n"
           "    just concatenate traj.dcd, traj1.dcd and traj2.dcd into traj1_joined.dcd\n"
           "  $ mill dcd join input.toml\n"
           "    specify path/to/dcdfile and output filename as an input file\n"
           "\n"
           "```toml\n"
           "inputs = [\n"
           "\"data/traj1.dcd\",\n"
           "\"data/traj2.dcd\",\n"
           "]\n"
           "output = \"data/traj.dcd\"\n"
           "include_initial = true # by default, it's true\n"
           "# if include_initial is false, this command removes initial snapshot from successive files.\n"
           "# it is for the case if the last snapshot of a file and the first snapshot of the next file is same.\n"
           "```\n";
}

//! argv = {"join", {args...}}
inline int mode_dcd_join(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::error("mill dcd-mode: too few arguments");
        log::error(dcd_join_usage());
        return 1;
    }

    const auto fname = args.at(1);
    if(fname == "help")
    {
        log::info(dcd_join_usage());
        return 0;
    }
    if(fname.size() < 5)
    {
        log::error("mill dcd join: invalid argument : ", fname);
        log::error(dcd_join_usage());
        return 1;
    }

    std::vector<std::string> files;
    std::string outname;
    bool include_initial;

    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {
        files.push_back(std::string(fname));
        //! argv = {"join", {"1.dcd", "2.dcd", ...}}
        for(std::size_t i=2; i<args.size(); ++i)
        {
            files.push_back(std::string(args.at(i)));
        }
        outname = std::string(fname.substr(0, fname.size() - 4)) + "_joined.dcd";
        include_initial = true;
    }
    else if(fname.substr(fname.size() - 5, 5) == ".toml")
    {
        const auto tomldata = toml::parse(std::string(fname));
        files = toml::find<std::vector<std::string>>(tomldata, "inputs");
        outname = toml::find<std::string>(tomldata, "output");
        include_initial = toml::find_or(tomldata, "include_initial", true);
    }
    else
    {
        log::error("mill dcd join: invalid argument : ", fname);
        log::error(dcd_join_usage());
        return 1;
    }

    log::info("files = ", files);

    Trajectory traj(DCDReader(fname).read_header());

    // check total steps
    traj.at("nset").as_integer() = 0;
    for(const auto& file : files)
    {
        log::info("reading ", file);
        DCDReader reader(file);
        const auto header = reader.read_header();

        if(header.at("nparticle").as_integer() != traj.at("nparticle").as_integer())
        {
            log::error("mill dcd join: dcd files have different structures!");
            log::error("file ", fname, " has ", traj.at("nparticle"),
                       " particles, but ", file, " has ", header.at("nparticle"));
            return 1;
        }
        traj.at("nset").as_integer() += header.at("nset").as_integer();
    }
    log::info("total frames = ", traj.at("nset"));

    DCDWriter writer(outname);
    writer.write_header(traj.attributes());
    for(const auto& file : files)
    {
        log::info("reading ", file);
        DCDReader reader(file);
        reader.read_header();
        if(not include_initial)
        {
            reader.read_frame();
        }
        for(const auto& frame : reader)
        {
            writer.write_frame(frame);
        }
    }
    return 0;

}

}//mill
#endif /* COFFEE_MILL_DCD_JOIN */
