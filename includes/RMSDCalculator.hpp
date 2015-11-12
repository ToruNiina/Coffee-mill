#ifndef COFFEE_MILL_RMSD_CALCULATOR
#define COFFEE_MILL_RMSD_CALCULATOR
#include <vector>
#include <complex>
#include <iostream>
#include <cmath>
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
                             const std::vector<Realvec>& rB,
                             const Matrix3& R);

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

        move_to_zero();

        std::vector<Realvec> rA(molecule1.size());
        std::vector<Realvec> rB(molecule2.size());

        for(int i(0); i<static_cast<int>(molecule1.size()); ++i)
        {
            rA.at(i) = molecule1.at(i) + molecule2.at(i);
            rB.at(i) = molecule2.at(i) - molecule1.at(i);
        }

        Matrix4 B = get_B(rA, rB);
        std::array<double, 4> q = get_eigenvec(B);
        Matrix3 R = get_R(q);
       
        rmsd_value = calc_RMSD(molecule1, molecule2, R);
        calculated = true;
        return rmsd_value;
    }

    void RMSDCalculator::move_to_zero()
    {
        Realvec mean1(mean(molecule1));
        for(std::vector<Realvec>::iterator iter = molecule1.begin();
            iter != molecule1.end(); ++iter)
        {
            *iter -= mean1;
        }

        Realvec mean2(mean(molecule2));
        for(std::vector<Realvec>::iterator iter = molecule2.begin();
            iter != molecule2.end(); ++iter)
        {
            *iter -= mean2;
        }

        return;
    }

    Realvec RMSDCalculator::mean(std::vector<Realvec>& structure)
    {//mass is undefined
        int num_particle(structure.size());
        Realvec sum(0e0, 0e0, 0e0);
        for(std::vector<Realvec>::iterator iter = structure.begin();
            iter != structure.end(); ++iter)
        {
            sum += *iter;
        }
        return (sum / static_cast<double>(num_particle));
    }

    double RMSDCalculator::calc_RMSD(const std::vector<Realvec>& rA,
                                     const std::vector<Realvec>& rB,
                                     const Matrix3& R)
    {
        int N(rA.size());
        double sigma(0e0);
        for(int i(0); i<N; ++i)
        {
            sigma += len_square(rB.at(i) - R * rA.at(i));
        }
        return std::sqrt(sigma / static_cast<double>(N));
    }

    Matrix3 RMSDCalculator::get_R(const std::array<double, 4>& q)
    {
        Matrix3 R;
        R(0,0) = 2e0*q[0]*q[0] + 2e0*q[1]*q[1] - 1e0;
        R(0,1) = 2e0*q[1]*q[2] - 2e0*q[0]*q[3];
        R(0,2) = 2e0*q[1]*q[3] + 2e0*q[0]*q[2];
        R(1,0) = 2e0*q[1]*q[2] + 2e0*q[0]*q[3];
        R(1,1) = 2e0*q[0]*q[0] + 2e0*q[2]*q[2] - 1e0;
        R(1,2) = 2e0*q[2]*q[3] - 2e0*q[0]*q[1];
        R(2,0) = 2e0*q[1]*q[3] - 2e0*q[0]*q[2];
        R(2,1) = 2e0*q[2]*q[3] + 2e0*q[0]*q[1];
        R(2,2) = 2e0*q[0]*q[0] + 2e0*q[3]*q[3] - 1e0;
        return R;
    }

    std::array<double, 4>
    RMSDCalculator::get_eigenvec(const Matrix4& B)
    {
        Jacobi44Solver solver(B);
        std::pair<double, std::array<double, 4> > min_pair
            = solver.get_mineigenpair();
        return min_pair.second;
    }

    Matrix4 RMSDCalculator::get_B(const std::vector<Realvec>& a,
                                  const std::vector<Realvec>& b)
    {
        if(a.size() != b.size())
            throw std::invalid_argument("cannot make matrix B");

        Matrix4 retval;

        int N(a.size());
        for(int i(0); i<N; ++i)
        {
            retval(0,0) += (b[i][0]*b[i][0]) + (b[i][1]*b[i][1]) + (b[i][2]*b[i][2]);
            retval(0,1) += (a[i][2]*b[i][1]) - (a[i][1]*b[i][2]);
            retval(0,2) += (a[i][0]*b[i][2]) - (a[i][2]*b[i][0]);
            retval(0,3) += (a[i][1]*b[i][0]) - (a[i][0]*b[i][1]);
            retval(1,1) += (b[i][0]*b[i][0]) + (a[i][1]*a[i][1]) + (a[i][2]*a[i][2]);
            retval(1,2) += (b[i][0]*b[i][1]) - (a[i][0]*a[i][1]);
            retval(1,3) += (b[i][0]*b[i][2]) - (a[i][0]*a[i][2]);
            retval(2,2) += (a[i][0]*a[i][0]) + (b[i][1]*b[i][1]) + (a[i][2]*a[i][2]);
            retval(2,3) += (b[i][1]*b[i][2]) - (a[i][1]*a[i][2]);
            retval(3,3) += (a[i][0]*a[i][0]) + (a[i][1]*a[i][1]) + (b[i][2]*b[i][2]);
        }

        retval(0,0) = retval(0,0) / static_cast<double>(N);
        retval(0,1) = retval(0,1) / static_cast<double>(N);
        retval(0,2) = retval(0,2) / static_cast<double>(N);
        retval(0,3) = retval(0,3) / static_cast<double>(N);
        retval(1,1) = retval(1,1) / static_cast<double>(N);
        retval(1,2) = retval(1,2) / static_cast<double>(N);
        retval(1,3) = retval(1,3) / static_cast<double>(N);
        retval(2,2) = retval(2,2) / static_cast<double>(N);
        retval(2,3) = retval(2,3) / static_cast<double>(N);
        retval(3,3) = retval(3,3) / static_cast<double>(N);

        retval(1,0) = retval(0,1);
        retval(2,0) = retval(0,2);
        retval(2,1) = retval(1,2);
        retval(3,0) = retval(0,3);
        retval(3,1) = retval(1,3);
        retval(3,2) = retval(2,3);//symmetric

        return retval;
    }

}

#endif //COFFEE_MILL_RMSD_CALCLATOR
