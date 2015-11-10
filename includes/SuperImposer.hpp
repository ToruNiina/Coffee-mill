#ifndef COFFEE_MILL_SUPER_IMPOSER
#define COFFEE_MILL_SUPER_IMPOSER
#include <iostream>
#include <vector>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Eigenvalues>

namespace coffeemill
{
    class SuperImposer
    {
        public:

            SuperImposer(): superimposed(false){}
            ~SuperImposer(){}

            void set_data(const std::vector<Eigen::Vector3d>& data1,
                          const std::vector<Eigen::Vector3d>& data2);
            void set_data1(const std::vector<Eigen::Vector3d>& data1);
            void set_data2(const std::vector<Eigen::Vector3d>& data2);

            void set_data_push(const std::vector<Eigen::Vector3d>& data);

            void superimpose();

            std::vector<Eigen::Vector3d>& get_data1();
            std::vector<Eigen::Vector3d>& get_data2();

        private:

            Eigen::Vector3d mean(std::vector<Eigen::Vector3d>& structure);
            void move_to_zero();
            Eigen::Matrix3d get_R(const Eigen::Vector4d& q);
            Eigen::Matrix4d get_B(const std::vector<Eigen::Vector3d>& rA,
                                  const std::vector<Eigen::Vector3d>& rB);
            Eigen::Vector4d get_eigenvec(const Eigen::Matrix4d& B);
            void rotate(const Eigen::Matrix3d& R);

        private:

            bool superimposed;
            std::vector<Eigen::Vector3d> structure1;
            std::vector<Eigen::Vector3d> structure2;
    };

    void SuperImposer::set_data(const std::vector<Eigen::Vector3d>& data1,
                  const std::vector<Eigen::Vector3d>& data2)
    {
        structure1 = data1;
        structure2 = data2;
        superimposed = false;
        return;
    }

    void SuperImposer::set_data1(const std::vector<Eigen::Vector3d>& data1)
    {
        structure1 = data1;
        superimposed = false;
        return;
    }

    void SuperImposer::set_data2(const std::vector<Eigen::Vector3d>& data2)
    {
        structure2 = data2;
        superimposed = false;
        return;
    }

    void SuperImposer::set_data_push(const std::vector<Eigen::Vector3d>& data)
    {
        structure2 = structure1;
        structure1 = data;
        superimposed = false;
        return;
    }

    std::vector<Eigen::Vector3d>& SuperImposer::get_data1()
    {
        if(!superimposed)
            std::cout << "Warning: data you got have not superimposed" << std::endl;
        return structure1;
    }

    std::vector<Eigen::Vector3d>& SuperImposer::get_data2()
    {
        if(!superimposed)
            std::cout << "Warning: data you got have not superimposed" << std::endl;
        return structure2;
    }

    void SuperImposer::superimpose()
    {
        if(superimposed) return;

        if(structure1.empty() || structure2.empty())
            throw std::invalid_argument(
                    "SuperImposer does not have two structures");

        if(structure1.size() != structure2.size())
            throw std::invalid_argument(
                    "SuperImposer has two structures that have different sizes");

        move_to_zero();

        std::vector<Eigen::Vector3d> rA(structure1.size());
        std::vector<Eigen::Vector3d> rB(structure2.size());

        for(size_t i(0); i<structure1.size(); ++i)
        {
            rA.at(i) = structure1.at(i) + structure2.at(i);
            rB.at(i) = structure2.at(i) - structure1.at(i);
        }

        Eigen::Matrix4d B(get_B(rA, rB));
        Eigen::Vector4d q = get_eigenvec(B);
        Eigen::Matrix3d R(get_R(q));

        rotate(R);
        superimposed = true;

        return;
    }

    void SuperImposer::move_to_zero()
    {
        Eigen::Vector3d mean1(mean(structure1));
        for(std::vector<Eigen::Vector3d>::iterator iter = structure1.begin();
            iter != structure1.end(); ++iter)
        {
            *iter = (*iter - mean1);
        }

        Eigen::Vector3d mean2(mean(structure2));
        for(std::vector<Eigen::Vector3d>::iterator iter = structure2.begin();
            iter != structure2.end(); ++iter)
        {
            *iter = (*iter - mean2);
        }

        return;
    }

    Eigen::Vector3d SuperImposer::mean(std::vector<Eigen::Vector3d>& structure)
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

    Eigen::Matrix4d
    SuperImposer::get_B(const std::vector<Eigen::Vector3d>& a,
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

    Eigen::Vector4d SuperImposer::get_eigenvec(const Eigen::Matrix4d& B)
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

    Eigen::Matrix3d SuperImposer::get_R(const Eigen::Vector4d& q)
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

    void SuperImposer::rotate(const Eigen::Matrix3d& R)
    {
        for(std::vector<Eigen::Vector3d>::iterator iter = structure1.begin();
            iter != structure1.end(); ++iter)
        {
            *iter = R * (*iter);
        }
        return;
    }

}//end namespace coffeemill

#endif//COFFEE_MILL_SUPER_IMPOSER
