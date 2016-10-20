#ifndef COFFEE_MILL_BEST_FIT
#define COFFEE_MILL_BEST_FIT
#include "math/LinearAlgebra.hpp"

namespace coffeemill
{

class BestFit
{
  public:

    using Vector3d = ax::Vector3d;
    using Vector4d = ax::RealVector<4>;
    using Matrix3d = ax::Matrix3;
    using Matrix4d = ax::Matrix4;
    using Structure = std::vector<Vector3d>;

  public:
    BestFit() = default;
    explicit BestFit(const Structure& reference)
    {
        Vector3d sum(0e0);
        for(auto iter = reference.cbegin(); iter != reference.cend(); ++iter)
            sum += *iter;
        const Vector3d CoM = sum / static_cast<double>(reference.size());

        this->reference_.reserve(reference.size());
        for(auto iter = reference.cbegin(); iter != reference.cend(); ++iter)
            this->reference_.push_back(*iter - CoM);
    }
    ~BestFit() = default;

    Structure fit(const Structure& snapshot) const;
    Matrix3d  RotationMatrix(const Structure& snapshot) const;

    const Structure& reference() const {return reference_;}
          Structure& reference()       {return reference_;}

  private:

    Matrix3d RotationMatrix(const Structure& snapshot,
                            bool zeroed /* = dummy */) const;

    Matrix4d score_matrix(const std::vector<Vector3d>& a,
                          const std::vector<Vector3d>& b) const;

    Matrix3d rotation_matrix(const Vector4d& quaternion) const;

  private:

    Structure reference_;
};

}

#endif /* COFFEE_MILL_BEST_FIT */
