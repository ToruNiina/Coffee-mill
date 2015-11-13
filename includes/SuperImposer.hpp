#ifndef COFFEE_MILL_SUPER_IMPOSER
#define COFFEE_MILL_SUPER_IMPOSER
#include <iostream>
#include <vector>
#include "mathematics/linear_algebra.hpp"

namespace coffeemill
{
    class SuperImposer
    {
        public:

            SuperImposer(): superimposed(false){}
            ~SuperImposer(){}

            void set_data(const std::vector<Realvec>& data1,
                          const std::vector<Realvec>& data2);
            void set_data1(const std::vector<Realvec>& data1);
            void set_data2(const std::vector<Realvec>& data2);

            void set_data_push(const std::vector<Realvec>& data);

            void superimpose();

            std::vector<Realvec>& get_data1();
            std::vector<Realvec>& get_data2();

            Matrix3 get_R();

        private:

            Realvec mean(std::vector<Realvec>& structure);
            void move_to_zero();
            Matrix3 calc_R(const std::array<double, 4>& q);
            Matrix4 get_B(const std::vector<Realvec>& rA,
                          const std::vector<Realvec>& rB);
            std::array<double, 4> get_eigenvec(const Matrix4& B);
            void rotate(const Matrix3& R);

        private:

            bool superimposed;
            std::vector<Realvec> structure1;
            std::vector<Realvec> structure2;
    };

    void SuperImposer::set_data(const std::vector<Realvec>& data1,
                  const std::vector<Realvec>& data2)
    {
        structure1 = data1;
        structure2 = data2;
        superimposed = false;
        return;
    }

    void SuperImposer::set_data1(const std::vector<Realvec>& data1)
    {
        structure1 = data1;
        superimposed = false;
        return;
    }

    void SuperImposer::set_data2(const std::vector<Realvec>& data2)
    {
        structure2 = data2;
        superimposed = false;
        return;
    }

    void SuperImposer::set_data_push(const std::vector<Realvec>& data)
    {
        structure2 = structure1;
        structure1 = data;
        superimposed = false;
        return;
    }

    std::vector<Realvec>& SuperImposer::get_data1()
    {
        if(!superimposed)
            std::cout << "Warning: data you got have not superimposed" << std::endl;
        return structure1;
    }

    std::vector<Realvec>& SuperImposer::get_data2()
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

        std::vector<Realvec> rA(structure1.size());
        std::vector<Realvec> rB(structure2.size());

        for(size_t i(0); i<structure1.size(); ++i)
        {
            rA.at(i) = structure1.at(i) + structure2.at(i);
            rB.at(i) = structure2.at(i) - structure1.at(i);
        }

        Matrix4 B(get_B(rA, rB));
        std::array<double, 4> q = get_eigenvec(B);
        Matrix3 R(calc_R(q));

        rotate(R);
        superimposed = true;

        return;
    }

    Matrix3 SuperImposer::get_R()
    {
        if(structure1.empty() || structure2.empty())
            throw std::invalid_argument(
                    "SuperImposer does not have two structures");

        if(structure1.size() != structure2.size())
            throw std::invalid_argument(
                    "SuperImposer has two structures that have different sizes");

        move_to_zero();

        std::vector<Realvec> rA(structure1.size());
        std::vector<Realvec> rB(structure2.size());

        for(size_t i(0); i<structure1.size(); ++i)
        {
            rA.at(i) = structure1.at(i) + structure2.at(i);
            rB.at(i) = structure2.at(i) - structure1.at(i);
        }

        Matrix4 B(get_B(rA, rB));
        std::array<double, 4> q = get_eigenvec(B);
        Matrix3 R(calc_R(q));       
        rotate(R);
        superimposed = true;

        return R;
    }

    void SuperImposer::move_to_zero()
    {
        Realvec mean1(mean(structure1));
        for(std::vector<Realvec>::iterator iter = structure1.begin();
            iter != structure1.end(); ++iter)
        {
            *iter -= mean1;
        }

        Realvec mean2(mean(structure2));
        for(std::vector<Realvec>::iterator iter = structure2.begin();
            iter != structure2.end(); ++iter)
        {
            *iter -= mean2;
        }

        return;
    }

    Realvec SuperImposer::mean(std::vector<Realvec>& structure)
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

    Matrix4
    SuperImposer::get_B(const std::vector<Realvec>& a,
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

    std::array<double, 4> SuperImposer::get_eigenvec(const Matrix4& B)
    {
        Jacobi44Solver solver(B);
        std::pair<double, std::array<double, 4> > min_pair
            = solver.get_mineigenpair();
        return min_pair.second;
    }

    Matrix3 SuperImposer::calc_R(const std::array<double, 4>& q)
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

    void SuperImposer::rotate(const Matrix3& R)
    {
        for(std::vector<Realvec>::iterator iter = structure1.begin();
            iter != structure1.end(); ++iter)
        {
            Realvec temp(R * (*iter));
            *iter = temp;
        }
        return;
    }

}//end namespace coffeemill

#endif//COFFEE_MILL_SUPER_IMPOSER
