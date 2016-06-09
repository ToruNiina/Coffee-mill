#ifndef COFFEE_MILL_PDB_TO_CG_H
#define COFFEE_MILL_PDB_TO_CG_H
#include "pdb/CGModel.hpp"
#include "pdb/PDBChain.hpp"
#include "mathematics/LinearAlgebra.hpp"

namespace coffeemill
{
    class PDBtoCG
    {
        public:

            enum I_USE_ATOM_PROTEIN
            {
                alpha_carbon,
                beta_carbon,
                center_of_mass_side_chain
            };

        public:
            PDBtoCG(I_USE_ATOM_PROTEIN use_atom = alpha_carbon)
                : i_use_atom_protein(use_atom)
            {}
            ~PDBtoCG(){}

            void set_pdbdata(const std::vector<PDBChnSptr>& pdbdata)
            {
                pure_pdb_style = pdbdata;
            }

            std::vector<CGChnSptr>& get_cgpdb()
            {
                return cg_style_chain;
            }

            std::vector<PDBChnSptr>& get_purepdb()
            {
                return pure_pdb_style;
            }

            void convert();

            void output(const std::string& outfile);

        private:

            bool is_C_alpha(const std::string& atomname);
            void make_using_alpha_carbon();
            void make_using_beta_carbon();
            void make_using_centerofmass_sidechain();
            BeadSptr make_cg(AtomSptr& atom, Realvec pos, int imp_all, int imp_local);

        private:

            I_USE_ATOM_PROTEIN i_use_atom_protein;

            //datas
            std::vector<PDBChnSptr> pure_pdb_style;
            std::vector<CGChnSptr> cg_style_chain;
    };

    void PDBtoCG::convert()
    {
        if(pure_pdb_style.empty())
        {
            std::cout << "pdb file is not defined" << std::endl;
            return;
        }

        switch(i_use_atom_protein)
        {
            case alpha_carbon:
                make_using_alpha_carbon();
                break;

            case beta_carbon:
                make_using_beta_carbon();
                break;

            case center_of_mass_side_chain:
                make_using_centerofmass_sidechain();
                break;

            default:
                throw std::invalid_argument("invalid i_use_atom_protein");
        }

        return;
    }

    void PDBtoCG::output(const std::string& outfile)
    {
        std::ofstream outputfile(outfile);
        int counter(1);
        for(auto iter = cg_style_chain.begin(); iter != cg_style_chain.end();
            ++iter)
        {
            (*iter)->write_block(outputfile, counter);
            ++counter;
        }
        return;
    }

    void PDBtoCG::make_using_alpha_carbon()
    {
        int imp_all(0);
        for(std::vector<PDBChnSptr>::iterator iter = pure_pdb_style.begin();
            iter != pure_pdb_style.end(); ++iter)
        {
            bool filled(false);
            int counter(0);
            CGChnSptr chain(new CGChain);
            for(PDBChain::iterator aiter = (*iter)->begin();
                aiter != (*iter)->end(); ++aiter)
            {
                if(is_C_alpha((*aiter)->get_name()))
                {
                    ++counter;
                    Realvec pos((*aiter)->get_x(),
                                (*aiter)->get_y(),
                                (*aiter)->get_z());
                    BeadSptr bead(make_cg(*aiter, pos, imp_all, counter));
                    chain->push_back(bead);
                    if(!filled) filled = true;
                }
            }
            if(filled) cg_style_chain.push_back(chain);
            imp_all += counter;
        }
        return;
    }

    void PDBtoCG::make_using_beta_carbon()
    {
        std::cout << "beta carbon is not supported yet" << std::endl;
        return;
    }

    void PDBtoCG::make_using_centerofmass_sidechain()
    {
        std::cout << "center of mass of side chain is not supported yet"
                  << std::endl;
        return;
    }

    BeadSptr PDBtoCG::make_cg(AtomSptr& atom, Realvec pos, int imp_all, int imp_local)
    {
        BeadSptr bead(new CGBead);
        bead->set_headder("ATOM  ");//header
        bead->set_imp(imp_all + imp_local);//imp
        bead->set_beadname("CA");//bead
        bead->set_seq(atom->get_resName());//seq
        bead->set_chainID(atom->get_chainID());//ID
        bead->set_iResNum(atom->get_resSeq()); //resNum
        bead->set_coordx(pos[0]);
        bead->set_coordy(pos[1]);
        bead->set_coordz(pos[2]);

        return bead;
    }

    bool PDBtoCG::is_C_alpha(const std::string& atomname)
    {
        size_t found = atomname.find("CA");
        return !(found == std::string::npos);
    }
}

#endif //COFFEE_MILL_PDB_TO_CG_H
