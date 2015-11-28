#ifndef COFFEE_MILL_ABSTRACT_CHAIN
#define COFFEE_MILL_ABSTRACT_CHAIN
#include <vector>
#include "Defs.hpp"
#include "AbstractAtom.hpp"

namespace coffeemill
{
    class AbstractChain
    {
        public:

            typedef std::vector<AbstAtomSptr>::iterator iterator;

        public:

            AbstractChain(){}
            AbstractChain(AbstAtomSptr& atom)
                : atoms{atom}
            {}
            AbstractChain(const std::vector<AbstAtomSptr>& chain)
                : atoms(chain)
            {}
            AbstractChain(const std::vector<AbstAtomSptr>& chain,
                          const MOLECULE_TYPE type)
                : atoms(chain), moltype(type)
            {}
            ~AbstractChain(){}

            MOLECULE_TYPE get_moltype() const {return moltype;}
            char get_chain_id() const {return chain_ID;}

            size_t size() const {return atoms.size();}
            bool empty() const {return atoms.empty();}
            void push_back(AbstAtomSptr& atom){atoms.push_back(atom);}
            iterator begin(){return atoms.begin();}
            iterator end(){return atoms.end();}
            AbstAtomSptr& at(const size_t i){return atoms.at(i);}

        private:

            MOLECULE_TYPE moltype;
            char chain_ID;
            std::vector<AbstAtomSptr> atoms;
    };
    
    typedef std::shared_ptr<AbstractChain> AbstChnSptr;
}

#endif//COFFEE_MILL_ABSTRACT_CHAIN
