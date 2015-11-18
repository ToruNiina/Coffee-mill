#ifndef COFFEE_MILL_PDB_CHAIN
#define COFFEE_MILL_PDB_CHAIN
#include <vector>
#include <boost/regex.hpp>
#include "PDBAtom.hpp"

namespace coffeemill
{
    class PDBChain
    {
    public:

        typedef std::vector<AtomSptr>::iterator iterator;

    public:

        PDBChain(): there_is_chain(false) {}
        ~PDBChain(){}

        void push_back(AtomSptr& atom);

        void read_block(std::ifstream& ifs);
        void write_block(std::ofstream& ofs);

        iterator begin()
        {
            return atoms.begin();
        }

        iterator end()
        {
            return atoms.end();
        }

        std::vector<AtomSptr>& get_chain()
        {
            return atoms;
        }

        char get_chainID()
        {
            return chainID;
        }

        std::string get_sequence();

        bool chain_exist() const 
        {
            return there_is_chain;
        }

    private:

        bool there_is_chain;
        char chainID;
        std::vector<AtomSptr> atoms;
    };

    void PDBChain::push_back(AtomSptr& atom)
    {
        if(atoms.empty())
        {
            there_is_chain = true;
            chainID = atom->get_chainID();
        }

        if(!atoms.empty() && atom->get_chainID() != chainID)
        {
            std::cout << "Error: push_back different chainID atom" << std::endl;
            throw std::invalid_argument("invalid Chain ID");
        }
        atoms.push_back(atom);
        return;
    }

    void PDBChain::read_block(std::ifstream& ifs)
    {
        while(!ifs.eof())
        {
            AtomSptr atom(new PDBAtom);
            PDBAtom::LINE_TYPE line_type(atom->getAtomLine(ifs));
            switch(line_type)
            {
            case PDBAtom::EMPTY:
                break;
            case PDBAtom::ATOM:
                there_is_chain = true;
                push_back(atom);
                break;
            case PDBAtom::TER:
                return;
            case PDBAtom::END:
                return;
            case PDBAtom::UNKNOWN:
                break;
            default:
                throw std::invalid_argument("unknown linetype");
            }
        }
        return;
    }
    void PDBChain::write_block(std::ofstream& ofs)
    {
        for(std::vector<AtomSptr>::iterator iter = atoms.begin();
            iter != atoms.end(); ++iter)
        {
            ofs << *(*iter) << std::endl;
        }
        ofs << "TER" << std::endl;
        return;
    }

    std::string PDBChain::get_sequence()
    {
        std::string return_seq;
        int current_seq(-1);
        for(std::vector<AtomSptr>::iterator iter = atoms.begin();
            iter != atoms.end(); ++iter)
        {
            if(current_seq == (*iter)->get_resSeq()) continue;
            else current_seq = (*iter)->get_resSeq();

            std::string seqbuf((*iter)->get_resName());
            boost::regex seqdna("D[ACGT]");
            if(boost::regex_search(seqbuf, seqdna))
            {
                size_t dpos(seqbuf.find('D'));
                return_seq += seqbuf[dpos+1];
            }
            else
            {
                seqbuf += ' ';
                return_seq += seqbuf;
            }
        }
        return return_seq;
    }

    typedef std::shared_ptr<PDBChain> PDBChnSptr;
}
#endif //COFFEE_MILL_PDB_CHAIN
