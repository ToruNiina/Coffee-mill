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
                : pdbfile(filename)
            {
                if(pdbfile.fail())
                {
                    std::cout << "filename: " << filename << std::endl;
                    throw std::invalid_argument("file open error");
                }
            }

            ~PDBReader()
            {
                pdbfile.close();
            }

            void read_file();
            void read_file(const std::string& filename);

            std::vector<PDBChnSptr>& get_chains()
            {
                return chains;
            }
                
        private:

            std::ifstream pdbfile;
            std::vector<PDBChnSptr> chains;
    
    };

    void PDBReader::read_file(const std::string& filename)
    {
        if(pdbfile.is_open())
        {
            std::cout << "file already open" << std::endl;
        }
        else
        {
            pdbfile.open(filename);
        }

        if(pdbfile.fail())
        {
            std::cout << "filename: " << filename << std::endl;
            throw std::invalid_argument("file open error");
        }

        read_file();
        return;
    }

    void PDBReader::read_file()
    {
        while(!pdbfile.eof())
        {
            PDBChnSptr chain(new PDBChain);
            chain->read_block(pdbfile);
            chains.push_back(chain);
        }
        return;
    }

}

#endif//COFFEE_MILL_PURE_PDB_READER
