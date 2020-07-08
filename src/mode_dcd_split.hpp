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
int mode_dcd_split(int argument_c, const char** argument_v)
{
    if(argument_c == 1)
    {
        log::error("error: mill dcd split: too few arguments");
        log::error(dcd_split_usage());
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        log::info(dcd_split_usage());
        return 0;
    }
    if(fname.size() < 5)
    {
        log::error("mill dcd split: invalid argument : ", fname);
        log::error(dcd_split_usage());
        return 1;
    }

    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {
        //! argv = {"split", "traj.dcd" 100}
        if(argument_c < 3)
        {
            log::error("mill dcd split: too few arguments");
            log::error(dcd_split_usage());
            return 0;
        }

        std::size_t unit;
        try
        {
            unit = std::stoi(std::string(argument_v[2]));
        }
        catch(std::exception& excpt)
        {
            log::error("mill dcd split: invalid argument: ", argument_v[2],
                       " is not an integer.");
            log::error(dcd_split_usage());
            return 1;
        }

        DCDReader reader(fname);
        Trajectory traj(reader.read_header());
        const std::size_t total_frame = traj.at("nset").as_integer();
        const std::size_t n_files = total_frame / unit;

        traj.at("nset") = unit;
        traj.snapshots().resize(unit);
        for(std::size_t i=0; i<n_files; ++i)
        {
            const std::string outname = fname.substr(0, fname.size() - 4) +
                std::string("_") + std::to_string(i) + std::string(".dcd");

            DCDWriter writer(outname);
            writer.write_header(traj.attributes());
            for(std::size_t i=0; i < unit; ++i)
            {
                writer.write_frame(*reader.read_frame());
            }
        }

        if(total_frame % unit != 0)
        {
            const std::size_t sz = total_frame % unit;
            traj.at("nset") = sz;
            traj.snapshots().resize(sz);

            const std::string outname = fname.substr(0, fname.size() - 4) +
                std::string("_") + std::to_string(n_files) + std::string(".dcd");

            DCDWriter writer(outname);
            writer.write_header(traj.attributes());
            for(std::size_t i=0; i < sz; ++i)
            {
                writer.write_frame(*reader.read_frame());
            }
        }
        return 0;
    }
    else
    {
        log::error("mill dcd split: invalid argument : ", fname);
        log::error(dcd_split_usage());
        return 1;
    }
}

}//mill
#endif /* COFFEE_MILL_DCD_SPLIT */
