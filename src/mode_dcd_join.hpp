#ifndef COFFEE_MILL_DCD_JOIN
#define COFFEE_MILL_DCD_JOIN
#include <mill/data/DCDReader.hpp>
#include <mill/data/DCDWriter.hpp>
#include <fstream>
#include <toml/toml.hpp>

namespace mill
{

inline const char* dcd_join_usage() noexcept
{
    return "usage: mill dcd join [parameters...]\n"
           "    $ mill dcd join traj.dcd traj1.dcd traj2.dcd\n"
           "    : concatenate traj.dcd, traj1.dcd and traj2.dcd into traj1_joined.dcd\n"
           "    $ mill dcd join input.toml\n"
           "    : specify path/to/dcdfile and output filename\n\n"
           "    # example.toml:\n"
           "    inputs = [\"data/traj1.dcd\", \"data/traj2.dcd\"]\n"
           "    output = \"data/traj.dcd\"\n"
           "    include_initial = true # by default, it's true\n"
           "    # if include_initial is false, this command removes initial snapshot from successive files.\n"
           "    # it is for the case if the last snapshot of a file and the first snapshot of the next file is same.\n";
}

//! argv = {"join", {args...}}
template<typename vectorT>
int mode_dcd_join(int argument_c, const char** argument_v)
{
    if(argument_c == 1)
    {
        std::cerr << "error: mill dcd-mode: too few arguments\n";
        std::cerr << dcd_join_usage() << std::endl;
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        std::cerr << dcd_join_usage() << std::endl;
        return 0;
    }
    if(fname.size() < 5)
    {
        std::cerr << "error: mill dcd join: invalid argument : "
                  << fname << '\n';
        std::cerr << dcd_join_usage() << std::endl;
        return 1;
    }

    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {
        //! argv = {"join", {"1.dcd", "2.dcd", ...}}
        DCDReader<vectorT> reader;
        auto dcddata = reader.read_header(fname);

        // check total steps
        for(int i=2; i<argument_c; ++i)
        {
            const std::string filename(argument_v[i]);
            const auto tmphdr = reader.read_header(filename);

            if(tmphdr.nparticle() != dcddata.nparticle())
            {
                std::cerr << "error: mill dcd join: "
                          << "dcd files have different structures!\n";
                std::cerr << "file " << fname << " has " << dcddata.nparticle()
                          << ", but file " << filename << " has "
                          << tmphdr.nparticle() << std::endl;
                return 1;
            }
            dcddata.nset() += tmphdr.nset();
        }

        const std::string outname =
            fname.substr(0, fname.size() - 4) + "_joined.dcd";

        std::ofstream ofs(outname);
        if(not ofs.good())
        {
            std::cerr << "error: mill dcd join: output file open error: "
                      << outname << std::endl;
            return 1;
        }

        DCDWriter<vectorT> writer;
        writer.write_header(ofs, dcddata.header());
        for(int i=0; i<argument_c; ++i)
        {
            const std::string filename(argument_v[i]);
            const auto tmpdata = reader.read(filename);
            for(const auto& ss : tmpdata)
            {
                writer.write_snapshot(ofs, ss);
            }
        }
        ofs.close();

        return 0;
    }
    else if(fname.substr(fname.size() - 5, 5) == "toml")
    {
        const auto tomldata = toml::parse(fname);
        const auto inputs =
            toml::get<std::vector<std::string>>(tomldata.at("inputs"));
        const auto output =
            toml::get<std::string>(tomldata.at("output"));
        const bool include_initial =
            toml::get_or(tomldata, "include_initial", true);

        DCDReader<vectorT> reader;
        auto dcddata = reader.read_header(inputs.front());

        // check total steps
        for(std::size_t i=1; i<inputs.size(); ++i)
        {
            const auto& filename = inputs[i];
            const auto tmpdata = reader.read_header(filename);
            if(tmpdata.nparticle() != dcddata.nparticle())
            {
                std::cerr << "error: mill dcd join: "
                          << "dcd files have different structures!\n";
                std::cerr << "file " << inputs.front() << " has "
                          << dcddata.nparticle()
                          << ", but file " << filename << " has "
                          << tmpdata.nparticle() << std::endl;
                return 1;
            }
            dcddata.nset() += tmpdata.nset();
        }

        std::ofstream ofs(output);
        if(not ofs.good())
        {
            std::cerr << "error: mill dcd join: output file open error: "
                      << output << std::endl;
            return 1;
        }

        DCDWriter<vectorT> writer;
        writer.write_header(ofs, dcddata.header());
        for(const auto& filename : inputs)
        {
            const auto tmpdata = reader.read(filename);
            if(include_initial)
            {
                writer.write_snapshot(ofs, tmpdata.front());
            }
            for(auto iter = tmpdata.cbegin()+1; iter != tmpdata.cend(); ++iter)
            {
                writer.write_snapshot(ofs, *iter);
            }
        }
        ofs.close();
        return 0;
    }
    else
    {
        std::cerr << "error: mill dcd join: invalid argument : "
                  << fname << '\n';
        std::cerr << dcd_join_usage() << std::endl;
        return 1;
    }
}

}//mill
#endif /* COFFEE_MILL_DCD_JOIN */
