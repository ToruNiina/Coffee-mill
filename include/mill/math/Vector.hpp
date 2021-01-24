#ifndef COFFEE_MILL_MATH_VECTOR_HPP
#define COFFEE_MILL_MATH_VECTOR_HPP
#include "Matrix.hpp"
#include <algorithm>
#include <cmath>

namespace mill
{

template<typename realT>
class Matrix<realT, 0, 1>
{
  public:
    using real_type = realT;
    using scalar_type = real_type;
    using container_type = std::vector<real_type>;

    static Matrix zero(const std::size_t len)
    {
        return Matrix<realT, 0, 1>({len, 1});
    }

  public:

    Matrix() : row_(0) {}
    ~Matrix() = default;

    Matrix(Matrix const& mat) = default;
    Matrix(Matrix&&      mat) = default;
    Matrix& operator=(Matrix const& mat) = default;
    Matrix& operator=(Matrix&&      mat) = default;

    Matrix(std::pair<std::size_t, std::size_t> size)
        : row_(size.first), values_(size.first * size.second, 0.0)
    {
        assert(size.second == 1u);
    }

    Matrix& operator+=(const Matrix& mat)
    {
        assert(this->row() == mat.row());
        assert(this->col() == mat.col());
        for(std::size_t i=0; i<this->len(); ++i)
        {
            this->values_[i] += mat[i];
        }
        return *this;
    }
    Matrix& operator-=(const Matrix& mat)
    {
        assert(this->row() == mat.row());
        assert(this->col() == mat.col());
        for(std::size_t i=0; i<this->len(); ++i)
        {
            this->values_[i] -= mat[i];
        }
        return *this;
    }

    Matrix& operator*=(const scalar_type s)
    {
        for(std::size_t i=0; i<this->len(); ++i)
        {
            this->values_[i] *= s;
        }
        return *this;
    }
    Matrix& operator/=(const scalar_type s)
    {
        for(std::size_t i=0; i<this->len(); ++i)
        {
            this->values_[i] /= s;
        }
        return *this;
    }

    constexpr std::size_t len() const noexcept {return this->values_.size();}
    constexpr std::size_t row() const noexcept {return this->row_;}
    constexpr std::size_t col() const noexcept {return 1;}

    scalar_type  at(const std::size_t i, const std::size_t j) const
    {
        return this->values_.at(i + j);
    }
    scalar_type& at(const std::size_t i, const std::size_t j)
    {
        return this->values_.at(i + j);
    }

    scalar_type  operator()(const std::size_t i, const std::size_t j) const noexcept
    {
        return this->values_[i + j];
    }
    scalar_type& operator()(const std::size_t i, const std::size_t j)       noexcept
    {
        return this->values_[i + j];
    }

    scalar_type  at(const std::size_t i) const {return values_.at(i);}
    scalar_type& at(const std::size_t i)       {return values_.at(i);}
    scalar_type  operator[](const std::size_t i) const noexcept {return values_[i];}
    scalar_type& operator[](const std::size_t i)       noexcept {return values_[i];}

  private:

    std::size_t row_;
    /* {a11, a12, ..., a1C, *
     *  a21, a22, ..., a2C, *
     *  aR1, aR2, ..., aRC} */
    container_type values_;
};

template<typename realT, std::size_t N>
using Vector = Matrix<realT, N, 1>;

template<typename realT, std::size_t N>
realT dot_product(const Vector<realT, N>& lhs, const Vector<realT, N>& rhs) noexcept
{
    assert(lhs.len() == rhs.len());
    realT sum = 0.0;
    for(std::size_t i=0; i<lhs.len(); ++i)
    {
        sum += lhs[i] * rhs[i];
    }
    return sum;
}

template<typename realT>
Vector<realT, 3>
cross_product(const Vector<realT, 3>& lhs, const Vector<realT, 3>& rhs) noexcept
{
    return Vector<realT, 3>(lhs[1] * rhs[2] - lhs[2] * rhs[1],
                            lhs[2] * rhs[0] - lhs[0] * rhs[2],
                            lhs[0] * rhs[1] - lhs[1] * rhs[0]);
}

template<typename realT>
realT scalar_triple_product(const Vector<realT, 3>& lhs,
        const Vector<realT, 3>& mid, const Vector<realT, 3>& rhs) noexcept
{
    return (lhs[1] * mid[2] - lhs[2] * mid[1]) * rhs[0] +
           (lhs[2] * mid[0] - lhs[0] * mid[2]) * rhs[1] +
           (lhs[0] * mid[1] - lhs[1] * mid[0]) * rhs[2];
}

template<typename realT>
realT length_sq(const Vector<realT, 3>& lhs) noexcept
{
    return lhs[0] * lhs[0] + lhs[1] * lhs[1] + lhs[2] * lhs[2];
}

template<typename realT>
realT length(const Vector<realT, 3>& lhs) noexcept
{
    return std::sqrt(length_sq(lhs));
}

template<typename realT>
Vector<realT, 3> regularize(const Vector<realT, 3>& v) noexcept
{
    return v * (1.0 / length(v));
}

template<typename realT>
realT angle(const Vector<realT, 3>& lhs, const Vector<realT, 3>& rhs) noexcept
{
    const auto l_reg = regularize(lhs);
    const auto r_reg = regularize(rhs);
    return std::acos(std::clamp<realT>(dot_product(l_reg, r_reg), -1, 1));
}

} // mill
#endif /* COFFEE_MILL_VECTOR */
