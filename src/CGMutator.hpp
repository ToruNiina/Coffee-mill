#ifndef COFFEE_MILL_CG_MUTATOR
#define COFFEE_MILL_CG_MUTATOR
#include "pdb/CGChain.hpp"

namespace coffeemill
{
    class CGMutator
    {
    public:

        CGMutator(CGChnSptr chain)
            : input(chain), seqmap() {}
        CGMutator(CGChnSptr chain, std::string seq)
            : mutated_seq(seq), input(chain), seqmap() {}
        ~CGMutator(){}

        void set_seq(std::string seq){mutated_seq = seq;}
        std::string get_seq() const {return mutated_seq;}

        void mutate();
        void output(std::ofstream& ofs){mutated->write_block(ofs);}

    private:

        void no_mut(std::vector<BeadSptr>::iterator& iter,
                    std::vector<BeadSptr>::iterator& end);
        void mut_DNA(const char base,
                     std::vector<BeadSptr>::iterator& iter,
                     std::vector<BeadSptr>::iterator& end);
        void mut_PRO(const char aacode,
                    std::vector<BeadSptr>::iterator& iter,
                    std::vector<BeadSptr>::iterator& end);

    private:

        std::string mutated_seq;
        CGChnSptr input;
        CGChnSptr mutated;
        ProteinSeq seqmap;

    };

    void CGMutator::mutate()
    {
        if(mutated_seq.empty())
        {
            std::cout << "CGMutator has no input sequence" << std::endl;
            throw std::invalid_argument("invalid sequence");
        }

        std::vector<BeadSptr> chain(input->get_chain());

        if(static_cast<int>(mutated_seq.size()) != input->get_ResNum())
        {
            std::cout << "mut_seq: " << mutated_seq.size() << std::endl;
            std::cout << "chain  : " << chain.size() << std::endl;
            std::cout << "mut    : " << mutated_seq << std::endl;
            std::cout << "input  : " << input->get_sequence() << std::endl;
            
            throw std::invalid_argument(
                    "length of sequene and chain size is not equal");
        }

        std::vector<BeadSptr>::iterator iter = chain.begin();
        std::vector<BeadSptr>::iterator end = chain.end();
        MOLECULE_TYPE mol_type = input->get_moltype();

        if(mol_type == DNA)
        {
            for(size_t current_seq(0); current_seq < mutated_seq.size();
                    ++current_seq)
            {
                char mut_residue = mutated_seq[current_seq];
                switch(mut_residue)
                {
                    case '=':
                        no_mut(iter, end);
                        break;
                    default:
                        mut_DNA(mut_residue, iter, end);
                        break;
                }
            }
        }
        else if(mol_type == PROTEIN)
        {
            for(size_t current_seq(0); current_seq < mutated_seq.size();
                    ++current_seq)
            {
                char mut_residue = mutated_seq[current_seq];
                switch(mut_residue)
                {
                    case '=':
                        no_mut(iter, end);
                        break;
                    default:
                        mut_PRO(mut_residue, iter, end);
                        break;
                }
            }           
        }

        mutated = std::shared_ptr<CGChain>(new CGChain(chain));

        return;
    }

    void CGMutator::no_mut(std::vector<BeadSptr>::iterator& iter,
                           std::vector<BeadSptr>::iterator& end)
    {
        int residue_number((*iter)->get_iResNum());
        while(true)
        {
            if(iter == end) return;

            if((*iter)->get_iResNum() == residue_number)
            {
                if(iter != end) ++iter;
            }else{
                return;
            }
        }
    }

    void CGMutator::mut_DNA(const char base,
                            std::vector<BeadSptr>::iterator& iter,
                            std::vector<BeadSptr>::iterator& end)
    {
        std::string mutres;
        switch(base)
        {
            case 'A':
                mutres = "DA";
                break;
            case 'C':
                mutres = "DC";
                break;
            case 'G':
                mutres = "DG";
                break;
            case 'T':
                mutres = "DT";
                break;
            default:
                throw std::invalid_argument("mut_DNA: unknown base");
        }

        int residue_number((*iter)->get_iResNum());
        while(true)
        {
            if(iter == end) return;

            if((*iter)->get_iResNum() == residue_number)
            {
                (*iter)->set_seq(mutres);
                if(iter != end) ++iter;
            }else{
                return;
            }
        }
    }

    void CGMutator::mut_PRO(const char aacode,
                            std::vector<BeadSptr>::iterator& iter,
                            std::vector<BeadSptr>::iterator& end)
    {
        std::string mutres(seqmap.pseq_1to3(aacode));

        int residue_number((*iter)->get_iResNum());
        while(true)
        {
            if(iter == end) return;

            if((*iter)->get_iResNum() == residue_number)
            {
                (*iter)->set_seq(mutres);
                if(iter != end) ++iter;
            }else{
                return;
            }
        }
    }
}
#endif //COFFEE_MILL_CG_MUTATOR
