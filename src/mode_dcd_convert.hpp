#ifndef COFFEE_MILL_DCD_CONVERT
#define COFFEE_MILL_DCD_CONVERT
#include <mill/util/file_extension.hpp>
#include <mill/traj.hpp>
#include <toml/toml.hpp>

#include <string_view>
#include <deque>

namespace mill
{

inline const char* dcd_convert_usage() noexcept
{
    return "usage: mill dcd convert <format> <dcdfile> [reference(optional)]\n"
           "    $ mill dcd convert pdb traj.dcd [model.pdb(optional)]\n"
           "      convert traj.dcd to pdb format.\n"
           "      without pdb file, it consider all the particles as CA.\n"
           "      the pdb file must have the same number of particles with snapshot in dcd.\n";
}

// argv := arrayof{ "convert", "pdb", "filename", [pdb] }
inline int mode_dcd_convert(std::deque<std::string_view> args)
{
    if(args.size() < 2)
    {
        log::error("mill dcd convert: too few arguments");
        log::error(dcd_convert_usage());
        return 1;
    }

    const auto format = args.at(1);
    if(format == "help")
    {
        log::info(dcd_convert_usage());
        return 0;
    }
    else if(format != "pdb")
    {
        log::error("mill dcd convert: unknown format: ", format);
        log::error(dcd_convert_usage());
        return 1;
    }

    if(args.size() < 3)
    {
        log::error("mill dcd convert: too few arguments");
        log::error(dcd_convert_usage());
        return 1;
    }
    const auto filename = args.at(2);

    if(extension_of(filename) == ".dcd")
    {
        std::optional<Snapshot> ref_frame = std::nullopt;
        if(4 <= args.size() && extension_of(args.at(3)) == ".pdb")
        {
            PDBReader reader(std::string(args.at(3)));
            std::ignore = reader.read_header();
            ref_frame = reader.read_frame(); // read 1st frame
        }

        auto r    = reader(filename);
        auto traj = r.read();

        if(ref_frame)
        {
            for(auto& frame : traj)
            {
                Snapshot pdbinfo(ref_frame.value());
                frame.merge_attributes(pdbinfo);
            }
        }
        PDBWriter writer(std::string(base_name_of(filename)) + "_converted.pdb");
        writer.write(traj);

        return 0;
    }
    else
    {
        log::error("error: mill dcd convert: unknown file extension: ", filename);
        log::error(dcd_convert_usage());
        return 1;
    }
}

} // mill
#endif// COFFEE_MILL_DCD_CONVERT
