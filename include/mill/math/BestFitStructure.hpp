#ifndef COFFEE_MILL_MATH_BEST_FIT_STRUCTURE_HPP
#define COFFEE_MILL_MATH_BEST_FIT_STRUCTURE_HPP
#include "EigenSolver.hpp"
#include <vector>

namespace mill
{

template<typename scalarT>
class BestFit
{
  public:

    using scalar_type    = scalarT;
    using vector3_type   = Matrix<scalar_type, 3, 1>;
    using vector4_type   = Matrix<scalar_type, 4, 1>;
    using matrix33_type  = Matrix<scalar_type, 3, 3>;
    using matrix44_type  = Matrix<scalar_type, 4, 4>;
    using structure_type = std::vector<vector3_type>;

  public:
    BestFit() = default;
    ~BestFit() = default;

    structure_type
    fit(const structure_type& snapshot, const structure_type& reference) const;

    structure_type // with reference cash
    fit(const structure_type& snapshot) const;

    matrix33_type
    rotational_matrix(const structure_type& snapshot,
                      const structure_type& reference) const;

    matrix33_type // with cash
    rotational_matrix(const structure_type& snapshot) const;

    vector3_type zeroing_vector(const structure_type& snapshot) const;

    structure_type const& reference() const {return reference_;}
    void set_reference(const structure_type& snapshot);

  private:

    structure_type copy_to_center(const structure_type& str) const;
    void           move_to_center(structure_type& str) const;

    matrix33_type make_rotational_matrix(const structure_type& snapshot,
                                         const structure_type& reference) const;
    matrix33_type make_rotational_matrix(const vector4_type& quaternion) const;

    matrix44_type make_score_matrix(const std::vector<vector3_type>& a,
                                    const std::vector<vector3_type>& b) const;


  private:

