#ifndef COFFEE_MILL_CGPDB_READER
#define COFFEE_MILL_CGPDB_READER
#include "CGModel.hpp"

namespace coffeemill
{
    class CGReader
    {
        public:
            CGReader()
            {
                ;
            }

            CGReader(const std::string& filename)
                : cgfile(filename)
            {
                if(cgfile.fail())
                {
                    std::cout << "filename : " << filename << std::endl;
                    throw std::invalid_argument("file open error");
                }
            }

            ~CGReader()
            {
                cgfile.close();
            }

            void read_file();
            void read_file(const std::string& filename);

            CGMdlSptr& get_model(const int i)
            {
                return models.at(i);
            }

            int get_model_num() const
            {
                return models.size();
            }

        private:

            std::ifstream cgfile;
            std::vector<CGMdlSptr> models;
    };

    void CGReader::read_file(const std::string& filename)
    {
        if(cgfile.is_open())
        {
            std::cout << "file already open" << std::endl;
        }
        else
        {
            cgfile.open(filename);
        }

        if(cgfile.fail())
        {
            std::cout << "filename : " << filename << std::endl;
            throw std::invalid_argument("file open error");
        }

        read_file();
        return;
    }

    void CGReader::read_file()
    {
        while(!cgfile.eof())
        {
            CGMdlSptr temp(new CGModel);

            temp->read_file(cgfile);

            if(temp->empty()) break;

            models.push_back(temp);
        }
        return;
    }

}

#endif //COFFEE_MILL_CGPDB_READER
