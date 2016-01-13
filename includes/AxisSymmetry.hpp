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
            AxisSymmetryNuc(const std::vector<Realvec>& data, const int DNAbeads = 880)
                : DNA_beads(DNAbeads / 2), system(data)
            {
                assert(data.size() == static_cast<std::size_t>(DNAbeads + 980));
            }
            ~AxisSymmetryNuc(){}

            void set_data(const std::vector<Realvec>& data, const int DNAbeads = 880)
            {
                assert(data.size() == static_cast<std::size_t>(DNAbeads + 980));
                DNA_beads = DNAbeads / 2;
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

            std::size_t DNA_beads; // length * 3 - 1
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

        std::vector<Realvec> points_on_axis(system.size() / 2);
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

        //this outputs midpoints. includes some zero cause tails are ignored
        std::ofstream dump("symaxis_axispoints_dump.dat");
        for(auto iter = points_on_axis.begin(); iter != points_on_axis.end();
                ++iter)
        {
            dump << (*iter)[0] << " "
                 << (*iter)[1] << " " 
                 << (*iter)[2] << std::endl;
        }
        dump.close();
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
        }

        axis = Forward;
        return;
    }

    const Realvec AxisSymmetryNuc::calc_CoM(const std::vector<Realvec>& v)
    {
        unsigned int index(0);
        Realvec sum_pos(0e0);
        for(auto iter = v.cbegin(); iter != v.cend(); ++iter)
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
        for(std::size_t i(0); i < DNA_beads; ++i)
        {
            Realvec temp1(system.at(index));
            Realvec temp2(system.at(DNA_beads + index));
            v.at(index) = (temp1 + temp2) * 0.5;
            ++index;
        }
        return index;
    }

    std::size_t AxisSymmetryNuc::get_points_from_HIS(std::vector<Realvec>& v,
                                                     const std::size_t index_)
    {
        std::size_t point_index(index_);
        // H3 0 <= i < 135
        for(std::size_t index(0); index < 90; ++index)
        {// remove 1 to 45
            Realvec temp1(system.at(DNA_beads * 2 + 45 + index));
            Realvec temp2(system.at(DNA_beads * 2 + 490 + 45 + index));
            v.at(point_index) = (temp1 + temp2) * 0.5;
            ++point_index;
        }

        // H4 0 <= i < 102
        for(std::size_t index(0); index < 77; ++index)
        {//remove 1 to 25
            Realvec temp1(system.at(DNA_beads * 2 + 135 + 25 + index));
            Realvec temp2(system.at(DNA_beads * 2 + 490 + 135 + 25 + index));
            v.at(point_index) = (temp1 + temp2) * 0.5; 
            ++point_index;
        }

        // H2A.1 0 <= i < 128
        for(std::size_t index(0); index < 104; ++index)
        {//remove 1 to 16, 121 to 128 -> 24 residues
            Realvec temp1(system.at(DNA_beads * 2 + 135 + 102 + 16 + index));
            Realvec temp2(system.at(DNA_beads * 2 + 490 + 135 + 102 + 16 + index));
            v.at(point_index) = (temp1 + temp2) * 0.5;
            ++point_index;
        }

        // H2B.2 0 <= i < 125
        for(std::size_t index(0); index < 90; ++index)
        {//remove 1 to 35
            Realvec temp1(system.at(DNA_beads * 2 + 135 + 102 + 128 + 35 + index));
            Realvec temp2(system.at(DNA_beads * 2 + 490 + 135 + 102 + 128 + 35 + index));
            v.at(point_index) = (temp1 + temp2) * 0.5;
            ++point_index;
        }
        return point_index;
    }
}
#endif//COFFEE_MILL_AXIS_INERTIA
