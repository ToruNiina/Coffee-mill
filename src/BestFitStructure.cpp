#include "BestFitStructure.hpp"

namespace coffeemill
{

BestFit::Structure BestFit::fit(const Structure& structure) const
{
    assert(structure.size() == this->reference_.size());

    Structure rotated(structure);
    Vector3d sum(0e0);
    for(auto iter = rotated.cbegin(); iter != rotated.cend(); ++iter)
        sum += *iter;

    const Vector3d CoM = sum / static_cast<double>(rotated.size());
    for(auto iter = rotated.begin(); iter != rotated.end(); ++iter)
        *iter -= CoM;

    auto R = this->RotationMatrix(rotated, true);
    for(auto iter = rotated.begin(); iter != rotated.end(); ++iter)
    {
        const Vector3d temp = R * (*iter);
        *iter = temp;
    }

    std::cerr << "fitting complete" << std::endl;
    
    return rotated;
}

BestFit::Matrix3d BestFit::RotationMatrix(const Structure& structure) const 
{
    assert(structure.size() == this->reference_.size());

    Vector3d sum(0e0);
    for(auto iter = structure.cbegin(); iter != structure.cend(); ++iter)
        sum += *iter;
    const Vector3d CoM = sum / static_cast<double>(structure.size());

    Structure zeroed;
    zeroed.reserve(structure.size());
    for(auto iter = structure.cbegin(); iter != structure.cend(); ++iter)
        zeroed.push_back(*iter - CoM);

    return RotationMatrix(zeroed, true);
}

BestFit::Matrix3d BestFit::RotationMatrix(const Structure& structure, bool zeroed) const 
{
    assert(zeroed);
    std::vector<Vector3d> rA(structure.size());
    std::vector<Vector3d> rB(structure.size());

    for(std::size_t i=0; i < structure.size(); ++i)
    {
        rA.at(i) = reference_[i] + structure[i];
        rB.at(i) = reference_[i] - structure[i];
    }

    auto score = score_matrix(rA, rB);
    ax::JacobiSolver<4> solver(score);
    std::pair<double, Vector4d> min_pair = solver.get_mineigenpair();
    const Vector4d q = min_pair.second;

    return rotation_matrix(q);
}

BestFit::Matrix4d
BestFit::score_matrix(const std::vector<Vector3d>& a,
                      const std::vector<Vector3d>& b) const
{
    assert(a.size() == b.size());

    Matrix4d retval(0e0);
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

    const double N = static_cast<double>(a.size());

    retval(0,0) = retval(0,0) / N;
    retval(0,1) = retval(0,1) / N;
    retval(0,2) = retval(0,2) / N;
    retval(0,3) = retval(0,3) / N;
    retval(1,1) = retval(1,1) / N;
    retval(1,2) = retval(1,2) / N;
    retval(1,3) = retval(1,3) / N;
    retval(2,2) = retval(2,2) / N;
    retval(2,3) = retval(2,3) / N;
    retval(3,3) = retval(3,3) / N;

    //symmetric matrix
    retval(1,0) = retval(0,1);
    retval(2,0) = retval(0,2);
    retval(2,1) = retval(1,2);
    retval(3,0) = retval(0,3);
    retval(3,1) = retval(1,3);
    retval(3,2) = retval(2,3);

    return retval;
}

BestFit::Matrix3d BestFit::rotation_matrix(const Vector4d& q) const
{
    Matrix3d R(0e0);
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

}
