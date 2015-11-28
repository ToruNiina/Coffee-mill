#ifndef COFFEE_MILL_ABSTRACT_MODEL
#define COFFEE_MILL_ABSTRACT_MODEL
#include "AbstractChain.hpp"

namespace coffeemill
{
    class AbstractModel
    {
        public:
            typedef std::vector<AbstChnSptr>::iterator iterator;

        public:

            AbstractModel(){}
            AbstractModel(AbstChnSptr& chain)
                : chains{chain}, model_id(1)
            {}
            AbstractModel(AbstChnSptr& chain, const int id)
                : chains{chain}, model_id(id)
            {}
            AbstractModel(const std::vector<AbstChnSptr> model)
                : chains(model), model_id(1)
            {}
            AbstractModel(const std::vector<AbstChnSptr> model,
                          const int id)
                : chains(model), model_id(id)
            {}

            void set_modelID(const int id)
            {
                model_id = id;
            }

            bool empty() const {return chains.empty();}
            size_t size() const {return chains.size();}
            void push_back(AbstChnSptr& chain){chains.push_back(chain);}
            iterator begin(){return chains.begin();}
            iterator end(){return chains.end();}
            AbstChnSptr& at(const size_t i){return chains.at(i);}

        private:

            int model_id;
            std::vector<AbstChnSptr> chains;
    };
}

#endif//COFFEE_MILL_ABSTRACT_MODEL
