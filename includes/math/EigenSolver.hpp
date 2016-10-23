#ifndef COFFEE_MILL_EIGEN_SOLVER
#define COFFEE_MILL_EIGEN_SOLVER
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
    static scalarT relative_tolerance();
    template<typename scalarT>
    static scalarT absolute_tolerance();
};

template<>
double JacobiEigenSolver::relative_tolerance<double>(){return 1e-13;}
template<>
float  JacobiEigenSolver::relative_tolerance<float>(){return 1e-5;}

template<>
double JacobiEigenSolver::absolute_tolerance<double>(){return 1e-13;}
template<>
float  JacobiEigenSolver::absolute_tolerance<float>(){return 1e-5;}

template<typename scalarT, std::size_t N>
std::array<std::pair<scalarT, Vector<scalarT, N>>, N>
JacobiEigenSolver::solve(const Matrix<scalarT, N, N>& mat) const
{
    if(not this->is_symmetric(mat))
        throw std::invalid_argument("asymmetric matrix");

    typedef Matrix<scalarT, N, N> Matrix_type;
    typedef Matrix<scalarT, N, 1> Vector_type;
    typedef scalarT               Real;

    Matrix_type m = mat;
    Matrix_type Ps;
    for(std::size_t i=0; i<N; ++i)
        for(std::size_t j=0; j<N; ++j)
            Ps(i, j) = (i == j) ? 1. : 0.;

    std::size_t loop=0;
    for(; loop < max_loop; ++loop)
    {
        const auto index = this->max_element(m);
        if(std::abs(m(index.first, index.second)) < absolute_tolerance<Real>())
            break;

        const Real alpha = (m(index.first, index.first) -
                            m(index.second, index.second)) * 0.5;
        const Real beta  = -1. * m(index.first, index.second);
        const Real gamma = std::abs(alpha) / std::sqrt(alpha * alpha + beta * beta);
        const Real cos_t = std::sqrt(0.5 + gamma * 0.5);
        const Real sin_t = std::copysign(std::sqrt(0.5 - gamma * 0.5), alpha * beta);

        Matrix_type P;
        for(std::size_t i=0; i<N; ++i)
            for(std::size_t j=0; j<N; ++j)
                P(i, j) = (i == j) ? 1. : 0.;
        P(index.first,  index.first)  =  cos_t;
        P(index.first,  index.second) =  sin_t;
        P(index.second, index.first)  = -sin_t;
        P(index.second, index.second) =  cos_t;

        Matrix_type tmp = transpose(P) * m * P;
        if(this->max_relative_diff(m, tmp) < relative_tolerance<Real>()) break;
        tmp(index.first,  index.second) = 0.;
        tmp(index.second, index.first)  = 0.;

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
            eigen[j] = Ps(j, i);
        retval[i] = std::make_pair(m(i, i), eigen);
    }
    return retval;
}


template<typename scalarT, std::size_t N>
bool JacobiEigenSolver::is_symmetric(const Matrix<scalarT, N, N>& mat) const
{
    for(std::size_t i=0; i<N-1; ++i)
    for(std::size_t j=i+1; j<N; ++j)
        if(std::abs(mat(i, j)/mat(j, i) - 1.) > relative_tolerance<scalarT>())
            return false;
    return true;
}

template<typename scalarT, std::size_t N>
std::pair<std::size_t, std::size_t>
JacobiEigenSolver::max_element(const Matrix<scalarT, N, N>& mat) const
{
    scalarT max_elem = std::abs(mat(0, 1));
    std::pair<std::size_t, std::size_t> retval = std::make_pair(0, 1);

    for(std::size_t i=0; i<N-1; ++i)
        for(std::size_t j=i+1; j<N; ++j)
            if(max_elem < std::abs(mat(i, j)))
            {
                max_elem = std::abs(mat(i, j));
                retval = std::make_pair(i, j);
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
        if(retval < tmp) retval = tmp;
    }
    return retval;
}

} // mill

#endif /* COFFEE_MILL_EIGEN_SOLVER */
