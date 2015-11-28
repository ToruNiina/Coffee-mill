#ifndef COFFEE_MILL_ABSTRACT_ATOM
#define COFFEE_MILL_ABSTRACT_ATOM
#include <memory>
#include <iomanip>
#include "../mathematics/LinearAlgebra.hpp"

namespace coffeemill
{
    class AbstractAtom
    {
        public:

            AbstractAtom(){}
            AbstractAtom(const int p_id,
                         const int r_id,
                         const Realvec& pos,
                         const std::string& residue,
                         const std::string& name)
                : particle_ID(p_id), residue_ID(r_id),
                  position(pos), residue_name(residue), atom_name(name)
            {}

            int get_particle_ID() const
            {
                return particle_ID;
            }

            int get_residue_ID() const
            {
                return residue_ID;
            }

            Realvec get_position() const
            {
                return position;
            }

            std::string get_residue_name() const
            {
                return residue_name;
            }

            std::string get_atom_name() const
            {
                return atom_name;
            }

            void set_ID(const int i)
            {
                particle_ID = i;
            }

            void set_position(const Realvec& pos)
            {
                position = pos;
            }

            void set_residue(const std::string& res)
            {
                residue_name = res;
            }

            void set_name(const std::string& name)
            {
                atom_name = name;
            }

        private:

            int particle_ID;
            int residue_ID;
            Realvec position;
            std::string atom_name;
            std::string residue_name;
    };

    std::ostream& operator<<(std::ostream& os, const AbstractAtom& atom)
    {
        os << "particle ID: " << std::setw(6) << atom.get_particle_ID() << std::endl;
        os << "atom       : " << std::setw(6) << atom.get_atom_name() << std::endl;
        os << "residue ID : " << std::setw(6) << atom.get_residue_ID() << std::endl;
        os << "residue    : " << std::setw(6) << atom.get_residue_name() << std::endl;
        os << "position   : " << atom.get_position() << std::endl;
        return os;
    }

    typedef std::shared_ptr<AbstractAtom> AbstAtomSptr;
}

#endif //COFFEE_MILL_ABSTRACT_ATOM
