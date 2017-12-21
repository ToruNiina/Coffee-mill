#ifndef COFFEE_MILL_DCD_IMPOSE
#define COFFEE_MILL_DCD_IMPOSE
#include <mill/math/BestFitStructure.hpp>
#include <mill/data/DCDReader.hpp>
#include <mill/data/DCDWriter.hpp>
#include <mill/data/PDBReader.hpp>
#include <mill/data/PDBWriter.hpp>
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

template<typename T>
std::vector<T>
remove_except_elements(const std::vector<T>& elements,
        const std::vector<std::array<std::int64_t, 2>>& exception)
{
    std::vector<T> retval;
    retval.reserve(elements.size());
    std::int64_t index = 0;
    for(const auto& elem : elements)
    {
        ++index;
        if(std::find_if(exception.begin(), exception.end(),
            [=](const std::array<std::int64_t, 2>& e){
                return e.front() <= index && index <= e.back();
            }) != exception.end()) {continue;}

        retval.push_back(elem);
    }
    return retval;
}

//! argv = {"impose", {args...}}
template<typename vectorT>
int mode_dcd_impose(int argument_c, const char** argument_v)
{
    if(argument_c == 1)
    {
        std::cerr << "error: mill dcd impose: too few arguments\n";
        std::cerr << dcd_impose_usage() << std::endl;
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname == "help")
    {
        std::cerr << dcd_impose_usage() << std::endl;
        return 0;
    }
    if(fname.size() < 5)
    {
        std::cerr << "error: mill dcd impose: invalid argument : "
                  << fname << '\n';
        std::cerr << dcd_impose_usage() << std::endl;
        return 1;
    }

    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {
        const std::string outname =
            fname.substr(0, fname.size() - 4) + "_imposed.dcd";

        std::ofstream ofs(outname);
        if(not ofs.good())
        {
            std::cerr << "error: mill dcd impose: file open error : "
                      << outname << '\n';
            std::cerr << dcd_impose_usage() << std::endl;
            return 1;
        }

        DCDReader<vectorT> reader;
        DCDWriter<vectorT> writer;
        auto dcddata = reader.read(fname);
        writer.write_header(ofs, dcddata.header());

        BestFit<typename scalar_type_of<vectorT>::type> bestfit;
        bestfit.set_reference(dcddata.traj().front());
        for(const auto& snap : dcddata)
        {
            writer.write_snapshot(ofs, bestfit.fit(snap));
        }
        ofs.close();
        return 0;
    }
    else if(fname.substr(fname.size() - 5, 5) == ".toml")
    {
        const auto data = toml::parse(fname);

        const std::string input    = toml::get<std::string>(data.at("input"));
        const std::string output   = toml::get<std::string>(data.at("output"));
        const auto except_particle = toml::get<
            std::vector<std::array<std::int64_t, 2>>>(data.at("except"));

        std::ofstream outfile(output);
        if(not outfile.good())
        {
            std::cerr << "error: mill dcd impose: file open error : "
                      << output << '\n';
            std::cerr << dcd_impose_usage() << std::endl;
            return 1;
        }

        std::ifstream dcdfile(input);
        if(not dcdfile.good())
        {
            std::cerr << "error: mill dcd impose: file open error : "
                      << input << '\n';
            std::cerr << dcd_impose_usage() << std::endl;
            return 1;
        }

        DCDReader<vectorT> reader;
        DCDWriter<vectorT> writer;

        auto dcddata = reader.read(dcdfile);
        writer.write_header(outfile, dcddata.header());

        auto initial = dcddata.traj().front();
        BestFit<typename scalar_type_of<vectorT>::type> bestfit;
        bestfit.set_reference(remove_except_elements(initial, except_particle));

        for(const auto& snap : dcddata)
        {
            const auto core = remove_except_elements(snap, except_particle);
            const auto R  = bestfit.rotational_matrix(core);
            const auto dx = bestfit.zeroing_vector(core);
            auto rotated = snap;
            for(auto& item : rotated) {item -= dx;}
            for(auto& item : rotated) {const auto tmp(R * item); item = tmp;}
            writer.write_snapshot(outfile, rotated);
        }
        outfile.close();
        return 0;
    }
    else
    {
        std::cerr << "error: mill dcd impose: invalid argument : "
                  << fname << '\n';
        std::cerr << dcd_impose_usage() << std::endl;
        return 1;
    }
}

}//mill
#endif /* COFFEE_MILL_DCD_IMPOSE */
