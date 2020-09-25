#ifndef COFFEE_MILL_MATH_EIGEN_SOLVER_HPP
#define COFFEE_MILL_MATH_EIGEN_SOLVER_HPP
#include "Matrix.hpp"
#include "Vector.hpp"
#include <array>
#include <utility>
#include <cmath>

namespace mill
{

class JacobiEigenSolver
{
  public:
    constexpr static std::size_t max_loop = 10000;

  public:
    JacobiEigenSolver() = default;
    ~JacobiEigenSolver() = default;

    template<typename scalarT, std::size_t N>
    std::array<std::pair<scalarT, Vector<scalarT, N>>, N>
    solve(const Matrix<scalarT, N, N>& mat) const;

  private:

    template<typename scalarT, std::size_t N>
    bool is_symmetric(const Matrix<scalarT, N, N>& mat) const;

    template<typename scalarT, std::size_t N>
    std::pair<std::size_t, std::size_t>
    max_element(const Matrix<scalarT, N, N>& mat) const;

    template<typename scalarT, std::size_t N>
    scalarT max_relative_diff(const Matrix<scalarT, N, N>& lhs,
                              const Matrix<scalarT, N, N>& rhs) const;

    template<typename scalarT>
    static scalarT relative_tolerance()
    {
        if constexpr (std::is_same_v<scalarT, double>) {return 1e-13;}
        else {return 1e-5f;}
    }

    template<typename scalarT>
    static scalarT absolute_tolerance()
    {
        if constexpr (std::is_same_v<scalarT, double>) {return 1e-13;}
        else {return 1e-5f;}
    }
};

template<typename scalarT, std::size_t N>
std::array<std::pair<scalarT, Vector<scalarT, N>>, N>
JacobiEigenSolver::solve(const Matrix<scalarT, N, N>& mat) const
{
    if(not this->is_symmetric(mat))
    {
        throw std::invalid_argument("asymmetric matrix");
    }

    typedef Matrix<scalarT, N, N> Matrix_type;
    typedef Matrix<scalarT, N, 1> Vector_type;
    typedef scalarT               Real;

    Matrix_type m = mat;
    Matrix_type Ps = Matrix_type::identity();

    std::size_t loop=0;
    for(; loop < max_loop; ++loop)
    {
        const auto [i, j] = this->max_element(m);
        if(std::abs(m(i, j)) < absolute_tolerance<Real>())
        {
            break;
        }

        const Real alpha = (m(i, i) - m(j, j)) * 0.5;
        const Real beta  = -1. * m(i, j);
        const Real gamma = std::abs(alpha) / std::sqrt(alpha * alpha + beta * beta);
        const Real cos_t = std::sqrt(0.5 + gamma * 0.5);
        const Real sin_t = std::copysign(std::sqrt(0.5 - gamma * 0.5), alpha * beta);

        Matrix_type P = Matrix_type::identity();
        P(i, i) =  cos_t;
        P(i, j) =  sin_t;
        P(j, i) = -sin_t;
        P(j, j) =  cos_t;

        Matrix_type tmp = transpose(P) * m * P;
        if(this->max_relative_diff(m, tmp) < relative_tolerance<Real>())
        {
            break;
        }
        tmp(i, j) = 0.;
        tmp(j, i) = 0.;

        m = tmp;
        tmp = Ps * P;
        Ps = tmp;
    }
    if(loop == max_loop)
        throw std::logic_error("cannot solve with the tolerance");

    std::array<std::pair<Real, Vector_type>, N> retval;
    for(std::size_t i=0; i<N; ++i)
    {
        Vector_type eigen;
        for(std::size_t j=0; j<N; ++j)
        {
            eigen[j] = Ps(j, i);
        }
        retval[i] = std::make_pair(m(i, i), eigen);
    }
    return retval;
}


template<typename scalarT, std::size_t N>
bool JacobiEigenSolver::is_symmetric(const Matrix<scalarT, N, N>& mat) const
{
    for(std::size_t i=0; i<N-1; ++i)
    {
        for(std::size_t j=i+1; j<N; ++j)
        {
            if(std::abs(mat(i, j)/mat(j, i) - 1.) > relative_tolerance<scalarT>())
            {
                return false;
            }
        }
    }
    return true;
}

template<typename scalarT, std::size_t N>
std::pair<std::size_t, std::size_t>
JacobiEigenSolver::max_element(const Matrix<scalarT, N, N>& mat) const
{
    scalarT max_elem = std::abs(mat(0, 1));
    std::pair<std::size_t, std::size_t> retval = std::make_pair(0, 1);

    for(std::size_t i=0; i<N-1; ++i)
    {
        for(std::size_t j=i+1; j<N; ++j)
        {
            if(max_elem < std::abs(mat(i, j)))
            {
                max_elem = std::abs(mat(i, j));
                retval = std::make_pair(i, j);
            }
        }
    }
    return retval;
}

template<typename scalarT, std::size_t N>
scalarT JacobiEigenSolver::max_relative_diff(const Matrix<scalarT, N, N>& lhs,
        const Matrix<scalarT, N, N>& rhs) const
{
    scalarT retval = 0.0;
    for(std::size_t i=0; i<N; ++i)
    {
        const scalarT tmp = std::abs(lhs(i, i) / rhs(i, i));
        if(retval < tmp)
        {
            retval = tmp;
        }
    }
    return retval;
}

} // mill

#endif /* COFFEE_MILL_EIGEN_SOLVER */
