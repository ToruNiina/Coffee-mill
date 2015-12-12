#ifndef COFFEE_MILL_AXIS_INERTIA
#define COFFEE_MILL_AXIS_INERTIA
#include "mathematics/LinearAlgebra.hpp"

namespace coffeemill
{
    class AxisInertia
    {
        public:

            AxisInertia(): calculated(false){}
            AxisInertia(const std::vector<std::pair<Realvec, double>>)
                : calculated(true)
            {
                calculate();
            }
            ~AxisInertia(){}

            const Realvec get_axis(const int i);

        private:

            void calculate();
            Realvec get_CoM();

        private:

            bool calculated;
            std::array<Realvec, 3> axises;
            std::vector<std::pair<Realvec, double>> system;//(position, mass)
    };

    const Realvec AxisInertia::get_axis(const int i)
    {
        if(i < 0 || 2 < 0)
            throw std::invalid_argument("out of range");

        if(!calculated)
            calculate();

        return axises.at(i);
    }

    void AxisInertia::calculate()
    {
        Realvec center_of_mass(get_CoM());
        Matrix3 Inertia(0e0);

        for(auto iter = system.begin(); iter != system.end(); ++iter)
        {
            Inertia(0,0) += iter->second * (iter->first[1] * iter->first[1] + 
                                            iter->first[2] * iter->first[2]);
            Inertia(1,1) += iter->second * (iter->first[0] * iter->first[0] + 
                                            iter->first[2] * iter->first[2]);
            Inertia(2,2) += iter->second * (iter->first[0] * iter->first[0] + 
                                            iter->first[1] * iter->first[1]);

            Inertia(0,1) -= iter->second * iter->first[0] * iter->first[1];
            Inertia(0,2) -= iter->second * iter->first[2] * iter->first[0];
            Inertia(1,2) -= iter->second * iter->first[1] * iter->first[2];
        }
        Inertia(1,0) = Inertia(0,1);
        Inertia(2,0) = Inertia(0,2);
        Inertia(2,1) = Inertia(1,2);

        JacobiSolver<3> solver(Inertia);

        axises.at(0) = solver.get_eigenvec(0);
        axises.at(1) = solver.get_eigenvec(1);
        axises.at(2) = solver.get_eigenvec(2);

        calculated = true;
        return;
    }

    Realvec AxisInertia::get_CoM()
    {
        double sum_mass(0e0);
        Realvec sum_pos(0e0);
        for(auto iter = system.begin(); iter != system.end(); ++iter)
        {
            sum_pos += iter->second * (iter->first);
            sum_mass += iter->second;
        }
        return sum_pos / sum_mass;
    }

}
#endif//COFFEE_MILL_AXIS_INERTIA
