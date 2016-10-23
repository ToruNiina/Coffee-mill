/*!
  @file DCDJoiner.hpp
  @brief definition of a class that joins some dcd files.

  DCDJoiner joins some dcdfiles to one dcd file. This help the analysis or 
  visualization of cafemol restarting simulation.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 16:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCDJOINER
#define COFFEE_MILL_DCDJOINER
#include "data/DCDReader.hpp"
#include "data/DCDWriter.hpp"
#include "toml/toml.hpp"

namespace mill
{

template<typename vectorT>
int dcdjoin(int argument_c, char** argument_v)
{
    const std::string fname(argument_v[0]);
    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {// join all the dcd files...
        DCDReader<vectorT> reader;
        auto dcddata = reader.read_header(fname);
        // check total steps
        for(int i=1; i<argument_c; ++i)
        {
            const std::string filename(argument_v[i]);
            const auto tmpdata = reader.read_header(filename);
            if(tmpdata.nparticle() != dcddata.nparticle())
                throw std::invalid_argument("different structure");
            dcddata.nset() += tmpdata.nset();
        }

        // write all the dcd files
        const std::string outname = fname.substr(0, fname.size() - 4) +
                                    "_joined.dcd";
        std::ofstream ofs(outname);
        if(not ofs.good())
            throw std::runtime_error("file open error: " + outname);

        DCDWriter<vectorT> writer;
        writer.write_header(ofs, dcddata.header());
        for(int i=0; i<argument_c; ++i)
        {
            const std::string filename(argument_v[i]);
            std::ifstream ifs(filename);
            const auto tmpdata = reader.read_header(ifs);
            writer.write_snapshot(ofs, reader.read_snapshot(ifs, dcddata.header()));
        }
        ofs.close();
        return 0;
    }
    else if(fname.substr(fname.size() - 4, 4) == "toml")
    {// toml file exists. read detail setting.
        std::ifstream tomlfile(fname);
        if(not tomlfile.good())
            throw std::invalid_argument("file open error: " + fname);

        const toml::Data tomldata = toml::parse(tomlfile);
        const std::vector<std::string> inputs =
            toml::get<toml::Array<toml::String>>(tomldata.at("inputs"));
        const std::string outname = toml::get<toml::String>(tomldata.at("output"));
        bool redundant = false;
        try{redundant = toml::get<toml::Boolean>(tomldata.at("redundancy"));}
        catch(std::exception& excpt){/*do nothing*/}

        DCDReader<vectorT> reader;
        auto dcddata = reader.read_header(inputs.front());
        // check total steps
        for(std::size_t i=1; i<inputs.size(); ++i)
        {
            const std::string filename(inputs[i]);
            const auto tmpdata = reader.read_header(filename);
            if(tmpdata.nparticle() != dcddata.nparticle())
                throw std::invalid_argument("different structure");
            dcddata.nset() += tmpdata.nset();
        }

        std::ofstream ofs(outname);
        if(not ofs.good())
            throw std::runtime_error("file open error: " + outname);

        DCDWriter<vectorT> writer;
        writer.write_header(ofs, dcddata.header());
        for(int i=0; i<argument_c; ++i)
        {
            const std::string filename(argument_v[i]);
            std::ifstream ifs(filename);
            const auto tmpdata = reader.read_header(ifs);
            if(not redundant) auto tmp = reader.read_snapshot(ifs, tmpdata.header());

            writer.write_snapshot(ofs, reader.read_snapshot(ifs, tmpdata.header()));
        }
        ofs.close();
        return 0;
    }
    else
    {
        throw std::invalid_argument("unknown file in argument: " + fname);
    }
}

}

#endif /* COFFEE_MILL_DCDJOINER */
