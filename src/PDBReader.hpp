#ifndef COFFEE_MILL_PURE_PDB_READER
#define COFFEE_MILL_PURE_PDB_READER
#include "PDBChain.hpp"

namespace coffeemill
{

class PDBReader
{
  public:

    PDBReader(){}
    PDBReader(const std::string& filename)
        : filename_(filename)
    {}
    ~PDBReader(){}

    void  read_file();

          std::string& filename()       {return filename_;}
    const std::string& filename() const {return filename_;}
          std::vector<std::shared_ptr<PDBAtom>>& atoms()       {return atoms_;}
    const std::vector<std::shared_ptr<PDBAtom>>& atoms() const {return atoms_;}

  private:

    std::string filename_;
    std::vector<std::shared_ptr<PDBAtom>> atoms_;
};

}

#endif//COFFEE_MILL_PURE_PDB_READER
