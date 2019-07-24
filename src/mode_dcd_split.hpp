#ifndef COFFEE_MILL_DCD_SPLIT
#define COFFEE_MILL_DCD_SPLIT
#include <mill/dcd/DCDReader.hpp>
#include <mill/dcd/DCDWriter.hpp>
#include <fstream>

namespace mill
{

inline const char* dcd_split_usage() noexcept
{
    return "usage: mill dcd split [parameters...]\n"
           "    $ mill dcd split traj.dcd 100 \n"
           "      split traj.dcd for every 100 snapshot\n";
// TODO
//            "    $ mill dcd split input.toml\n"
//            "    # example.toml:\n"
//            "    input  = \"data/traj.dcd\"\n"
//            "    output = [\n"
//            "        {filename = \"data/traj1.dcd\", begin =   0, end =  99},\n"
//            "        {filename = \"data/traj2.dcd\", begin = 100, end = 199},\n"
//            "        {filename = \"data/traj3.dcd\", begin = 200, end = 499},\n"
//            "    ]\n";
}

//! argv = {"split", {args...}}
template<typename vectorT>
int mode_dcd_split(int argument_c, const char** argument_v)
{
    if(argument_c == 1)
    {
        std::cerr << "error: mill dcd split: too few arguments\n";
        std::cerr << dcd_split_usage() << std::endl;
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        std::cerr << dcd_split_usage() << std::endl;
        return 0;
    }
    if(fname.size() < 5)
    {
        std::cerr << "error: mill dcd split: invalid argument : "
                  << fname << '\n';
        std::cerr << dcd_split_usage() << std::endl;
        return 1;
    }

    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {
        //! argv = {"split", "traj.dcd" 100}
        if(argument_c < 3)
        {
            std::cerr << "error: mill dcd split: too few arguments\n";
            std::cerr << dcd_split_usage() << std::endl;
            return 0;
        }

        std::size_t unit;
        try
        {
            unit = std::stoi(std::string(argument_v[2]));
        }
        catch(std::exception& excpt)
        {
            std::cerr << "error: mill dcd-mode: invalid argument: ";
            std::cerr << argument_v[2] << " cannot parse as int.\n";
            std::cerr << dcd_split_usage() << std::endl;
            return 1;
        }

        std::ifstream ifs(fname);
        if(not ifs.good())
        {
            std::cerr << "error: mill dcd split: file open error: "
                      << fname << '\n';
            std::cerr << dcd_split_usage() << std::endl;
            return 1;
        }

        DCDReader<vectorT> reader;
        auto dcddata = reader.read_header(ifs);
        const auto header = dcddata.header();

        dcddata.nset() = unit;
        dcddata.traj().resize(unit);
        const std::size_t n_files = header.nset / unit;
        for(std::size_t i=0; i<n_files; ++i)
        {
            const std::string outname = fname.substr(0, fname.size() - 4) +
                std::string("_") + std::to_string(i) + std::string(".dcd");

            std::ofstream ofs(outname);
            if(not ofs.good())
            {
                std::cerr << "error: mill dcd split: output file open error: "
                          << outname << std::endl;
                return 1;
            }
            for(std::size_t i=0; i < unit; ++i)
            {
                dcddata[i] = reader.read_snapshot(ifs, header);
            }

            DCDWriter<vectorT> writer;
            writer.write(ofs, dcddata);
            ofs.close();
        }

        if(header.nset % unit != 0)
        {
            const std::size_t sz = header.nset % unit;
            dcddata.nset() = sz;
            dcddata.traj().resize(sz);
            const std::string outname = fname.substr(0, fname.size() - 4) +
                std::string("_") + std::to_string(n_files) + std::string(".dcd");

            std::ofstream ofs(outname);
            if(not ofs.good())
            {
                std::cerr << "error: mill dcd split: output file open error: "
                          << outname << std::endl;
                return 1;
            }
            for(std::size_t i=0; i < sz; ++i)
            {
                dcddata[i] = reader.read_snapshot(ifs, header);
            }

            DCDWriter<vectorT> writer;
            writer.write(ofs, dcddata);
            ofs.close();
        }
        return 0;
    }
    else
    {
        std::cerr << "error: mill dcd split: invalid argument : "
                  << fname << '\n';
        std::cerr << dcd_split_usage() << std::endl;
        return 1;
    }
}

}//mill
#endif /* COFFEE_MILL_DCD_SPLIT */
