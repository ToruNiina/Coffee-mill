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

    template<typename realT, std::size_t N>
    std::array<std::pair<realT, Vector<realT, N>>, N>
    solve(Matrix<realT, N, N> mat) const;

  private:

    template<typename realT, std::size_t N>
    bool is_symmetric(const Matrix<realT, N, N>& mat) const;

    template<typename realT, std::size_t N>
    std::pair<std::size_t, std::size_t>
    max_element(const Matrix<realT, N, N>& mat) const;

    template<typename realT, std::size_t N>
    realT max_relative_diff(const Matrix<realT, N, N>& lhs,
                              const Matrix<realT, N, N>& rhs) const;

    template<typename realT>
    static constexpr realT relative_tolerance()
    {
        if constexpr (std::is_same_v<realT, double>) {return 1e-13;}
        else {return 1e-5f;}
    }

    template<typename realT>
    static constexpr realT absolute_tolerance()
    {
        if constexpr (std::is_same_v<realT, double>) {return 1e-13;}
        else {return 1e-5f;}
    }
};

template<typename realT, std::size_t N>
std::array<std::pair<realT, Vector<realT, N>>, N>
JacobiEigenSolver::solve(Matrix<realT, N, N> m) const
{
    constexpr auto abs_tol = absolute_tolerance<realT>();
    constexpr auto rel_tol = relative_tolerance<realT>();
    constexpr std::size_t max_loop = 10000;

    assert(mat.row() == mat.col());

    if(not this->is_symmetric(m))
    {
        throw std::invalid_argument("JacobiEigenSolver: asymmetric matrix");
    }

    auto Ps = Matrix<realT, N, N>::identity(m.row(), m.col());

    std::size_t loop = 0;
    for(; loop < max_loop; ++loop)
    {
        const auto [i, j] = this->max_element(m);
        if(std::abs(m(i, j)) < abs_tol)
        {
            break;
        }

        const auto alpha = (m(i, i) - m(j, j)) * 0.5;
        const auto beta  = -1.0 * m(i, j);
        const auto gamma = std::abs(alpha) / std::sqrt(alpha * alpha + beta * beta);
        const auto cos_t = std::sqrt(0.5 + gamma * 0.5);
        const auto sin_t = std::copysign(std::sqrt(0.5 - gamma * 0.5), alpha * beta);

        auto P = Matrix<realT, N, N>::identity(m.row(), m.col());
        P(i, i) =  cos_t;
        P(i, j) =  sin_t;
        P(j, i) = -sin_t;
        P(j, j) =  cos_t;

        auto tmp = transpose(P) * m * P;
        if(this->max_relative_diff(m, tmp) < rel_tol)
        {
            break;
        }
        tmp(i, j) = 0.0;
        tmp(j, i) = 0.0;

        m   = tmp;
        tmp = Ps * P;
        Ps  = tmp;
    }
    if(loop == max_loop)
    {
        throw std::runtime_error("JacobiEigenSolver: cannot solve with the tolerance");
    }

    std::array<std::pair<realT, Vector<realT, N>>, N> retval;
    for(std::size_t i=0; i<Ps.row(); ++i)
    {
        Vector<realT, N> eigen(std::make_pair(Ps.col(), std::size_t(1)));
        for(std::size_t j=0; j<Ps.col(); ++j)
        {
            eigen[j] = Ps(j, i);
        }
        retval[i] = std::make_pair(m(i, i), eigen);
    }
    return retval;
}

template<typename realT, std::size_t N>
bool JacobiEigenSolver::is_symmetric(const Matrix<realT, N, N>& mat) const
{
    assert(mat.row() == mat.col());
    constexpr auto rel_tol = relative_tolerance<realT>();
    for(std::size_t i=0; i<mat.row()-1; ++i)
    {
        for(std::size_t j=i+1; j<mat.col(); ++j)
        {
            if(std::abs(mat(i, j) / mat(j, i) - 1.0) > rel_tol)
            {
                return false;
            }
        }
    }
    return true;
}

template<typename realT, std::size_t N>
std::pair<std::size_t, std::size_t>
JacobiEigenSolver::max_element(const Matrix<realT, N, N>& mat) const
{
    assert(mat.row() == mat.col());
    realT max_elem = std::abs(mat(0, 1));
    std::pair<std::size_t, std::size_t> retval = std::make_pair(0, 1);

    for(std::size_t i=0; i<mat.row()-1; ++i)
    {
        for(std::size_t j=i+1; j<mat.col(); ++j)
        {
            if(max_elem < std::abs(mat(i, j)))
            {
                max_elem = std::abs(mat(i, j));
                retval   = std::make_pair(i, j);
            }
        }
    }
    return retval;
}

template<typename realT, std::size_t N>
realT JacobiEigenSolver::max_relative_diff(const Matrix<realT, N, N>& lhs,
        const Matrix<realT, N, N>& rhs) const
{
    assert(mat.row() == mat.col());
    realT retval = 0.0;
    for(std::size_t i=0; i<mat.row(); ++i)
    {
        const realT tmp = std::abs(lhs(i, i) / rhs(i, i) - 1.0);
        if(retval < tmp)
        {
            retval = tmp;
        }
    }
    return retval;
}

} // mill

#endif /* COFFEE_MILL_EIGEN_SOLVER */
