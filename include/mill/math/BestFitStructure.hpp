#ifndef COFFEE_MILL_MATH_BEST_FIT_STRUCTURE_HPP
#define COFFEE_MILL_MATH_BEST_FIT_STRUCTURE_HPP
#include "Matrix.hpp"
#include "Vector.hpp"
#include "EigenSolver.hpp"
#include <vector>

namespace mill
{

class BestFit
{
  public:
    using real_type = double;

  public:

    BestFit() = default;
    ~BestFit() = default;

    std::vector<Vector<real_type, 3>>
    fit(const std::vector<Vector<real_type, 3>>& snapshot,
        const std::vector<Vector<real_type, 3>>& reference) const;

    std::vector<Vector<real_type, 3>>
    fit(const std::vector<Vector<real_type, 3>>& snapshot) const;

    Matrix<real_type, 3, 3>
    rotational_matrix(const std::vector<Vector<real_type, 3>>& snapshot,
                      const std::vector<Vector<real_type, 3>>& reference) const;

    Matrix<real_type, 3, 3>
    rotational_matrix(const std::vector<Vector<real_type, 3>>& snapshot) const; // use cache

    Vector<real_type, 3>
    zeroing_vector(const std::vector<Vector<real_type, 3>>& snapshot) const;

    std::vector<Vector<real_type, 3>> const& reference() const {return reference_;}
    void set_reference(const std::vector<Vector<real_type, 3>>& snapshot);

  private:

    std::vector<Vector<real_type, 3>>
    copy_to_center(const std::vector<Vector<real_type, 3>>& str) const;

    void move_to_center(std::vector<Vector<real_type, 3>>& str) const;

    Matrix<real_type, 3, 3>
    make_rotational_matrix(const std::vector<Vector<real_type, 3>>& snapshot,
                           const std::vector<Vector<real_type, 3>>& reference) const;

    Matrix<real_type, 3, 3>
    make_rotational_matrix(const Vector<real_type, 4>& quaternion) const;

    Matrix<real_type, 4, 4>
    make_score_matrix(const std::vector<Vector<real_type, 3>>& a,
                      const std::vector<Vector<real_type, 3>>& b) const;
  private:

