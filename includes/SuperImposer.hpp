#ifndef COFFEE_MILL_SUPER_IMPOSER
#define COFFEE_MILL_SUPER_IMPOSER
#include <iostream>
#include <vector>
#include "mathematics/LinearAlgebra.hpp"

namespace coffeemill
{
    /*@brief > This class superimposes(structural alignment) the Structure *
     * (a.k.a. std::vector<RealVector<3>>). Mainly used for preparation of *
     * RMSD calculation. This rotate subject structure to fit reference    *
     * structure.                                                          */
    class SuperImposer
    {
        public:

            SuperImposer(): is_superimposed(false){}
            SuperImposer(const std::vector<Realvec>& ref)
                : is_superimposed(false), is_rotation_matrix_calculated(false),
                  reference(ref)
            {}
            // attention the order of argument.
            SuperImposer(const std::vector<Realvec>& ref,
                         const std::vector<Realvec>& sub)
                : is_superimposed(false), is_rotation_matrix_calculated(false),
                  reference(ref), subject(sub)
            {}

            SuperImposer(const SuperImposer& s) = delete;
            SuperImposer(SuperImposer&& s) = delete;
            SuperImposer& operator=(const SuperImposer& s) = delete;
            ~SuperImposer() = default;

            void set_reference(const std::vector<Realvec>& ref);
            void set_subject(const std::vector<Realvec>& subj);
            void set_ref_and_sub(const std::vector<Realvec>& ref,
                                 const std::vector<Realvec>& sub);

            // this method returns reference structure and make subject
            // structure reference. the next subject structure is argument, ref.
            // image) return <== reference <== subject <== argument
            std::vector<Realvec>
                push_datas(const std::vector<Realvec>& newsub);

            void superimpose();
            Matrix3 calc_and_get_R();

            std::vector<Realvec>& get_reference(){return reference;};
            std::vector<Realvec>& get_subject(){return subject;};
            std::vector<Realvec> copy_reference() const;
            std::vector<Realvec> copy_subject() const;

        private:

            void calc_R();
            Realvec mean(const std::vector<Realvec>& structure);
            void move_to_zero();
            Matrix3 make_R_from_quaternion(const RealVector<4>& q);
            Matrix4 calc_B(const std::vector<Realvec>& rA,
                           const std::vector<Realvec>& rB);
            RealVector<4> get_eigenvec(const Matrix4& B);
            void rotate_subject(const Matrix3& R);

        private:

            bool is_superimposed;
            bool is_rotation_matrix_calculated;
            Matrix3 rotation_matrix;
            std::vector<Realvec> reference;// keep this const
            std::vector<Realvec> subject;  // rotate this!
    };

    void SuperImposer::set_ref_and_sub(const std::vector<Realvec>& ref,
                                       const std::vector<Realvec>& sub)
    {
        reference = ref;
        subject   = sub;
        is_superimposed = false;
        is_rotation_matrix_calculated = false;
        return;
    }

    void SuperImposer::set_reference(const std::vector<Realvec>& ref)
    {
        reference = ref;
        is_superimposed = false;
        is_rotation_matrix_calculated = false;
        return;
    }

    void SuperImposer::set_subject(const std::vector<Realvec>& sub)
    {
        subject = sub;
        is_superimposed = false;
        is_rotation_matrix_calculated = false;
        return;
    }

    std::vector<Realvec>
        SuperImposer::push_datas(const std::vector<Realvec>& newsub)
    {
        std::vector<Realvec> temp = reference;
        reference = subject;
        subject = newsub;
        is_superimposed = false;
        is_rotation_matrix_calculated = false;
        return temp;
    }

    std::vector<Realvec> SuperImposer::copy_reference() const
    {
        if(!is_superimposed)
            std::cout << "Warning: data you got have not superimposed"
                      << std::endl;
        return reference;
    }

    std::vector<Realvec> SuperImposer::copy_subject() const
    {
        if(!is_superimposed)
            std::cout << "Warning: data you got have not superimposed"
                      << std::endl;
        return subject;
    }

    void SuperImposer::superimpose()
    {
        if(is_superimposed) return;

        calc_R();
        rotate_subject(rotation_matrix);
        is_superimposed = true;

        return;
    }

    Matrix3 SuperImposer::calc_and_get_R()
    {
        calc_R();
        rotate_subject(rotation_matrix);
        is_superimposed = true;

        return rotation_matrix;
    }
    
    void SuperImposer::calc_R()
    {
        if(is_rotation_matrix_calculated)
            return;

        if(reference.empty() || subject.empty())
            throw std::invalid_argument(
                    "SuperImposer does not have two structures");

        if(reference.size() != subject.size())
            throw std::invalid_argument(
                    "SuperImposer has different structure");

        move_to_zero();

        std::vector<Realvec> rA(reference.size());
        std::vector<Realvec> rB(reference.size());

        for(size_t i(0); i<reference.size(); ++i)
        {
            rA.at(i) = reference[i] + subject[i];
            rB.at(i) = reference[i] - subject[i];
        }

        Matrix4 B(calc_B(rA, rB));
        RealVector<4> q = get_eigenvec(B);
        rotation_matrix = (make_R_from_quaternion(q));
        is_rotation_matrix_calculated = true;

        return;
    }

    void SuperImposer::move_to_zero()
    {
        Realvec mean_ref(mean(reference));
        if(length(mean_ref) > 1e-12)
        {
            for(auto iter = reference.begin();
                    iter != reference.end(); ++iter)
            {
                *iter -= mean_ref;
            }
        }

        Realvec mean_sub(mean(subject));
        if(length(mean_sub) > 1e-12)
        {
            for(auto iter = subject.begin();
                    iter != subject.end(); ++iter)
            {
                *iter -= mean_sub;
            }
        }
        return;
    }

    // mass is not defined.
    // this calculates geometric center, mean of positions.
    Realvec SuperImposer::mean(const std::vector<Realvec>& structure)
    {
        Realvec sum(0e0, 0e0, 0e0);
        for(auto iter = structure.cbegin(); iter != structure.cend(); ++iter)
        {
            sum += *iter;
        }
        return (sum / static_cast<double>(structure.size()));
    }

    Matrix4
    SuperImposer::calc_B(const std::vector<Realvec>& a,
                         const std::vector<Realvec>& b)
    {
        if(a.size() != b.size())
            throw std::invalid_argument("different size in calculation B");

        Matrix4 retval(0e0);

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

        // this matrix is symmetric matrix
        retval(1,0) = retval(0,1);
        retval(2,0) = retval(0,2);
        retval(2,1) = retval(1,2);
        retval(3,0) = retval(0,3);
        retval(3,1) = retval(1,3);
        retval(3,2) = retval(2,3);

        return retval;
    }

    RealVector<4> SuperImposer::get_eigenvec(const Matrix4& B)
    {
        JacobiSolver<4> solver(B);
        std::pair<double, RealVector<4>> min_pair
            = solver.get_mineigenpair();
        return min_pair.second;
    }

    Matrix3 SuperImposer::make_R_from_quaternion(const RealVector<4>& q)
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

    //this rotates subject.
    void SuperImposer::rotate_subject(const Matrix3& R)
    {
        for(auto iter = subject.begin(); iter != subject.end(); ++iter)
        {
            Realvec temp(R * (*iter));
            *iter = temp;
        }
        return;
    }

}//end namespace coffeemill

#endif//COFFEE_MILL_SUPER_IMPOSER
