#ifndef COFFEE_MILL_RMSD_CALCULATOR
#define COFFEE_MILL_RMSD_CALCULATOR
#include <vector>
#include <complex>
#include <iostream>
#include <cmath>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigenvalues>

namespace coffeemill
{
    class RMSDCalculator
    {
        public:
            RMSDCalculator(): calculated(false){}
            ~RMSDCalculator(){}

            void set_data(const std::vector<Eigen::Vector3d>& data1,
                          const std::vector<Eigen::Vector3d>& data2);
            void set_data2(const std::vector<Eigen::Vector3d>& data2);

            double get_RMSD();

        private:

            void move_to_zero();

            Eigen::Vector3d mean(std::vector<Eigen::Vector3d>& structure);

            Eigen::Matrix3d get_R(const Eigen::Vector4d& eigenV);

            Eigen::Matrix4d get_B(const std::vector<Eigen::Vector3d>& rA,
                                  const std::vector<Eigen::Vector3d>& rB);

            Eigen::Vector4d get_eigenvec(const Eigen::Matrix4d& B);

            double calc_RMSD(const std::vector<Eigen::Vector3d>& rA, 
                             const std::vector<Eigen::Vector3d>& rB,
                             const Eigen::Matrix3d& R);

        private:

            bool calculated;
            double rmsd_value;
            std::vector<Eigen::Vector3d> molecule1;
            std::vector<Eigen::Vector3d> molecule2;

    };

    void RMSDCalculator::set_data(const std::vector<Eigen::Vector3d>& data1,
                                  const std::vector<Eigen::Vector3d>& data2)
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

    void RMSDCalculator::set_data2(const std::vector<Eigen::Vector3d>& data2)
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

        std::vector<Eigen::Vector3d> rA(molecule1.size());
        std::vector<Eigen::Vector3d> rB(molecule2.size());

        for(int i(0); i<static_cast<int>(molecule1.size()); ++i)
        {
            rA.at(i) = molecule1.at(i) + molecule2.at(i);
            rB.at(i) = molecule2.at(i) - molecule1.at(i);
        }

        Eigen::Matrix4d B = get_B(rA, rB);
        Eigen::Vector4d q = get_eigenvec(B);
        Eigen::Matrix3d R = get_R(q);
       
        rmsd_value = calc_RMSD(molecule1, molecule2, R);
        calculated = true;
        return rmsd_value;
    }

    void RMSDCalculator::move_to_zero()
    {
        Eigen::Vector3d mean1(mean(molecule1));
        for(std::vector<Eigen::Vector3d>::iterator iter = molecule1.begin();
            iter != molecule1.end(); ++iter)
        {
            *iter = (*iter - mean1);
        }

        Eigen::Vector3d mean2(mean(molecule2));
        for(std::vector<Eigen::Vector3d>::iterator iter = molecule2.begin();
            iter != molecule2.end(); ++iter)
        {
            *iter = (*iter - mean2);
        }

        return;
    }

    Eigen::Vector3d RMSDCalculator::mean(std::vector<Eigen::Vector3d>& structure)
    {//mass is undefined
        int num_particle(structure.size());
        Eigen::Vector3d sum(0e0, 0e0, 0e0);
        for(std::vector<Eigen::Vector3d>::iterator iter = structure.begin();
            iter != structure.end(); ++iter)
        {
            sum += *iter;
        }
        return (sum / static_cast<double>(num_particle));
    }

    double RMSDCalculator::calc_RMSD(const std::vector<Eigen::Vector3d>& rA,
                                     const std::vector<Eigen::Vector3d>& rB,
                                     const Eigen::Matrix3d& R)
    {
        int N(rA.size());
        double sigma(0e0);
        for(int i(0); i<N; ++i)
        {
            sigma += (rB.at(i) - R * rA.at(i)).dot(rB.at(i) - R * rA.at(i));
        }
        return std::sqrt(sigma / static_cast<double>(N));
    }

    Eigen::Matrix3d RMSDCalculator::get_R(const Eigen::Vector4d& q)
    {
        Eigen::Matrix3d R = Eigen::MatrixXd::Zero(3,3);
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

    Eigen::Vector4d
    RMSDCalculator::get_eigenvec(const Eigen::Matrix4d& B)
    {
        Eigen::EigenSolver<Eigen::Matrix4d> solver(B);
        std::vector<std::complex<double> > eigenval(4);
        eigenval.at(0) = solver.eigenvalues()(0);
        eigenval.at(1) = solver.eigenvalues()(1);
        eigenval.at(2) = solver.eigenvalues()(2);
        eigenval.at(3) = solver.eigenvalues()(3);

        int index(0);
        double min(real(eigenval.at(0)));
        if(imag(eigenval.at(0)) != 0e0)
            std::cout << "Warning: B has complex eigenvalue 0. imag: " 
                      << imag(eigenval.at(0));

        for(int i(1); i<4; ++i)
        {
            if(imag(eigenval.at(i)) != 0e0)
            {
                std::cout << "Warning: B has complex eigenvalue " << i
                          << " imag: " << imag(eigenval.at(i)) << std::endl;
            }

            if(real(eigenval.at(i)) < min)
            {
                min = real(eigenval.at(i));
                index = i;
            }
        }

        Eigen::Vector4cd col = solver.eigenvectors().col(index);
        Eigen::Vector4d retval(real(col[0]), real(col[1]), real(col[2]), real(col[3]));

        return retval;
    }

    Eigen::Matrix4d
    RMSDCalculator::get_B(const std::vector<Eigen::Vector3d>& a,
                          const std::vector<Eigen::Vector3d>& b)
    {
        if(a.size() != b.size())
            throw std::invalid_argument("cannot make matrix B");

        Eigen::Matrix4d retval = Eigen::MatrixXd::Zero(4, 4);

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
