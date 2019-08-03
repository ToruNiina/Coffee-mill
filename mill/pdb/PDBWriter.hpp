/*!
  @file PDBWriter.hpp
  @brief pdb writer.

  write pdb atoms or chains into output stream.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-10-18 10:00
  @copyright Toru Niina 2016 on MIT License
*/
#ifndef COFFEE_MILL_PDB_WRITER
#define COFFEE_MILL_PDB_WRITER
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
    if(not ofs.good())
    {
        throw std::runtime_error("file open error: " + filename);
    }
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
    if(not ofs.good())
    {
        throw std::runtime_error("file open error: " + filename);
    }
    this->write(ofs, model, sty);
    ofs.close();
    return;
}

template<typename vectorT>
void PDBWriter<vectorT>::write(std::ostream& os,
        const std::vector<atom_type>& atoms) const
{
    for(auto iter = atoms.cbegin(); iter != atoms.cend(); ++iter)
    {
        os << *iter << std::endl;
    }
    return;
}

template<typename vectorT>
void PDBWriter<vectorT>::write(
        std::ostream& os, const std::vector<chain_type>& chains,
        const style sty) const
{
    std::size_t index = 1;
    for(auto chain = chains.cbegin(); chain != chains.cend(); ++chain)
    {
        if(sty == style::cafemol)
        {
            os << "<< protein " << index << std::endl;
        }

        for(auto resi = chain->cbegin(); resi != chain->cend(); ++resi)
        {
            for(auto iter = resi->cbegin(); iter != resi->cend(); ++iter)
            {
                os << *iter << std::endl;
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
