#ifndef COFFEE_MILL_SUPER_IMPOSE
#define COFFEE_MILL_SUPER_IMPOSE
#include "math/BestFitStructure.hpp"
#include "data/DCDReader.hpp"
#include "data/DCDWriter.hpp"
#include "data/PDBReader.hpp"
#include "data/PDBWriter.hpp"
#include <list>

namespace mill
{

template<typename T>
std::vector<T>
remove_except_elements(const std::vector<T>& elements,
        const std::vector<std::vector<std::int_least64_t>>& exception)
{
    std::vector<T> retval;
    retval.reserve(elements.size());
    std::int_least64_t index = 0;
    for(auto iter = elements.cbegin(); iter != elements.cend(); ++iter)
    {
        ++index;
        bool remove = false;
        for(auto e = exception.cbegin(); e != exception.cend(); ++e)
        {
            if(e->front() <= index && index <= e->back())
            {
                remove = true;
                break;
            }
        }
        if(remove) continue;
        retval.push_back(*iter);
    }
    return retval;
}

template<typename vectorT>
int superimpose(const std::string& fname)
{
    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {// no input file. use all particles.
        const std::string outname =
            fname.substr(0, fname.size() - 4) + "_imposed.dcd";

        std::ofstream ofs(outname);
        if(not ofs.good())
            throw std::runtime_error("file open error: " + outname);

        DCDReader<vectorT> reader;
        DCDWriter<vectorT> writer;
        auto dcddata = reader.read(fname);
        writer.write_header(ofs, dcddata.header());

        BestFit<typename scalar_type_extractor<vectorT>::type, Matrix> bestfit;
        bestfit.set_reference(dcddata.traj().front());
        for(auto snap = dcddata.cbegin(); snap != dcddata.cend(); ++snap)
        {
            writer.write_snapshot(ofs, bestfit.fit(*snap));
        }
        ofs.close();
        return 0;
    }
    else if(fname.substr(fname.size() - 5, 5) == ".toml")
    {// remove some particles to fit.
        std::ifstream tomlfile(fname);
        if(not tomlfile.good())
            throw std::invalid_argument("file open error: " + fname);

        const toml::Data input = toml::parse(tomlfile);
        const std::string dcdname = toml::get<toml::String>(input.at("dcdfile"));
        const std::string outname = toml::get<toml::String>(input.at("output"));
        const std::vector<std::vector<std::int_least64_t>> except_particle = 
            toml::get<toml::Array<toml::Array<toml::Integer>>>(input.at("except"));

        std::ofstream outfile(outname);
        if(not outfile.good())
            throw std::runtime_error("file open error: " + outname);

        DCDReader<vectorT> reader;
        DCDWriter<vectorT> writer;
        std::ifstream dcdfile(dcdname);
        auto dcddata = reader.read(dcdfile);
        writer.write_header(outfile, dcddata.header());

        auto initial = dcddata.traj().front();
        BestFit<typename scalar_type_extractor<vectorT>::type, Matrix> bestfit;
        bestfit.set_reference(remove_except_elements(initial, except_particle));
        for(auto snap = dcddata.cbegin(); snap != dcddata.cend(); ++snap)
        {
            const auto core = remove_except_elements(*snap, except_particle);
            const auto R = bestfit.rotational_matrix(core);
            const auto dx = bestfit.zeroing_vector(core);
            auto rotated = *snap;
            for(auto iter = rotated.begin(); iter != rotated.end(); ++iter)
                *iter -= dx;

            for(auto iter = rotated.begin(); iter != rotated.end(); ++iter)
            {
                auto tmp = R * (*iter);
                *iter = tmp;
            }
            writer.write_snapshot(outfile, rotated);
        }
        outfile.close();
        return 0;
    }
    else
    {
        throw std::invalid_argument("unknown file in argument: " + fname);
    }
}

}// mill

#endif//COFFEE_MILL_SUPER_IMPOSE