    vector3_type   center_;
    structure_type reference_; //!< optional;
};

template<typename sclT>
typename BestFit<sclT>::structure_type
BestFit<sclT>::fit(const structure_type& str, const structure_type& ref) const
{
    if(str.size() != ref.size())
    {
        throw std::invalid_argument("different structure in argument");
    }

    structure_type target = this->copy_to_center(str);
    const structure_type reference = this->copy_to_center(ref);

    auto R = this->make_rotational_matrix(target, reference);
    for(auto iter = target.begin(); iter != target.end(); ++iter)
    {
        const vector3_type temp = R * (*iter);
        *iter = temp;
    }

    return target;
}

template<typename sclT>
typename BestFit<sclT>::structure_type
BestFit<sclT>::fit(const structure_type& str) const
{
    if(str.size() != this->reference_.size())
    {
        throw std::invalid_argument("different structure in argument");
    }

    structure_type target = this->copy_to_center(str);

    auto R = this->make_rotational_matrix(target, this->reference_);
    for(auto iter = target.begin(); iter != target.end(); ++iter)
    {
        const vector3_type temp = R * (*iter) + this->center_;
        *iter = temp;
    }

    return target;
}

template<typename sclT>
typename BestFit<sclT>::matrix33_type
BestFit<sclT>::rotational_matrix(
        const structure_type& str, const structure_type& ref) const
{
    if(str.size() != ref.size())
    {
        throw std::invalid_argument("different structure in argument");
    }

    const structure_type target = this->copy_to_center(str);
    const structure_type reference = this->copy_to_center(ref);

    return this->make_rotational_matrix(target, reference);
}

template<typename sclT>
typename BestFit<sclT>::matrix33_type
BestFit<sclT>::rotational_matrix(const structure_type& str) const
{
    if(str.size() != this->reference_.size())
    {
        throw std::invalid_argument("different structure in argument");
    }

    const structure_type target = this->copy_to_center(str);

    return this->make_rotational_matrix(target, this->reference_);
}

template<typename sclT>
typename BestFit<sclT>::matrix33_type
BestFit<sclT>::make_rotational_matrix(
        const structure_type& structure, const structure_type& reference) const
{
    if(structure.size() != reference.size())
    {
        throw std::invalid_argument("different size structures");
    }

    std::vector<vector3_type> rA(structure.size());
    std::vector<vector3_type> rB(structure.size());

    for(std::size_t i=0; i < structure.size(); ++i)
    {
        rA[i] = reference[i] + structure[i];
        rB[i] = reference[i] - structure[i];
    }

    auto score = make_score_matrix(rA, rB);

    JacobiEigenSolver solver;
    auto ev = solver.solve(score);
    sclT eigenvalue = std::numeric_limits<sclT>::max();
    std::size_t index = 0;
    for(std::size_t i=0; i<4; ++i)
    {
        if(ev.at(i).first < eigenvalue)
        {
            eigenvalue = ev.at(i).first;
            index = i;
        }
    }
    const vector4_type q = ev.at(index).second;
    return this->make_rotational_matrix(q);
}

template<typename sclT>
typename BestFit<sclT>::matrix33_type
BestFit<sclT>::make_rotational_matrix(const vector4_type& q) const
{
    matrix33_type R;
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

template<typename sclT>
typename BestFit<sclT>::matrix44_type
BestFit<sclT>::make_score_matrix(const std::vector<vector3_type>& a,
                                 const std::vector<vector3_type>& b) const
{
    if(a.size() != b.size())
        throw std::invalid_argument("different size structures");

    matrix44_type retval;
    retval(0,0) = 0.;
    retval(0,1) = 0.;
    retval(0,2) = 0.;
    retval(0,3) = 0.;
    retval(1,1) = 0.;
    retval(1,2) = 0.;
    retval(1,3) = 0.;
    retval(2,2) = 0.;
    retval(2,3) = 0.;
    retval(3,3) = 0.;

    for(std::size_t i(0); i<a.size(); ++i)
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

    const sclT invN = 1. / static_cast<sclT>(a.size());

    retval(0,0) *= invN;
    retval(0,1) *= invN;
    retval(0,2) *= invN;
    retval(0,3) *= invN;
    retval(1,1) *= invN;
    retval(1,2) *= invN;
    retval(1,3) *= invN;
    retval(2,2) *= invN;
    retval(2,3) *= invN;
    retval(3,3) *= invN;

    //symmetric matrix
    retval(1,0) = retval(0,1);
    retval(2,0) = retval(0,2);
    retval(2,1) = retval(1,2);
    retval(3,0) = retval(0,3);
    retval(3,1) = retval(1,3);
    retval(3,2) = retval(2,3);

    return retval;
}

template<typename sclT>
typename BestFit<sclT>::structure_type
BestFit<sclT>::copy_to_center(const structure_type& str) const
{
    vector3_type sum(0., 0., 0.);
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
    {
        sum += *iter;
    }
    const sclT invN = 1. / static_cast<sclT>(str.size());
    const vector3_type centroid = sum * invN;

    structure_type retval(str.size());
    for(std::size_t i = 0; i<str.size(); ++i)
    {
        retval[i] = str[i] - centroid;
    }

    return retval;
}

template<typename sclT>
void BestFit<sclT>::move_to_center(structure_type& str) const
{
    vector3_type sum(0., 0., 0.);
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
    {
        sum += *iter;
    }
    const sclT invN = 1. / static_cast<sclT>(str.size());
    const vector3_type centroid = sum * invN;

    for(auto iter = str.begin(); iter != str.end(); ++iter)
    {
        *iter -= centroid;
    }
    return ;
}

template<typename sclT>
void BestFit<sclT>::set_reference(const structure_type& ref)
{
    this->reference_ = ref;
    this->center_    = this->zeroing_vector(ref);
    this->move_to_center(this->reference_);
    return;
}

template<typename sclT>
typename BestFit<sclT>::vector3_type
BestFit<sclT>::zeroing_vector(const structure_type& str) const
{
    vector3_type sum(0., 0., 0.);
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
    {
        sum += *iter;
    }
    const sclT invN = 1. / static_cast<sclT>(str.size());
    return sum * invN;
}

}// mill

#endif /* COFFEE_MILL_BEST_FIT */
