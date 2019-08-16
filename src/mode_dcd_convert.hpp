#ifndef COFFEE_MILL_DCD_CONVERT
#define COFFEE_MILL_DCD_CONVERT
#include <mill/dcd/DCDWriter.hpp>
#include <mill/dcd/DCDReader.hpp>
#include <mill/pdb/PDBReader.hpp>
#include <mill/pdb/PDBWriter.hpp>
#include <toml/toml.hpp>

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
template<typename vectorT>
int mode_dcd_convert(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        std::cerr << "error: mill dcd convert: too few arguments\n";
        std::cerr << dcd_convert_usage() << std::endl;
        return 1;
    }

    const std::string format(argument_v[1]);
    if(format == "help")
    {
        std::cerr << dcd_convert_usage() << std::endl;
        return 0;
    }
    else if(format != "pdb")
    {
        std::cerr << "error: mill dcd convert: unknown format: "
                  << format << '\n';
        std::cerr << dcd_convert_usage() << std::endl;
        return 1;
    }

    if(argument_c < 3)
    {
        std::cerr << "error: mill dcd convert: too few arguments\n";
        std::cerr << dcd_convert_usage() << std::endl;
        return 1;
    }
    const std::string fname(argument_v[2]);

    if(fname.substr(fname.size()-4, 4) == ".dcd")
    {
        std::string pdbname;
        if(argument_c >= 4)
        {
            pdbname = std::string(argument_v[3]);
        }

        const std::string outname = fname.substr(0, fname.size()-4) +
                                    "_converted.pdb";
        std::ofstream ofs(outname);
        if(not ofs.good())
        {
            std::cerr << "error: mill dcd convert: file open error: "
                      << outname << '\n';
            std::cerr << dcd_convert_usage() << std::endl;
            return 1;
        }

        DCDReader<vectorT> reader;
        PDBWriter<vectorT> writer;

        std::ifstream ifs(fname);
        const auto dcddata = reader.read_header(ifs);

        std::vector<PDBAtom<vectorT>> atoms(dcddata.nparticle());
        if(not pdbname.empty()) // reference pdb exists.
        {
            PDBReader<vectorT> pdbreader;
            std::ifstream pdbfile(pdbname);
            if(not pdbfile.good())
            {
                std::cerr << "error: mill dcd convert: file open error: "
                          << pdbname << '\n';
                std::cerr << dcd_convert_usage() << std::endl;
                return 1;
            }
            atoms = pdbreader.read(pdbname);
            if(atoms.size() != static_cast<std::size_t>(dcddata.nparticle()))
            {
                std::cerr << "error: mill dcd convert: "
                          << "pdb file may have different structure: "
                          << "num particle in dcd = " << dcddata.nparticle()
                          << "num particle in pdb = " << atoms.size() << '\n';
                std::cerr << dcd_convert_usage() << std::endl;
                return 1;
            }
        }
        else
        {
            std::size_t idx = 1;
            for(auto& atom : atoms)
            {
                atom = make_default_atom(static_cast<int>(idx), vectorT(0,0,0));
                ++idx;
            }
        }

        for(int i=0; i<dcddata.nset(); ++i)
        {
            ofs << "MODEL     " << std::setw(4) << i << '\n';
            std::size_t idx = 0;
            for(auto&& p : reader.read_snapshot(ifs, dcddata.header()))
            {
                atoms.at(idx).position = std::move(p);
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
        std::cerr << "error: mill dcd convert: unknown file extension"
                  << fname << '\n';
        std::cerr << dcd_convert_usage() << std::endl;
        return 1;
    }
}

} // mill
#endif// COFFEE_MILL_DCD_CONVERT
