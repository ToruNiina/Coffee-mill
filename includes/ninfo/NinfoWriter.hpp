#ifndef COFFEE_MILL_NINFO_WRITER
#define COFFEE_MILL_NINFO_WRITER
#include "NinfoBond.hpp"
#include "NinfoAngl.hpp"
#include "NinfoDihd.hpp"
#include "NinfoAicg13.hpp"
#include "NinfoAicg14.hpp"
#include "NinfoAicgdih.hpp"
#include "NinfoContact.hpp"
#include "NinfoBasepair.hpp"
#include "NinfoBasestack.hpp"

namespace coffeemill
{

    class NinfoWriter
    {
    private:
        std::string filename;
        std::ofstream ninfo_file;
        std::vector<BlockSptr> blocks;

    public:
        NinfoWriter(std::string _filename)
            : filename(_filename), ninfo_file(_filename.c_str())
        {}

        NinfoWriter(std::string _filename, std::vector<BlockSptr> data)
            : filename(_filename), ninfo_file(_filename.c_str()), blocks(data)
        {}

        void push_back(BlockSptr block)
        {
            if(!blocks.empty())
            {
                //for each blocks which is already included
                for(std::vector<BlockSptr>::iterator iter = blocks.begin();
                    iter != blocks.end(); ++iter)
                {
                    if(block->get_BlockType() == (*iter)->get_BlockType())
                    {
                        std::cout << "Warning: trying to push_back block that already exists.";
                        std::cout << std::endl;
                        return;
                    }
                }
            }
            blocks.push_back(block);
            return;
        }

        void write();

        std::string get_filename() const
        {
            return filename;
        }

    };

    void NinfoWriter::write()
    {
        if(!ninfo_file)
        {
            std::cout << "File Open Error: Writer fails opening output ninfo file." << std::endl;
            std::cout << "Filename: " << filename << std::endl;
            return;
        }

        for(std::vector<BlockSptr>::iterator iter = blocks.begin(); iter != blocks.end(); ++iter)
        {
            (*iter)->write_block(ninfo_file);
        }
        return;
    }


    typedef std::shared_ptr<NinfoWriter> WriterSptr;
}
#endif //COFFEE_MILL_NINFO_WRITER
