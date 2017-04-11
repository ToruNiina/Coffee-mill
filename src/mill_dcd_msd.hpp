#ifndef COFFEE_MILL_DCD_MSD
#define COFFEE_MILL_DCD_MSD
#include <mill/data/DCDReader.hpp>
#include <mill/util/scalar_type_of.hpp>
#include <toml/toml.hpp>
#include <fstream>
#include <algorithm>
#include <cassert>

namespace mill
{

template<typename vectorT>
struct calc_msd
{
    calc_msd(const vectorT& ini) : init(ini){}

    typename scalar_type_of<vectorT>::type
    operator()(const std::vector<vectorT>& snapshot)
    {
        vectorT center(0,0,0);
        for(auto&& pos : snapshot)
            center += pos;
        center /= static_cast<typename scalar_type_of<vectorT>::type
                              >(snapshot.size());
        return length_sq(init - center);
    }

    vectorT init;
};


// argv := {"msd", "traj.dcd", [traj.dcd...] }
//      || {"msd", "traj.toml"}
template<typename vectorT>
int mill_dcd_msd(int argument_c, char **argument_v)
{
    const std::string fname(argument_v[1]);
    std::string              outname;
    std::vector<std::string> dcdnames;
    if(fname.substr(fname.size()-4, 4) == ".dcd")
    {
        dcdnames.push_back(fname);
        for(int i=2; i<argument_c; ++i)
            dcdnames.push_back(std::string(argument_v[i]));
        outname = fname.substr(0, fname.size()-4) + std::string("_msd.ts");
    }
    else if(fname.substr(fname.size()-5, 5) == ".toml")
    {
        std::ifstream ifs(fname);
        if(not ifs.good())
        {
            std::cerr << "error: mill dcd extract: file open error"
                      << std::endl;
            return 1;
        }
        toml::Data data = toml::parse(ifs);
        try
        {
            outname = toml::get<toml::String>(data.at("outname"));
            dcdnames = toml::get<toml::Array<toml::String>>(data.at("dcdfiles"));
        }
        catch(std::exception& except)
        {
            std::cerr << "error: mill dcd msd: required toml values are..." << std::endl;
            std::cerr << "outname  = String" << std::endl;
            std::cerr << "dcdfiles = Array of {String, ...}" << std::endl;
        }
    }
    else
    {
        std::cerr << "error: mill dcd msd: unknown file extension" << std::endl;
        return 1;
    }

    DCDReader<vectorT> reader;
    typename scalar_type_of<vectorT>::type dt = 0.0;
    std::size_t nset = 0;
    for(auto&& filename : dcdnames)
    {
        const auto header = reader.read_header(filename);
        if(dt == 0.0)
        {
            dt = header.delta_t();
        }
        else if(dt != header.delta_t())
        {
            std::cerr << "error: mill dcd msd: hetero-delta_t is not yet supported" << std::endl;
            return 1;
        }

        if(nset == 0)
        {
            nset = header.nset();
        }
        else if(nset != header.nset())
        {
            std::cerr << "error: mill dcd msd: nset differs" << std::endl;
            return 1;
        }
    }

    std::size_t idx = 0;
    std::vector<std::vector<typename scalar_type_of<vectorT>::type>>
        msds(dcdnames.size());
    for(auto&& filename : dcdnames)
    {
        const auto traj = reader.read(filename);
        assert(nset == traj.size());

        vectorT pos0(0,0,0);
        for(auto&& pos : traj.front()) pos0 += pos;
        pos0 /= static_cast<typename scalar_type_of<vectorT>::type
            >(traj.front().size());

        msds.at(idx).resize(traj.size());
        std::transform(traj.cbegin(), traj.cend(), msds.at(idx).begin(),
                       calc_msd<vectorT>(pos0));
        ++idx;
    }

    std::vector<typename scalar_type_of<vectorT>::type> msd(msds.front().size());
    for(std::size_t i=0; i<msds.front().size(); ++i)
    {
        typename scalar_type_of<vectorT>::type mean = 0;
        for(std::size_t j=0; j<msds.size(); ++j)
        {
            mean += msds.at(j).at(i);
        }
        mean /= static_cast<typename scalar_type_of<vectorT>::type>(msds.size());

        msd.at(i) = mean;
    }

    std::vector<typename scalar_type_of<vectorT>::type> err(msds.front().size());
    for(std::size_t i=0; i<msds.front().size(); ++i)
    {
        typename scalar_type_of<vectorT>::type s = 0;
        for(std::size_t j=0; j<msds.size(); ++j)
        {
            s += std::pow(msds.at(j).at(i) - msd.at(i), 2);
        }
        s /= (msds.size()-1);

        err.at(i) = std::sqrt(s / msds.size());
    }

    std::ofstream ofs(outname);
    if(!ofs.good())
    {
        std::cerr << "file open error: " << outname << std::endl;
        return 1;
    }
    typename scalar_type_of<vectorT>::type t = 0.;
    ofs << "tstep, time, msd, std-err" << std::endl;
    for(std::size_t i=0; i<msd.size(); ++i)
    {
        ofs << i << " " << t << " " << msd.at(i) << " " << err.at(i) << std::endl;
        t += dt;
    }
    ofs.close();

    return 0;
}




} // mill
#endif //COFFEE_MILL_DCD_MSD
