#ifndef COFFEE_MILL_AXIS_SYMMETRY_NUCLEOSOME
#define COFFEE_MILL_AXIS_SYMMETRY_NUCLEOSOME
#include "mathematics/LinearAlgebra.hpp"
#include <vector>
#include <cassert>

namespace coffeemill
{
    class AxisSymmetryNuc
    {
        public:
            enum USE_CHAIN
            {
                DNA,
                HISTONE,//automatically truncated
                ALL,
            };

        public:

            AxisSymmetryNuc(){}
            AxisSymmetryNuc(const std::vector<Realvec>& data)
                : system(data)
            {
                assert(data.size() == 1860);
            }
            ~AxisSymmetryNuc(){}

            void set_data(const std::vector<Realvec>& data)
            {
                assert(data.size() == 1860);
                system = data;
            }

            const Realvec get_CoM();
            const Realvec get_axis(const USE_CHAIN condition);

        private:

            void calculate(const USE_CHAIN condition);

        private:

            Realvec axis;
            std::vector<Realvec> system;//(position, mass)
    };

    const Realvec AxisSymmetryNuc::get_axis(const USE_CHAIN condition)
    {
        calculate(condition);
        return axis;
    }

    void AxisSymmetryNuc::calculate(const USE_CHAIN condition)
    {
        Realvec center_of_mass(get_CoM());
        if(length(center_of_mass) > 1e-12)
        {
            for(auto iter = system.begin(); iter != system.end(); ++iter)
            {
                (*iter) -= center_of_mass;
            }
        }// consider the axis is on the center of mass

        /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
         * This class is very hardly coded for CG PDB files        *
         * of 1KX5. it may not work collectly on other condition.  *
         *                                                         *
         * 1KX5 (CG) PDB:                                          *
         * chain A DNA imp 1-440                                   *
         * chain B DNA imp 441-880                                 *
         * chain C H3  imp 881-1015                                *
         * chain D H4  imp 1016-1117                               *
         * chain E H2A imp 1118-1245                               *
         * chain F H2B imp 1246-1370                               *
         * chain G H3  imp 1371-1505                               *
         * chain H H4  imp 1506-1607                               *
         * chain I H2A imp 1608-1735                               *
         * chain J H2B imp 1736-1860                               *
         * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

        std::vector<Realvec> points_on_axis(930);
        if(condition == DNA)
        {
            for(std::size_t index(0); index < 440; ++index)
            {
                Realvec temp1(system.at(index));
                Realvec temp2(system.at(880 - index));
                points_on_axis.at(index) = (temp1 + temp2) / 2e0;
            }
        }
        else if(condition == HISTONE)
        {//TODO remove tail
            std::size_t point_index(0);

            // H3
            for(std::size_t index(0); index < 135; ++index)
            {
                Realvec temp1(system.at(881 + index));
                Realvec temp2(system.at(1371 + index));
                points_on_axis.at(point_index) = (temp1 + temp2) / 2e0;
                ++point_index;
            }

            // H4
            for(std::size_t index(0); index < 102; ++index)
            {
                Realvec temp1(system.at(1016 + index));
                Realvec temp2(system.at(1506 + index));
                points_on_axis.at(point_index) = (temp1 + temp2) / 2e0;
                ++point_index;
            }

            // H2A
            for(std::size_t index(0); index < 128; ++index)
            {
                Realvec temp1(system.at(1118 + index));
                Realvec temp2(system.at(1608 + index));
                points_on_axis.at(point_index) = (temp1 + temp2) / 2e0;
                ++point_index;
            }

            // H2B
            for(std::size_t index(0); index < 125; ++index)
            {
                Realvec temp1(system.at(1246 + index));
                Realvec temp2(system.at(1736 + index));
                points_on_axis.at(point_index) = (temp1 + temp2) / 2e0;
                ++point_index;
            }
        }
        else if(condition == ALL)
        {
            std::size_t point_index(0);
            for(std::size_t index(0); index < 440; ++index)
            {
                Realvec temp1(system.at(index));
                Realvec temp2(system.at(880 - index));
                points_on_axis.at(point_index) = (temp1 + temp2) / 2e0;
                ++point_index;
            } 

        }
        else
        {
            throw std::logic_error("invalid enum value");
        }

        // obtain axis
        for(auto iter = points_on_axis.begin();
                 iter != points_on_axis.end(); ++iter)
        {
            ;
        }

        return;
    }

    const Realvec AxisSymmetryNuc::get_CoM()
    {
        unsigned int index(0);
        Realvec sum_pos(0e0);
        for(auto iter = system.begin(); iter != system.end(); ++iter)
        {
            sum_pos += *iter;
            ++index;
        }
        return sum_pos / static_cast<double>(index);
    }

}
#endif//COFFEE_MILL_AXIS_INERTIA
