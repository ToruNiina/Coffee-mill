#ifndef COFFEE_MILL_CGPDB_MODEL
#define COFFEE_MILL_CGPDB_MODEL
#include "CGChain.hpp"

namespace coffeemill
{
    class CGModel
    {
        public:
            CGModel()
            {
                ;
            }
            ~CGModel()
            {
                ;
            }

            void read_file(std::ifstream& pdbfile);
            void write_file(std::ofstream& outfile)
            {//TODO
                std::cout << "not supported yet" << std::endl;
            }

            bool empty(){return chains.empty();}
            int size(){return chains.size();}
            CGChnSptr& at(const int i){return chains.at(i);}
            std::vector<CGChnSptr>& get_data(){return chains;}
            CGChnSptr& find(const char ID);
            int find_id(const char ID);

        private:

            int model_ID;
            int step;
            std::vector<CGChnSptr> chains;
    };

    void CGModel::read_file(std::ifstream& pdbfile)
    {
        if(!chains.empty())
        {
            throw std::invalid_argument("already read");
        }

        bool model_found(false);
        while(!pdbfile.eof())
        {
            std::string line;
            std::getline(pdbfile, line);

            if(line.substr(0,5) == "MODEL")
            {
                std::cout << "Info   : model found." << std::endl;
                model_found = true;
                std::istringstream ls(line);
                std::string temp;
                ls >> temp >> model_ID;
                ls >> temp >> step;
                std::cout << "Info   :" << line << std::endl;
            }

            if(line.substr(0,6) == "ENDMDL")
            {
                if(model_found)
                    std::cout << "Info   : end of model block found" << std::endl;
                else
                    std::cout << "Warning: model does not exist but end of model found"
                              << std::endl;
                return;
            }
            CGChnSptr chain(new CGChain);
            chain->read_block(pdbfile);
            chains.push_back(chain);
        }

        if(!model_found)
        {
            std::cout << "Warning: model description not found" << std::endl;
            return;
        }

        std::cout << "Warning: end of model not found" << std::endl;
        return;
    }

    CGChnSptr& CGModel::find(const char ID)
    {
        for(auto iter = chains.begin(); iter != chains.end(); ++iter)
        {
            if((*iter)->get_chainID() == ID)
                return *iter;
        }
        std::cout << "Warning: cannot find ID: " << ID << std::endl;
        return *(chains.end());
    }

    int CGModel::find_id(const char ID)
    {
        for(int i(0); i<chains.size(); ++i)
        {
            if(chains.at(i)->get_chainID() == ID)
                return i;
        }
        std::cout << "Warning: cannot find ID: " << ID << std::endl;
        return -1;
    }

    typedef std::shared_ptr<CGModel> CGMdlSptr;
}

#endif //COFFEE_MILL_CGPDB_MODEL
