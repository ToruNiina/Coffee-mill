#ifndef COFFEE_MILL_CG_CHAIN
#define COFFEE_MILL_CG_CHAIN
#include <vector>
#include <algorithm>
#include <boost/regex.hpp>
#include "../mathematics/LinearAlgebra.hpp"
#include "CGBeads.hpp"
#include "ProteinSeq.hpp"

namespace coffeemill
{
    class CGChain
    {
    public:
        typedef std::vector<BeadSptr>::iterator iterator;

    public:
        CGChain(): there_is_chain(false){}

        CGChain(const BeadSptr& bead)
            : there_is_chain(true), chainID(bead->get_chainID()), seqmap()
        {
            residue.push_back(bead);
        }

        CGChain(std::vector<BeadSptr>& beads)
            : there_is_chain(true), chainID(beads.at(0)->get_chainID()),
              residue(beads), seqmap()
        {
            ;
        }

        ~CGChain(){}

        void push_back(BeadSptr& bead);

        void read_block(std::ifstream& ifs);
        void write_block(std::ofstream& ofs);

        std::vector<BeadSptr> get_chain(){return residue;}
        std::vector<Realvec> get_coord();

        void sort_imp();

        size_t size() const {return residue.size();}
        bool empty() const {return residue.empty();}
        iterator begin() {return residue.begin();}
        iterator end() {return residue.end();}
        BeadSptr& at(size_t i) {return residue.at(i);}

        char get_chainID() const {return chainID;}
        int get_iunit() const {return iunit;}
        int get_ResNum() const {return (*(residue.end()-1))->get_iResNum();}
        std::string get_sequence() const {return sequence;}
        MOLECULE_TYPE get_moltype() const {return mol_type;}
        bool chain_exist() const {return there_is_chain;}

    private:

        void read_sequence();

    private:

        bool there_is_chain;
        MOLECULE_TYPE mol_type;
        char chainID;
        int iunit;
        std::string sequence;
        std::vector<BeadSptr> residue;
        ProteinSeq seqmap;

    };

    void CGChain::push_back(BeadSptr& bead)
    {
        if(!there_is_chain)
        {
            there_is_chain = true;
            chainID = bead->get_chainID();
        }

        if(bead->get_chainID() != chainID)
        {
            std::cout << "Error  : trying to push_back bead having different chainID"
                      << std::endl;
            throw std::invalid_argument("invalid chainID");
        }
        residue.push_back(bead);
        return;
    }

    void CGChain::read_block(std::ifstream& ifs)
    {
        bool block_found(false);
        bool read_id(false);
        while(!ifs.eof())
        {
            std::string line;
            std::getline(ifs, line);

            if(line.substr(0,6) == "ATOM  " ||
               line.substr(0,6) == "HEATOM")
            {
                BeadSptr bead(new CGBead);
                bead->get_line(line);
                if(!read_id)
                {
                    chainID = bead->get_chainID();
                    read_id = true;
                }
                push_back(bead);
                continue;
            }

            if(line.substr(0,2) == "<<")
            {
                if(block_found)
                    throw std::invalid_argument(
                            "CG PDB block is not closed correctly");
                std::cout << "Info   : found block: " << line << std::endl;
                block_found = true;
                continue;
            }

            if(line.substr(0,2) == ">>")
            {
                read_sequence();
                return;
            }

            if(line.substr(0,6) == "ENDMDL")
            {
                if(block_found)
                {
                    std::cout << "Info   : ENDMDL found." << std::endl;
                    throw std::invalid_argument(
                            "CG PDB block is not closed correctly");
                }
            }

            if(line.substr(0,5) == "MODEL")
            {
                if(block_found)
                {
                    std::cout << "Info   : MODEL found." << std::endl;
                    throw std::invalid_argument(
                            "CG PDB block is not closed correctly");
                }
            }
            std::cout << "unknown line: " << line << std::endl;
        }

        if(block_found)
        {
            std::cout << "Warning: cannot find end of block >>" << std::endl;
            throw std::invalid_argument("CG PDB block does not close correctly");
        }
        read_sequence();
        return;
    }

    void CGChain::sort_imp()
    {
        std::sort(residue.begin(), residue.end(), less_imp);
    }

    void CGChain::write_block(std::ofstream& ofs)
    {
        ofs << "<< protein_ " << std::endl;
        for(std::vector<BeadSptr>::iterator iter = residue.begin();
            iter != residue.end(); ++iter)
        {
            ofs << *(*iter) << std::endl;
        }
        ofs << ">>" << std::endl;
        return;
    }

    void CGChain::read_sequence()
    {
        if(!sequence.empty())
        {
            std::cout << "Warning: trying to re-read sequence. do nothing."
                      << std::endl;
            return;
        }

        int current_resseq(-1);
        for(std::vector<BeadSptr>::iterator iter = residue.begin();
            iter != residue.end(); ++iter)
        {
            boost::regex seqdna("D[ACGT]");
            std::string seqname((*iter)->get_seq());

            if(current_resseq == (*iter)->get_iResNum()) continue;
            else current_resseq = (*iter)->get_iResNum();

            if(boost::regex_search(seqname, seqdna))
            {//DNA
                mol_type = DNA;
                size_t dpos(seqname.find('D'));
                sequence += seqname[dpos+1];
            }
            else if(seqmap.find(seqname))
            {//protein
                mol_type = PROTEIN;
                sequence += seqmap.pseq_3to1(seqname);
            }
            else
            {//unknown. RNA?
                mol_type = UNKNOWN;
                std::cout << "Warning: Unknown sequence: " << seqname << std::endl;
            }
        }
        return;
    }

    std::vector<Realvec> CGChain::get_coord()
    {
        std::vector<Realvec> retval;
        for(auto iter = residue.begin(); iter != residue.end(); ++iter)
        {
            retval.emplace_back(Realvec((*iter)->get_coordx(), 
                                        (*iter)->get_coordy(),
                                        (*iter)->get_coordz()));
        }
        return retval;
    }

    typedef std::shared_ptr<CGChain> CGChnSptr;
}

#endif //COFFEE_MILL_CG_CHAIN