    Vector<real_type, 3> center_;
    std::vector<Vector<real_type, 3>> reference_; //!< optional;
};

inline std::vector<Vector<BestFit::real_type, 3>>
BestFit::fit(const std::vector<Vector<real_type, 3>>& str,
             const std::vector<Vector<real_type, 3>>& ref) const
{
    if(str.size() != ref.size())
    {
        log::fatal("BestFit::fit(structure, reference): "
                "number of particles differ from each other.");
    }

    auto target = this->copy_to_center(str);
    const auto reference = this->copy_to_center(ref);

    const auto R = this->make_rotational_matrix(target, reference);
    for(auto& vec : target)
    {
        vec = R * vec;
    }
    return target;
}

inline std::vector<Vector<BestFit::real_type, 3>>
BestFit::fit(const std::vector<Vector<real_type, 3>>& str) const
{
    if(str.size() != this->reference_.size())
    {
        log::fatal("BestFit::fit(structure): number of particles differ from "
                "each other. Reference structure might not be defined");
    }

    auto target = this->copy_to_center(str);

    const auto R = this->make_rotational_matrix(target, this->reference_);
    for(auto& vec : target)
    {
        vec = R * vec + this->center_;
    }
    return target;
}

inline Matrix<BestFit::real_type, 3, 3>
BestFit::rotational_matrix(const std::vector<Vector<real_type, 3>>& str,
                           const std::vector<Vector<real_type, 3>>& ref) const
{
    if(str.size() != ref.size())
    {
        log::fatal("BestFit::rotational_matrix(structure, reference): "
                "number of particles differ from each other.");
    }

    const auto target    = this->copy_to_center(str);
    const auto reference = this->copy_to_center(ref);

    return this->make_rotational_matrix(target, reference);
}

inline Matrix<BestFit::real_type, 3, 3>
BestFit::rotational_matrix(const std::vector<Vector<real_type, 3>>& str) const
{
    if(str.size() != this->reference_.size())
    {
        log::fatal("BestFit::rotational_matrix(structure): "
                "number of particles differ from each other. "
                "reference structure might not be set.");
    }

    const std::vector<Vector<real_type, 3>> target = this->copy_to_center(str);

    return this->make_rotational_matrix(target, this->reference_);
}

inline Matrix<BestFit::real_type, 3, 3>
BestFit::make_rotational_matrix(
        const std::vector<Vector<real_type, 3>>& structure,
        const std::vector<Vector<real_type, 3>>& reference) const
{
    if(structure.size() != reference.size())
    {
        log::fatal("BestFit::make_rotational_matrix(structure): "
                "number of particles differ from each other. ");
    }

    std::vector<Vector<real_type, 3>> rA(structure.size());
    std::vector<Vector<real_type, 3>> rB(structure.size());

    for(std::size_t i=0; i < structure.size(); ++i)
    {
        rA[i] = reference[i] + structure[i];
        rB[i] = reference[i] - structure[i];
    }

    auto score = make_score_matrix(rA, rB);

    JacobiEigenSolver solver;
    const auto ev = solver.solve(score);

    // find a quaternion that corresponds to the minimum score (minimum RMSD)
    double eigenvalue = std::numeric_limits<double>::max();
    std::size_t index = 0;

    for(std::size_t i=0; i<4; ++i)
    {
        if(ev.at(i).first < eigenvalue)
        {
            eigenvalue = ev.at(i).first;
            index = i;
        }
    }

    return this->make_rotational_matrix(ev.at(index).second);
}

inline Matrix<BestFit::real_type, 3, 3>
BestFit::make_rotational_matrix(const Vector<real_type, 4>& q) const
{
    Matrix<real_type, 3, 3> R;
    R(0,0) = 2 * q[0] * q[0] + 2 * q[1] * q[1] - 1.0;
    R(0,1) = 2 * q[1] * q[2] - 2 * q[0] * q[3];
    R(0,2) = 2 * q[1] * q[3] + 2 * q[0] * q[2];
    R(1,0) = 2 * q[1] * q[2] + 2 * q[0] * q[3];
    R(1,1) = 2 * q[0] * q[0] + 2 * q[2] * q[2] - 1.0;
    R(1,2) = 2 * q[2] * q[3] - 2 * q[0] * q[1];
    R(2,0) = 2 * q[1] * q[3] - 2 * q[0] * q[2];
    R(2,1) = 2 * q[2] * q[3] + 2 * q[0] * q[1];
    R(2,2) = 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1.0;
    return R;
}

inline Matrix<BestFit::real_type, 4, 4>
BestFit::make_score_matrix(const std::vector<Vector<real_type, 3>>& a,
                           const std::vector<Vector<real_type, 3>>& b) const
{
    if(a.size() != b.size())
    {
        log::fatal("BestFit::make_score_matrix(): numbers of particle differ from each other.");
    }

    Matrix<real_type, 4, 4> retval;
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

    for(std::size_t i=0; i<a.size(); ++i)
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

    const real_type invN = 1. / static_cast<real_type>(a.size());

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

inline std::vector<Vector<BestFit::real_type, 3>>
BestFit::copy_to_center(const std::vector<Vector<real_type, 3>>& str) const
{
    const auto centroid = this->zeroing_vector(str);

    std::vector<Vector<real_type, 3>> retval(str.size());
    for(std::size_t i = 0; i<str.size(); ++i)
    {
        retval[i] = str[i] - centroid;
    }
    return retval;
}

inline void BestFit::move_to_center(std::vector<Vector<real_type, 3>>& str) const
{
    const auto centroid = this->zeroing_vector(str);
    for(auto& vec : str)
    {
        vec -= centroid;
    }
    return ;
}

inline void BestFit::set_reference(const std::vector<Vector<real_type, 3>>& ref)
{
    this->reference_ = ref;
    this->center_    = this->zeroing_vector(ref);
    this->move_to_center(this->reference_);
    return;
}

inline Vector<BestFit::real_type, 3>
BestFit::zeroing_vector(const std::vector<Vector<real_type, 3>>& str) const
{
    Vector<real_type, 3> sum(0., 0., 0.);
    for(const auto& vec : str)
    {
        sum += vec;
    }
    return sum / static_cast<real_type>(str.size());
}

}// mill
#endif /* COFFEE_MILL_BEST_FIT */
