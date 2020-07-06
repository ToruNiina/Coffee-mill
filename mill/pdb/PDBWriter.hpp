/*!
  @file PDBWriter.hpp
  @brief pdb writer.

  write pdb atoms or chains into output stream.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-10-18 10:00
  @copyright Toru Niina 2016 on MIT License
*/
#ifndef COFFEE_MILL_PDB_WRITER_HPP
#define COFFEE_MILL_PDB_WRITER_HPP
#include <mill/util/logger.hpp>
#include "PDBChain.hpp"
#include <fstream>
#include <sstream>

namespace mill
{

template<typename vectorT>
class PDBWriter
{
  public:

    enum class style
    {
        normal,  // use TER.
        cafemol, // use << protein 1 and >>
    };

    using vector_type  = vectorT;
    using atom_type    = PDBAtom<vector_type>;
    using residue_type = PDBResidue<vector_type>;
    using chain_type   = PDBChain<vector_type>;

  public:

    PDBWriter() = default;
    ~PDBWriter() = default;

    void write(const std::string& filename,
               const std::vector<atom_type>& atoms) const;

    void write(std::ostream& os, const std::vector<atom_type>& atoms) const;

    void write(const std::string& fname, const std::vector<chain_type>& model,
               const style sty = style::normal) const;

    void write(std::ostream& os, const std::vector<chain_type>& model,
               const style sty = style::normal) const;
};

template<typename vectorT>
void PDBWriter<vectorT>::write(
        const std::string& filename, const std::vector<atom_type>& atoms) const
{
    std::ofstream ofs(filename);
    if(not ifs.good()) {log::fatal("PDBWriter: file open error: ", fname);}
    this->write(ofs, atoms);
    ofs.close();
    return;
}

template<typename vectorT>
void PDBWriter<vectorT>::write(
        const std::string& filename, const std::vector<chain_type>& model,
        const style sty) const
{
    std::ofstream ofs(filename);
    if(not ifs.good()) {log::fatal("PDBWriter: file open error: ", fname);}
    this->write(ofs, model, sty);
    ofs.close();
    return;
}

template<typename vectorT>
void PDBWriter<vectorT>::write(std::ostream& os,
        const std::vector<atom_type>& atoms) const
{
    for(const auto& atom : atoms)
    {
        os << atom << std::endl;
    }
    return;
}

template<typename vectorT>
void PDBWriter<vectorT>::write(
        std::ostream& os, const std::vector<chain_type>& chains,
        const style sty) const
{
    std::size_t index = 1;
    for(const auto& chain : chains)
    {
        if(sty == style::cafemol)
        {
            os << "<< protein " << index << std::endl;
        }
        for(const auto& residue : chain)
        {
            for(const auto& atom : residue)
            {
                os << atom << std::endl;
            }
        }
        if     (sty == style::cafemol) {os << ">>" << std::endl; }
        else if(sty == style::normal)  {os << "TER" << std::endl;}
        ++index;
    }
    return;
}

} // mill
#endif /* COFFEE_MILL_PDB_WRITER */
