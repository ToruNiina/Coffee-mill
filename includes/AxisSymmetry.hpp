#ifndef COFFEE_MILL_AXIS_SYMMETRY_NUCLEOSOME
#define COFFEE_MILL_AXIS_SYMMETRY_NUCLEOSOME
#include "mathematics/LinearAlgebra.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>

namespace coffeemill
{
    class AxisSymmetryNuc
    {
        public:
            enum USE_CHAIN
            {
                DNA,
                HISTONE,
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

            const Realvec get_CoM(){return CoM;};
            const Realvec get_axis(const USE_CHAIN condition);

        private:

            const Realvec calc_CoM(const std::vector<Realvec>& v);

            void calculate(const USE_CHAIN condition);
            std::size_t get_points_from_DNA(std::vector<Realvec>& v,
                                            const std::size_t index_);
            std::size_t get_points_from_HIS(std::vector<Realvec>& v,
                                            const std::size_t index_);

        private:

            Realvec axis;
            Realvec CoM;
            std::vector<Realvec> system;//(position, mass)
    };

    const Realvec AxisSymmetryNuc::get_axis(const USE_CHAIN condition)
    {
        calculate(condition);
        return axis;
    }

    void AxisSymmetryNuc::calculate(const USE_CHAIN condition)
    {
        // center of whole system?
        Realvec center_of_mass(calc_CoM(system));
        if(length(center_of_mass) > 1e-12)
        {
            for(auto iter = system.begin(); iter != system.end(); ++iter)
            {
                (*iter) -= center_of_mass;
            }
            center_of_mass = calc_CoM(system);
        }

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
        switch(condition)
        {
            case DNA:
                {
                std::size_t point_index(0);
                point_index = get_points_from_DNA(points_on_axis, point_index);
                break;
                }

            case HISTONE:
                {
                std::size_t point_index(0);
                point_index = get_points_from_HIS(points_on_axis, point_index);
                break;
                }

            case ALL:
                {
                std::size_t point_index(0);
                point_index = get_points_from_DNA(points_on_axis, point_index);
                point_index = get_points_from_HIS(points_on_axis, point_index);
                break;
                }

            default:
                throw std::logic_error("invalid enum value");
        }

        //dump
        std::ofstream dump("symaxis_dump.dat");
        for(auto iter = points_on_axis.begin(); iter != points_on_axis.end();
                ++iter)
        {
            dump << (*iter)[0] << " "
                 << (*iter)[1] << " " 
                 << (*iter)[2] << std::endl;
        }
        //end dump

        // obtain axis
        Realvec temp = points_on_axis.at(0);
        Realvec sum_fw(0e0);
        Realvec sum_rv(0e0);
        for(auto iter = points_on_axis.begin();
                 iter != points_on_axis.end(); ++iter)
        {
            if(dot_prod(temp, *iter) > 0e0)
            {
                sum_fw += *iter;
            }
            else
            {
                sum_rv += *iter;
            }
        }
        Realvec Forward = sum_fw / length(sum_fw);
        Realvec Reverse = sum_rv / length(sum_rv);
        if((dot_prod(Forward, Reverse) + 1e0) > 1e-12)
        {
            std::cout << "Warning: "
                      << "dot_product(Fw, Rv): " 
                      << dot_prod(Forward, Reverse)
                      << std::endl;
//             throw std::logic_error("internal error");
        }

        axis = Forward;

        return;
    }

    const Realvec AxisSymmetryNuc::calc_CoM(const std::vector<Realvec>& v)
    {
        std::vector<Realvec>& container = const_cast<std::vector<Realvec>&>(v);
        unsigned int index(0);
        Realvec sum_pos(0e0);
        for(auto iter = container.begin(); iter != container.end(); ++iter)
        {
            sum_pos += *iter;
            ++index;
        }
        return sum_pos / static_cast<double>(index);
    }

    std::size_t AxisSymmetryNuc::get_points_from_DNA(std::vector<Realvec>& v,
                                                     const std::size_t index_)
    {
        std::size_t index(index_);
        for(std::size_t i(0); i < 440; ++i)
        {
            Realvec temp1(system.at(index));
            Realvec temp2(system.at(440 + index));
            v.at(index) = (temp1 + temp2) / 2e0;
            ++index;
        }
        return index;
    }

    std::size_t AxisSymmetryNuc::get_points_from_HIS(std::vector<Realvec>& v,
                                                     const std::size_t index_)
    {
        std::size_t point_index(index_);
        // H3
        for(std::size_t index(0); index < 90; ++index)//0 <= i < 135
        {// remove 1 to 45
            Realvec temp1(system.at(925 + index));//881(+1) to +135
            Realvec temp2(system.at(1415 + index));//1371(+1) to +135
            v.at(point_index) = (temp1 + temp2) / 2e0;
            ++point_index;
        }

        // H4
        for(std::size_t index(0); index < 77; ++index)// 0<= i < 102
        {//remove 1 to 25
            Realvec temp1(system.at(1040 + index));//1016(+1) to +102
            Realvec temp2(system.at(1530 + index));//1506(+1) to +102
            v.at(point_index) = (temp1 + temp2) / 2e0;
            ++point_index;
        }

        // H2A.1
        for(std::size_t index(0); index < 104; ++index)// 0 <= i < 128
        {//remove 1 to 16, 121 to 128 -> 24 residues
            Realvec temp1(system.at(1133 + index));//1118(+1) to 1245
            Realvec temp2(system.at(1623 + index));//1608(+1) to 1735
            v.at(point_index) = (temp1 + temp2) / 2e0;
            ++point_index;
        }

        // H2B.2
        for(std::size_t index(0); index < 90; ++index)//0 <= i < 125
        {//remove 1 to 35
            Realvec temp1(system.at(1280 + index));//1246(+1) to +125
            Realvec temp2(system.at(1770 + index));//1736(+1) to +125
            v.at(point_index) = (temp1 + temp2) / 2e0;
            ++point_index;
        }
        return point_index;
    }


}
#endif//COFFEE_MILL_AXIS_INERTIA
