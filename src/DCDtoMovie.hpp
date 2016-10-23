/*!
  @file DCDtoMovie.hpp
  @brief definition of a class that converts dcd file into movie file

  definition of a class DCDtoMovie. this class output trajectory data of
  dcd file into movie file (an array of pdb model) using the information
  included in pdb file. the pdb file should contain only one model and
  same number of particle in dcd snapshot.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_TO_MOVIE
#define COFFEE_MILL_DCD_TO_MOVIE
#include "toml/toml.hpp"
#include "data/DCDReader.hpp"
#include "data/DCDWriter.hpp"
#include "data/PDBReader.hpp"
#include "data/PDBWriter.hpp"

namespace mill
{

// TODO: support huge dcd file(convert stepwise)
template<typename vectorT>
int dcd_to_movie(const std::string& fname)
{
    if(fname.substr(fname.size() - 3, 3) == "dcd")
    {// no input file. make all beads Ca.
        const std::string outname = fname + ".movie";
        std::ofstream ofs(outname);
        if(not ofs.good())
            throw std::runtime_error("file open error: " + outname);

        DCDReader<vectorT> reader;
        PDBWriter<vectorT> writer;
        auto dcddata = reader.read(fname);
        std::vector<PDBAtom<vectorT>> atoms(dcddata.front().size());

        for(auto snap = dcddata.cbegin(); snap != dcddata.cend(); ++snap)
        {
            ofs << "MODEL" << std::endl;
            int id = 0;
            for(auto iter = snap->cbegin(); iter != snap->cend(); ++iter)
            {
                atoms.at(id) = make_default_atom(id+1, *iter);
                ++id;
            }
            writer.write(ofs, atoms);
            ofs << "ENDMDL" << std::endl;
        }
        ofs.close();
        return 0;
    }
    else if(fname.substr(fname.size() - 4, 4) == "toml")
    {// toml file exists. read reference pdb.
        std::ifstream ifs(fname);
        if(not ifs.good())
            throw std::invalid_argument("file open error: " + fname);

        const toml::Data input = toml::parse(ifs);
        const std::string dcdname = toml::get<toml::String>(input.at("dcdfile"));
        const std::string pdbname = toml::get<toml::String>(input.at("pdbfile"));
        const std::string outname = toml::get<toml::String>(input.at("output"));
        bool pymol_style = false;
        try{pymol_style = toml::get<toml::Boolean>(input.at("pymol_style"));}
        catch(std::exception& excpt){/*do nothing*/}

        PDBReader<vectorT> pdb_reader;
        auto atoms = pdb_reader.read(pdbname);
        DCDReader<vectorT> dcd_reader;
        auto dcddata = dcd_reader.read(dcdname);
        if(atoms.size() != dcddata.front().size())
            throw std::invalid_argument("dcd and pdb structures are not same");
        std::ofstream ofs(outname);
        if(not ofs.good())
            throw std::runtime_error("file open error: " + outname);
        const typename PDBWriter<vectorT>::style sty = pymol_style ?
            PDBWriter<vectorT>::style::normal : PDBWriter<vectorT>::style::cafemol;

        PDBWriter<vectorT> pdb_writer;
        for(auto snap = dcddata.cbegin(); snap != dcddata.cend(); ++snap)
        {
            ofs << "MODEL" << std::endl;
            if(not pymol_style)
                ofs << "<<<<" << std::endl;

            for(std::size_t i = 0; i < atoms.size(); ++i)
                atoms.at(i).position = snap->at(i);
            auto chains = pdb_reader.parse(atoms);
            pdb_writer.write(ofs, chains, sty);

            if(not pymol_style)
                ofs << ">>>>" << std::endl;
            ofs << "ENDMDL" << std::endl;
        }
        ifs.close();
        ofs.close();
        return 0;
    }
    else
    {
        throw std::invalid_argument("unknown file in argument: " + fname);
    }
}

} // mill

#endif /* COFFEE_MILL_DCD_TO_MOVIE */
