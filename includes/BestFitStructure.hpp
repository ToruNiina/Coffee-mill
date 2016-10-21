#ifndef COFFEE_MILL_BEST_FIT
#define COFFEE_MILL_BEST_FIT
#include <vector>

namespace mill
{

template<typename sclT, template<typename sclT1, std::size_t D> class vectorT,
         template<typename sclT2, std::size_t R, std::size_t C> class matrixT>
class BestFit
{
  public:

    using scalar_type   = sclT;
    using vector3_type  = vectorT<sclT, 3>;
    using vector4_type  = vectorT<sclT, 4>;
    using matrix33_type = matrixT<sclT, 3, 3>;
    using matrix44_type = matrixT<sclT, 4, 4>;
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

    structure_type const& reference() const {return reference_;}
    structure_type &      reference()       {return reference_;}

  private:

    structure_type move_to_center(const structure_type& str) const;
    void move_to_center(structure_type& str) const;

    matrix33_type make_rotation_matrix(const structure_type& snapshot,
                                       const structure_type& reference) const;
    matrix33_type make_rotation_matrix(const vector4_type& quaternion) const;

    matrix44_type score_matrix(const std::vector<vector3_type>& a,
                               const std::vector<vector3_type>& b) const;


  private:

    structure_type reference_; //!< optional;
};

template<typename sclT, template<typename sT1, std::size_t D> class vectorT,
         template<typename sT2, std::size_t R, std::size_t C> class matrixT>
typename BestFit<sclT, vectorT, matrixT>::structure_type
BestFit<sclT, vectorT, matrixT>::fit(
        const structure_type& str, const structure_type& ref) const
{
    if(str.size() != ref.size())
        throw std::invalid_argument("different structure in argument");

    vector3_type sum_str(0., 0., 0.);
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        sum_str += *iter;
    const vector3_type centroid_str =
        sum_str / static_cast<scalar_type>(str.size());

    structure_type rotated_str;
    rotated_str.reserve(str.size());
    for(auto iter = str.begin(); iter != str.end(); ++iter)
        rotated_str.push_back(*iter - centroid_str);

    vector3_type sum_ref(0., 0., 0.);
    for(auto iter = ref.cbegin(); iter != ref.cend(); ++iter)
        sum_ref += *iter;
    vector3_type centroid_ref = 
        sum_ref / static_cast<scalar_type>(ref.size());

    structure_type zeroed_ref;
    zeroed_ref.reserve(ref.size());
    for(auto iter = ref.begin(); iter != ref.end(); ++iter)
        zeroed_ref.push_back(*iter - centroid_ref);

    auto R = this->make_rotation_matrix(rotated_str, zeroed_ref);
    for(auto iter = rotated_str.begin(); iter != rotated_str.end(); ++iter)
    {
        const vector3_type temp = R * (*iter);
        *iter = temp;
    }

    return rotated_str;
}

template<typename sclT, template<typename sT1, std::size_t D> class vectorT,
         template<typename sT2, std::size_t R, std::size_t C> class matrixT>
typename BestFit<sclT, vectorT, matrixT>::matrix33_type
BestFit<sclT, vectorT, matrixT>::rotational_matrix(
        const structure_type& structure) const
{
    assert(structure.size() == this->reference_.size());

    vector3_type sum(0e0);
    for(auto iter = structure.cbegin(); iter != structure.cend(); ++iter)
        sum += *iter;
    const vector3_type centroid = sum / static_cast<sclT>(structure.size());

    structure_type zeroed;
    zeroed.reserve(structure.size());
    for(auto iter = structure.cbegin(); iter != structure.cend(); ++iter)
        zeroed.push_back(*iter - centroid);

    return RotationMatrix(zeroed, true);
}

template<typename sclT, template<typename sT1, std::size_t D> class vectorT,
         template<typename sT2, std::size_t R, std::size_t C> class matrixT>
typename BestFit<sclT, vectorT, matrixT>::matrix33_type
BestFit<sclT, vectorT, matrixT>::make_rotation_matrix(
        const structure_type& structure, const structure_type& reference) const 
{
    if(structure.size() != reference.size())
        throw std::invalid_argument("different size structures");
    std::vector<vector3_type> rA(structure.size());
    std::vector<vector3_type> rB(structure.size());

    for(std::size_t i=0; i < structure.size(); ++i)
    {
        rA[i] = reference[i] + structure[i];
        rB[i] = reference[i] - structure[i];
    }

    auto score = score_matrix(rA, rB);
//     ax::JacobiSolver<4> solver(score);
//     std::pair<double, Vector4d> min_pair = solver.get_mineigenpair();
//     const Vector4d q = min_pair.second;

//     return rotation_matrix(q);
}

template<typename sclT, template<typename sT1, std::size_t D> class vectorT,
         template<typename sT2, std::size_t R, std::size_t C> class matrixT>
typename BestFit<sclT, vectorT, matrixT>::matrix44_type
BestFit<sclT, vectorT, matrixT>::score_matrix(
        const std::vector<vector3_type>& a, const std::vector<vector3_type>& b) const
{
    if(a.size() != b.size())
        throw std::invalid_argument("different size structures");

    matrix44_type retval;
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

    const sclT inv_N = 1. / static_cast<sclT>(a.size());

    retval(0,0) = retval(0,0) * inv_N;
    retval(0,1) = retval(0,1) * inv_N;
    retval(0,2) = retval(0,2) * inv_N;
    retval(0,3) = retval(0,3) * inv_N;
    retval(1,1) = retval(1,1) * inv_N;
    retval(1,2) = retval(1,2) * inv_N;
    retval(1,3) = retval(1,3) * inv_N;
    retval(2,2) = retval(2,2) * inv_N;
    retval(2,3) = retval(2,3) * inv_N;
    retval(3,3) = retval(3,3) * inv_N;

    //symmetric matrix
    retval(1,0) = retval(0,1);
    retval(2,0) = retval(0,2);
    retval(2,1) = retval(1,2);
    retval(3,0) = retval(0,3);
    retval(3,1) = retval(1,3);
    retval(3,2) = retval(2,3);

    return retval;
}

template<typename sclT, template<typename sT1, std::size_t D> class vectorT,
         template<typename sT2, std::size_t R, std::size_t C> class matrixT>
typename BestFit<sclT, vectorT, matrixT>::matrix33_type
BestFit<sclT, vectorT, matrixT>::make_rotation_matrix(const vector4_type& q) const
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


template<typename sclT, template<typename sT1, std::size_t D> class vectorT,
         template<typename sT2, std::size_t R, std::size_t C> class matrixT>
typename BestFit<sclT, vectorT, matrixT>::structure_type
BestFit<sclT, vectorT, matrixT>::move_to_center(const structure_type& str) const
{
    vector3_type sum(0., 0., 0.);
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        sum += *iter;
    const sclT invN = 1. / static_cast<double>(str.size());
    const vector3_type centroid = sum * invN;

    structure_type retval(str.size());
    for(std::size_t i = 0; i<str.size(); ++i)
        retval[i] = str[i] - centroid;

    return retval;
}

template<typename sclT, template<typename sT1, std::size_t D> class vectorT,
         template<typename sT2, std::size_t R, std::size_t C> class matrixT>
void BestFit<sclT, vectorT, matrixT>::move_to_center(structure_type& str) const
{
    vector3_type sum(0., 0., 0.);
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        sum += *iter;
    const sclT invN = 1. / static_cast<double>(str.size());
    const vector3_type centroid = sum * invN;

    for(auto iter = str.begin(); iter != str.end(); ++iter)
        *iter -= centroid;

    return ;
}

}// mill

#endif /* COFFEE_MILL_BEST_FIT */
