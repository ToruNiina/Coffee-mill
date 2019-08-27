/*!
  @file PDBReader.hpp
  @brief pdb reader.

  read pdb atoms from input stream and parse atoms as chains.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 13:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_PDB_READER
#define COFFEE_MILL_PDB_READER
#include "PDBChain.hpp"
#include <fstream>
#include <sstream>

namespace mill
{

//! PDBReader class
/*!
 * read atoms in pdb file until "ENDMDL" or "END".
 * and then parse the PDBAtoms into vector of PDBChains.
 * for movie file, parse only one model at once. you can specify the model
 * by model index.
 */
template<typename vectorT>
class PDBReader
{
  public:

    using vector_type  = vectorT;
    using atom_type    = PDBAtom<vector_type>;
    using residue_type = PDBResidue<vector_type>;
    using chain_type   = PDBChain<vector_type>;

  public:

    PDBReader() = default;
    ~PDBReader() = default;

    std::vector<chain_type> parse(const std::vector<atom_type>& atoms) const;

    std::vector<atom_type> read(const std::string& fname) const;
    std::vector<atom_type> read(const std::string& fname, const std::size_t model_idx) const;
    std::vector<atom_type> read(std::istream& fname) const;
    std::vector<atom_type> read(std::istream& fname, const std::size_t model_idx) const;
};

template<typename vectorT>
std::vector<typename PDBReader<vectorT>::atom_type>
PDBReader<vectorT>::read(const std::string& fname) const
{
    log(log_level::debug, "reading PDB file ", fname, '\n');
    std::ifstream ifs(fname);
    if(not ifs.good()) {throw std::runtime_error("file open error: " + fname);}
    const auto data = this->read(ifs);
    return data;
}

template<typename vectorT>
std::vector<typename PDBReader<vectorT>::atom_type>
PDBReader<vectorT>::read(const std::string& fname, const std::size_t model_idx) const
{
    log(log_level::debug, "reading PDB file ", fname, " model ", model_idx, "\n");
    std::ifstream ifs(fname);
    if(not ifs.good()) {throw std::runtime_error("file open error: " + fname);}
    const auto data = this->read(ifs, model_idx);
    return data;
}

template<typename vectorT>
std::vector<typename PDBReader<vectorT>::atom_type>
PDBReader<vectorT>::read(std::istream& ifs, const std::size_t model_idx) const
{
    while(not ifs.eof())
    {
        std::string line;
        std::getline(ifs, line);
        if(line.empty()) {continue;}
        if(line.substr(0, 5) == "MODEL")
        {
            std::size_t index;
            std::istringstream iss(line);
            std::string model;
            iss >> model >> index;
            log(log_level::debug, "PDB model ", index, " found\n");
            if(index == model_idx) {break;}
        }
    }
    if(ifs.eof())
    {
        throw std::invalid_argument("no model #" + std::to_string(model_idx));
    }
    return this->read(ifs);
}

template<typename vectorT>
std::vector<typename PDBReader<vectorT>::atom_type>
PDBReader<vectorT>::read(std::istream& ifs) const
{
    std::vector<atom_type> atoms;
    while(not ifs.eof())
    {
        std::string line;
        std::getline(ifs, line);
        if(line.empty()) {continue;}
        if(line.substr(0, 3) == "END") {break;}

        atom_type atom;
        if(line >> atom)
        {
            log(log_level::debug, "PDB reading `", line, "`\n");
            atoms.emplace_back(std::move(atom));
        }
    }
    return atoms;
}

template<typename vectorT>
std::vector<typename PDBReader<vectorT>::chain_type>
PDBReader<vectorT>::parse(const std::vector<atom_type>& atoms) const
{
    log(log_level::debug, "PDB parsing atoms ...\n");
    std::vector<residue_type> residues;
    residue_type tmp_residue;
    int current_residue_id = std::numeric_limits<int>::min();
    for(const auto& atom : atoms)
    {
        if(atom.residue_id != current_residue_id)
        {
            if(not tmp_residue.empty())
            {
                residues.push_back(tmp_residue);
            }
            tmp_residue.clear();
            current_residue_id = atom.residue_id;
            log(log_level::debug, "residue ", current_residue_id, " found.\n");
        }
        tmp_residue.push_back(atom);
    }
    if(not tmp_residue.empty())
    {
        residues.push_back(tmp_residue);
    }
    log(log_level::debug, residues.size(), " residues found.\n");

    std::vector<chain_type> chains;
    chain_type tmp_chain;
    std::string current_chain_id = "";
    for(const auto& residue : residues)
    {
        if(residue.chain_id() != current_chain_id)
        {
            if(not tmp_chain.empty())
            {
                chains.push_back(tmp_chain);
            }
            tmp_chain.clear();
            current_chain_id = residue.chain_id();
            log(log_level::debug, "chain ", current_chain_id, " found.\n");
        }
        tmp_chain.push_back(residue);
    }
    if(not tmp_chain.empty())
    {
        chains.push_back(tmp_chain);
    }
    log(log_level::debug, chains.size(), " chains found.\n");
    return chains;
}

} // mill
#endif//COFFEE_MILL_PURE_PDB_READER
