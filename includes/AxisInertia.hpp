#ifndef COFFEE_MILL_AXIS_INERTIA
#define COFFEE_MILL_AXIS_INERTIA
#include "mathematics/LinearAlgebra.hpp"
#include <vector>

namespace coffeemill
{
    class AxisInertia
    {
        public:

            AxisInertia(): calculated(false){}
            AxisInertia(const std::vector<std::pair<Realvec, double>>& data)
                : calculated(true), system(data)
            {
                calculate();
            }
            AxisInertia(const std::vector<Realvec>& data);
            ~AxisInertia(){}

            const Realvec get_axis(const int i);
            const double get_value(const int i);

            const std::array<Realvec, 3> get_axes();
            const std::array<double, 3> get_values();

            void calculate();
            Realvec get_CoM();

        private:

            bool calculated;
            std::array<Realvec, 3> axes;
            std::array<double, 3> values;
            std::vector<std::pair<Realvec, double>> system;//(position, mass)
    };

    AxisInertia::AxisInertia(const std::vector<Realvec>& data)
        : calculated(true), system(data.size())
    {
        auto sysiter = system.begin();
        for(auto iter = data.begin(); iter != data.end(); ++iter)
        {
            *sysiter = std::make_pair(*iter, 1e0);
            ++sysiter;
        }
        calculate();
    }

    const Realvec AxisInertia::get_axis(const int i)
    {
        if(i < 0 || 2 < 0)
            throw std::invalid_argument("AxisInertia: get_axis: out of range");

        if(!calculated)
            calculate();

        return axes.at(i);
    }

    const double AxisInertia::get_value(const int i)
    {
        if(i < 0 || 2 < 0)
            throw std::invalid_argument("AxisInertia: get_value: out of range");

        if(!calculated)
            calculate();

        return values.at(i);
    }

    const std::array<Realvec, 3> AxisInertia::get_axes()
    {
        if(!calculated)
            calculate();

        return axes;
    }

    const std::array<double, 3> AxisInertia::get_values()
    {
        if(!calculated)
            calculate();

        return values;
    }

    void AxisInertia::calculate()
    {
        Realvec center_of_mass(get_CoM());
        if(length(center_of_mass) > 1e-12)
        {
            for(auto iter = system.begin(); iter != system.end(); ++iter)
            {
                iter->first -= center_of_mass;
            }
        }// zeroing

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
        std::cout << "solved" << std::endl;

        axes.at(0) = solver.get_eigenvec(0);
        axes.at(1) = solver.get_eigenvec(1);
        axes.at(2) = solver.get_eigenvec(2);

        values.at(0) = solver.get_eigenvalue(0);
        values.at(1) = solver.get_eigenvalue(1);
        values.at(2) = solver.get_eigenvalue(2);

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
