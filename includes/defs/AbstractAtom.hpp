#ifndef COFFEE_MILL_ABSTRACT_ATOM
#define COFFEE_MILL_ABSTRACT_ATOM
#include "../mathematics/LinearAlgebra.hpp"

namespace coffeemill
{
    class Atom
    {
        public:

            Atom(){}
            Atom(const double m, const Realvec& pos, const Realvec& vel,
                 const std::string& name)
                : mass(m), position(pos), velocity(vel), atom_name(name){}

            Realvec get_position() const
            {
                return position;
            }

            Realvec get_velocity() const
            {
                return velocity;
            }

            std::string get_name() const
            {
                return atom_name;
            }

            void set_position(const Realvec& pos)
            {
                position = pos;
            }

            void set_velocity (const Realvec& vel)
            {
                velocity = vel;
            }

            void set_name(const std::string& name)
            {
                atom_name = name;
            }

        private:

            double mass;
            Realvec position;
            Realvec velocity;
            std::string atom_name;
    };

    std::ostream& operator<<(std::ostream& os, const Atom& atom)
    {
        os << "position: " << atom.get_position() << std::endl;
        os << "velocity: " << atom.get_velocity() << std::endl;
        os << "name    : " << atom.get_name() << std::endl;
        return os;
    }


}

#endif //COFFEE_MILL_ABSTRACT_ATOM
