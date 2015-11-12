#ifndef COFFEE_MILL_RMSD_CALCULATOR
#define COFFEE_MILL_RMSD_CALCULATOR
#include <vector>
#include <iostream>
#include <cmath>
#include "SuperImposer.hpp"
#include "mathematics/linear_algebra.hpp"

namespace coffeemill
{
    class RMSDCalculator
    {
        public:
            RMSDCalculator(): calculated(false){}
            ~RMSDCalculator(){}

            void set_data(const std::vector<Realvec>& data1,
                          const std::vector<Realvec>& data2);
            void set_data2(const std::vector<Realvec>& data2);

            double get_RMSD();

        private:

            void move_to_zero();

            Realvec mean(std::vector<Realvec>& structure);

            Matrix3 get_R(const std::array<double, 4>& eigenV);

            Matrix4 get_B(const std::vector<Realvec>& rA,
                                  const std::vector<Realvec>& rB);

            std::array<double, 4> get_eigenvec(const Matrix4& B);

            double calc_RMSD(const std::vector<Realvec>& rA, 
                             const std::vector<Realvec>& rB);

        private:

            bool calculated;
            double rmsd_value;
            std::vector<Realvec> molecule1;
            std::vector<Realvec> molecule2;

    };

    void RMSDCalculator::set_data(const std::vector<Realvec>& data1,
                                  const std::vector<Realvec>& data2)
    {
        if(data1.size() != data2.size())
        {
            throw std::invalid_argument(
                "structures having different number of particles is unsupported");
        }
        molecule1 = data1;
        molecule2 = data2;
        calculated = false;
        return;
    }

    void RMSDCalculator::set_data2(const std::vector<Realvec>& data2)
    {
        if(molecule1.size() != data2.size())
        {
            throw std::invalid_argument(
                "structures having different number of particles is unsupported");
        }
        molecule2 = data2;
        calculated = false;
        return;
    }

    double RMSDCalculator::get_RMSD()
    {
        if(calculated)
            return rmsd_value;

        if(molecule1.empty() || molecule2.empty())
        {
            throw std::invalid_argument(
                    "RMSDCalculator does not have two structures");
        }

        SuperImposer imposer;
        imposer.set_data(molecule1, molecule2);
        imposer.superimpose();
        molecule1 = imposer.get_data1();
        molecule2 = imposer.get_data2();

        rmsd_value = calc_RMSD(molecule1, molecule2);
        calculated = true;
        return rmsd_value;
    }

    double RMSDCalculator::calc_RMSD(const std::vector<Realvec>& rA,
                                     const std::vector<Realvec>& rB)
    {
        int N(rA.size());
        double sigma(0e0);
        for(int i(0); i<N; ++i)
        {
            sigma += len_square(rB.at(i) - rA.at(i));
        }
        return std::sqrt(sigma / static_cast<double>(N));
    }

}

#endif //COFFEE_MILL_RMSD_CALCLATOR
