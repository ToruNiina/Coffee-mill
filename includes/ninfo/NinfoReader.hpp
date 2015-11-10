#ifndef COFFEE_MILL_NINFO_READER
#define COFFEE_MILL_NINFO_READER
#include "NinfoFactory.hpp"

namespace coffeemill
{
    class NinfoReader
    {
    public:

        NinfoReader(std::string filename)
            : block_readed(false),
              ninfo_file(filename.c_str()),
              factory()
        {
            ;
        }

        NinfoReader()
            : block_readed(false),
              factory()
        {
            ;
        }

        ~NinfoReader(){}

        std::vector<BlockSptr>& read_file();
        std::vector<BlockSptr>& read_file(std::string filename);
        bool eof() const {return ninfo_file.eof();}
        void set_iunits(iUnits iunits);

    private:

        BlockType find_block(std::ifstream& file);
        BlockType get_blocktype(const std::string& line);

    private:

        bool block_readed;
        std::ifstream ninfo_file;
        std::vector<BlockSptr> blocks;
        NinfoBlockFactory factory;

    };

    std::vector<BlockSptr>& NinfoReader::read_file()
    {
        while(!ninfo_file.eof())
        {
            BlockType btype(find_block(ninfo_file));
            if(btype == NO_BLOCK)
            {
                std::cout << "Message: ninfo file reading is completed."
                          << std::endl;
                std::cout << "         there are " << blocks.size()
                          << " blocks in the file.";
                std::cout << std::endl;
                return blocks;
            }

            BlockSptr ninfo_block(factory.create(btype));
            ninfo_block->read_block(ninfo_file);
            blocks.push_back(ninfo_block);
            block_readed = true;
        }
        
        std::cout << "Warning: while loop is exitted with some strange way."
                  << std::endl;
        std::cout << "Message: ninfo file reading is completed." << std::endl;
        std::cout << "         there are " << blocks.size()
                  << " blocks in the file.";
        return blocks;
    }

    std::vector<BlockSptr>& NinfoReader::read_file(std::string filename)
    {
        if(ninfo_file.is_open())
        {
            std::cout << "Warning: ninfo file has been opened but" << std::endl;
            std::cout << "       : trying to open new file." << std::endl;
        }

        ninfo_file.open(filename, std::ifstream::in);

        while(!ninfo_file.eof())
        {
            BlockType btype(find_block(ninfo_file));
            if(btype == NO_BLOCK)
            {
                std::cout << "Message: ninfo file reading is completed."
                          << std::endl;
                std::cout << "         there are " << blocks.size()
                          << " blocks in the file.";
                std::cout << std::endl;
                return blocks;
            }

            BlockSptr ninfo_block(factory.create(btype));
            ninfo_block->read_block(ninfo_file);
            blocks.push_back(ninfo_block);
            block_readed = true;
        }
        
        std::cout << "Warning: while loop is exitted with some strange way."
                  << std::endl;
        std::cout << "Message: ninfo file reading is completed." << std::endl;
        std::cout << "         there are " << blocks.size()
                  << " blocks in the file.";
        return blocks;
    }

    BlockType NinfoReader::find_block(std::ifstream& file)
    {
        while(!file.eof())
        {
            bool unknown(false);
            std::string line;
            std::getline(file, line);

            if(line.empty()) continue;
            if(line[0] == '\x2a') continue;

            if(line.substr(0,4) == "<<<<")
            {
                return get_blocktype(line);
            }

            if(!unknown)
            {
                std::cout << "unknown type line: probably not supported yet."
                          << std::endl;
                std::cout << "this supports only bond, angl, dihd, contact,"
                          << " aicg13 and aicg2+dih"
                          << std::endl;
                unknown = true;
            }
            std::cout << line << std::endl;
        }

        if(!block_readed)
            throw std::invalid_argument("no block in this ninfo file.");
        else
            return NO_BLOCK;
    }
    
    BlockType NinfoReader::get_blocktype(const std::string& line)
    {
        std::istringstream ls(line);

        std::string head;
        ls >> head;
        if(head != "<<<<")
        {
            throw std::invalid_argument("argument is not block-initialize line");
        }

        if(line.substr(5,18) == "native bond length")
        {
            return N_BOND;
        }
        else if(line.substr(5,18) == "native bond angles")
        {
            return N_ANGL;
        }
        else if(line.substr(5,22) == "native dihedral angles")
        {
            return N_DIHD;
        }
        else if(line.substr(5,41) == "1-3 contacts with L_AICG2 or L_AICG2_PLUS")
        {
            return N_AICG13;
        }
        else if(line.substr(5,25) == "1-4 contacts with L_AICG2")
        {
            return N_AICG13;
        }
        else if(line.substr(5,30) == "1-4 contacts with L_AICG2_PLUS" ||
                line.substr(5,35) == "<<<< 1-4 contacts with L_AICG2_PLUS" )
        {
            return N_AICGDIH;
        }
        else if(line.substr(5,14) == "native contact")
        {
            return N_CONTACT;
        }
        else if(line.substr(5,15) == "native basepair")
        {
            return N_BASEPAIR;
        }
        else if(line.substr(5,16) == "native basestack")
        {
            return N_BASESTACK;
        }
        else
        {
            std::cout <<"unknown block: probably it is not supported yet." << std::endl;
            std::cout <<"this supports only bond, angl, dihd, contact, aicg13 and aicg2+dih"
                      << std::endl;
            std::cout << line << std::endl;
            throw std::invalid_argument("unknown block name");
        }
    }

    typedef std::shared_ptr<NinfoReader> ReaderSptr;
}
#endif //COFFEE_MILL_NINFO_READER
