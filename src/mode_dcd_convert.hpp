#ifndef COFFEE_MILL_DCD_CONVERT
#define COFFEE_MILL_DCD_CONVERT
#include <mill/dcd/DCDReader.hpp>
#include <mill/pdb/PDBReader.hpp>
#include <mill/pdb/PDBWriter.hpp>
#include <mill/util/file_extension.hpp>
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
    using vector_type = DCDReader::vector_type;
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
    const auto fname = args.at(2);

    if(extension_of(fname) == ".dcd")
    {
        std::optional<std::string> pdbname;
        if(args.size() >= 4)
        {
            pdbname = std::string(args.at(3));
        }

        const std::string outname = std::string(base_name_of(fname)) + "_converted.pdb";
        std::ofstream ofs(outname);
        if(not ofs.good())
        {
            log::error("mill dcd convert: file open error: ", outname);
            log::error(dcd_convert_usage());
            return 1;
        }

        DCDReader reader(fname);
        PDBWriter<vector_type> writer;

        const auto header = reader.read_header();

        const std::size_t num_particles = header.at("nparticle").as_integer();
        std::vector<PDBAtom<vector_type>> atoms(num_particles);
        if(pdbname) // reference pdb exists.
        {
            log::fatal("PDB reference option is currently unavailable");
//             PDBReader<vector_type> pdbreader;
//             std::ifstream pdbfile(*pdbname);
//             if(not pdbfile.good())
//             {
//                 log::error("mill dcd convert: file open error: ", *pdbname);
//                 log::error(dcd_convert_usage());
//                 return 1;
//             }
//             atoms = pdbreader.read(*pdbname);
//             if(atoms.size() != static_cast<std::size_t>(num_particles))
//             {
//                 log::error("mill dcd convert: file open error: "
//                            "pdb file may have different structure: ",
//                            "num particle in dcd = ", num_particles,
//                            "num particle in pdb = ", atoms.size());
//                 log::error(dcd_convert_usage());
//                 return 1;
//             }
        }
        else
        {
            std::size_t idx = 1;
            for(auto& atom : atoms)
            {
                atom = make_default_atom(static_cast<int>(idx), vector_type(0,0,0));
                ++idx;
            }
        }

        std::size_t model = 0;
        for(const auto& frame : reader)
        {
            ++model;
            ofs << "MODEL     " << std::setw(4) << model << '\n';
            std::size_t idx = 0;
            for(const auto& p : frame)
            {
                atoms.at(idx).position = p.position();
                ++idx;
            }
            writer.write(ofs, atoms);
            ofs << "ENDMDL\n";
        }
        ofs << std::flush;
        ofs.close();
        return 0;
    }
    else
    {
        log::error("error: mill dcd convert: unknown file extension: ", fname);
        log::error(dcd_convert_usage());
        return 1;
    }
}

} // mill
#endif// COFFEE_MILL_DCD_CONVERT
