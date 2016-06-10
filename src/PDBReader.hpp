/*!
  @file PDBReader.hpp
  @brief definition of a class that reads pdb atom.

  read pdb file and store the information as std::vector of shared_ptr of PDBAtom
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-10 13:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_PURE_PDB_READER
#define COFFEE_MILL_PURE_PDB_READER
#include "PDBChain.hpp"

namespace coffeemill
{

//! PDBReader class
/*!
 * read pdb file and store the data as vector of shared_ptr of PDBAtom.
 * and also can parse the vector of PDBAtom into vector of PDBChains.
 */
class PDBReader
{
  public:

    //! ctor.
    PDBReader(){}
    //! ctor with the name of the file to read.
    explicit PDBReader(const std::string& filename)
        : filename_(filename)
    {}
    //! dtor.
    ~PDBReader(){}

    //! read the file.
    void  read();
    //! parse the data into chains.
    std::vector<PDBChain> parse();

    //! filename
          std::string& filename()       {return filename_;}
    //! filename
    const std::string& filename() const {return filename_;}
    //! data
          std::vector<std::shared_ptr<PDBAtom>>& atoms()       {return atoms_;}
    //! data
    const std::vector<std::shared_ptr<PDBAtom>>& atoms() const {return atoms_;}

  private:

    std::string filename_; //!< the name of the file to read
    std::vector<std::shared_ptr<PDBAtom>> atoms_;//!< data in the file
};


}

#endif//COFFEE_MILL_PURE_PDB_READER
