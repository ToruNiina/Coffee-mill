#ifndef COFFEE_MILL_MATH_MATRIX_HPP
#define COFFEE_MILL_MATH_MATRIX_HPP
#include <array>
#include <ostream>
#include <type_traits>
#include <vector>

#include <cassert>

namespace mill
{

template<typename realT, std::size_t Row, std::size_t Col>
class Matrix
{
  public:
    using real_type = realT;
    using scalar_type = real_type;
    using container_type = std::array<real_type, Row * Col>;

    static Matrix zero() noexcept
    {
        return Matrix<realT, Row, Col>();
    }

    static Matrix identity() noexcept
    {
        Matrix<realT, Row, Col> I;
        for(std::size_t i=0, e=std::min(Row, Col); i<e; ++i)
        {
            I(i, i) = 1.0;
        }
        return I;
    }

    static Matrix zero(std::size_t r, std::size_t c) noexcept
    {
        return Matrix<realT, Row, Col>(std::make_pair(r, c));
    }

    static Matrix identity(std::size_t r, std::size_t c) noexcept
    {
        Matrix<realT, Row, Col> I(std::make_pair(r, c));
        for(std::size_t i=0, e=std::min(Row, Col); i<e; ++i)
        {
            I(i, i) = 1.0;
        }
        return I;
    }

  public:

    Matrix(){values_.fill(0);}
    ~Matrix() = default;

    Matrix(Matrix const& mat) = default;
    Matrix(Matrix&&      mat) = default;
    Matrix& operator=(Matrix const& mat) = default;
    Matrix& operator=(Matrix&&      mat) = default;

    template<typename ... Ts>
    Matrix(Ts ... args) : values_{{static_cast<real_type>(args)...}}
    {
        static_assert(sizeof...(Ts) == Row * Col);
        static_assert(std::conjunction_v<std::is_convertible<Ts, realT>...>);
    }

    Matrix(std::pair<std::size_t, std::size_t> size)
    {
        assert(size.first  == this->row());
        assert(size.second == this->col());
        (void)size;
        values_.fill(0);
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

    constexpr std::size_t len() const noexcept {return values_.size();}
    constexpr std::size_t row() const noexcept {return Row;}
    constexpr std::size_t col() const noexcept {return Col;}

    scalar_type  at(const std::size_t i, const std::size_t j) const
    {
        return this->values_.at(i*Col + j);
    }
    scalar_type& at(const std::size_t i, const std::size_t j)
    {
        return this->values_.at(i*Col + j);
    }

    scalar_type  operator()(const std::size_t i, const std::size_t j) const noexcept
    {
        return this->values_[i*Col + j];
    }
    scalar_type& operator()(const std::size_t i, const std::size_t j)       noexcept
    {
        return this->values_[i*Col + j];
    }

    scalar_type  at(const std::size_t i) const {return values_.at(i);}
    scalar_type& at(const std::size_t i)       {return values_.at(i);}
    scalar_type  operator[](const std::size_t i) const noexcept {return values_[i];}
    scalar_type& operator[](const std::size_t i)       noexcept {return values_[i];}

  private:
    /* {a11, a12, ..., a1C, *
     *  a21, a22, ..., a2C, *
     *  aR1, aR2, ..., aRC} */
    container_type values_;
};

constexpr inline std::size_t DYNAMIC = 0;

template<typename realT>
class Matrix<realT, 0, 0>
{
  public:
    using real_type = realT;
    using scalar_type = real_type;
    using container_type = std::vector<real_type>;

    static Matrix zero() noexcept
    {
        return Matrix<realT, 0, 0>();
    }

    static Matrix identity() noexcept
    {
        return Matrix<realT, 0, 0>();
    }

    static Matrix zero(std::size_t r, std::size_t c) noexcept
    {
        return Matrix<realT, 0, 0>(std::make_pair(r, c));
    }

    static Matrix identity(std::size_t r, std::size_t c) noexcept
    {
        Matrix<realT, 0, 0> I(std::make_pair(r, c));
        for(std::size_t i=0, e=std::min(r, c); i<e; ++i)
        {
            I(i, i) = 1.0;
        }
        return I;
    }

  public:

    Matrix() : row_(0), col_(0) {}
    ~Matrix() = default;

    Matrix(Matrix const& mat) = default;
    Matrix(Matrix&&      mat) = default;
    Matrix& operator=(Matrix const& mat) = default;
    Matrix& operator=(Matrix&&      mat) = default;

    Matrix(std::pair<std::size_t, std::size_t> size)
        : row_(size.first), col_(size.second), values_(size.first * size.second, 0.0)
    {}

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
    constexpr std::size_t col() const noexcept {return this->col_;}

    scalar_type  at(const std::size_t i, const std::size_t j) const
    {
        return this->values_.at(i*col_ + j);
    }
    scalar_type& at(const std::size_t i, const std::size_t j)
    {
        return this->values_.at(i*col_ + j);
    }

    scalar_type  operator()(const std::size_t i, const std::size_t j) const noexcept
    {
        return this->values_[i*col_ + j];
    }
    scalar_type& operator()(const std::size_t i, const std::size_t j)       noexcept
    {
        return this->values_[i*col_ + j];
    }

    scalar_type  at(const std::size_t i) const {return values_.at(i);}
    scalar_type& at(const std::size_t i)       {return values_.at(i);}
    scalar_type  operator[](const std::size_t i) const noexcept {return values_[i];}
    scalar_type& operator[](const std::size_t i)       noexcept {return values_[i];}

  private:

    std::size_t row_;
    std::size_t col_;
    /* {a11, a12, ..., a1C, *
     *  a21, a22, ..., a2C, *
     *  aR1, aR2, ..., aRC} */
    container_type values_;
};

template<typename realT, std::size_t R, std::size_t C>
typename std::enable_if_t<C != 1, std::ostream>::type&
operator<<(std::ostream& os, const Matrix<realT, R, C>& mat)
{
    for(std::size_t i=0; i<mat.row(); ++i)
    {
        os << "[ ";
        for(std::size_t j=0; j<mat.col(); ++j)
        {
            os << mat(i, j) << ' ';
        }
        os << "]\n";
    }
    return os;
}
template<typename realT, std::size_t R>
std::ostream& operator<<(std::ostream& os, const Matrix<realT, R, 1>& mat)
{
    os << "[ ";
    for(std::size_t j=0; j<mat.row(); ++j)
    {
        os << mat(0, j) << ' ';
    }
    os << ']';
    return os;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator+(const Matrix<realT, R, C>& lhs, const Matrix<realT, R, C>& rhs)
{
    assert(lhs.row() == rhs.row());
    assert(lhs.col() == rhs.col());

    Matrix<realT, R, C> retval(std::make_pair(lhs.row(), lhs.col()));
    for(std::size_t i=0; i<lhs.len(); ++i)
    {
        retval[i] = lhs[i] + rhs[i];
    }
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator-(const Matrix<realT, R, C>& lhs, const Matrix<realT, R, C>& rhs)
{
    assert(lhs.row() == rhs.row());
    assert(lhs.col() == rhs.col());

    Matrix<realT, R, C> retval(std::make_pair(lhs.row(), lhs.col()));
    for(std::size_t i=0; i<lhs.len(); ++i)
    {
        retval[i] = lhs[i] - rhs[i];
    }
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator*(const Matrix<realT, R, C>& lhs, const realT rhs)
{
    Matrix<realT, R, C> retval(std::make_pair(lhs.row(), lhs.col()));
    for(std::size_t i=0; i<lhs.len(); ++i)
    {
        retval[i] = lhs[i] * rhs;
    }
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator*(const realT lhs, const Matrix<realT, R, C>& rhs)
{
    Matrix<realT, R, C> retval(std::make_pair(rhs.row(), rhs.col()));
    for(std::size_t i=0; i<rhs.len(); ++i)
    {
        retval[i] = lhs * rhs[i];
    }
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator/(const Matrix<realT, R, C>& lhs, const realT rhs)
{
    Matrix<realT, R, C> retval(std::make_pair(lhs.row(), lhs.col()));
    for(std::size_t i=0; i<lhs.len(); ++i)
    {
        retval[i] = lhs[i] / rhs;
    }
    return retval;
}

template<typename realT, std::size_t L, std::size_t M, std::size_t N>
Matrix<realT, L, N>
operator*(const Matrix<realT, L, M>& lhs, const Matrix<realT, M, N>& rhs)
{
    assert(lhs.col() == rhs.row());

    Matrix<realT, L, N> retval(std::make_pair(lhs.row(), rhs.col()));
    for(std::size_t i=0; i < lhs.row(); ++i)
    {
        for(std::size_t j=0; j < rhs.col(); ++j)
        {
            for(std::size_t k=0; k < lhs.col(); ++k)
            {
                retval(i, j) += lhs(i, k) * rhs(k, j);
            }
        }
    }
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, C, R> transpose(const Matrix<realT, R, C>& mat)
{
    Matrix<realT, C, R> retval(std::make_pair(mat.col(), mat.row()));
    for(std::size_t i=0; i<mat.row(); ++i)
    {
        for(std::size_t j=0; j<mat.col(); ++j)
        {
            retval(j, i) = mat(i, j);
        }
    }
    return retval;
}

// determinant and inverse is not so efficient.

template<typename realT>
inline realT determinant(const Matrix<realT, 3, 3>& mat) noexcept
{
    return mat(0,0) * mat(1,1) * mat(2,2) +
           mat(1,0) * mat(2,1) * mat(0,2) +
           mat(2,0) * mat(0,1) * mat(1,2) -
           mat(0,0) * mat(2,1) * mat(1,2) -
           mat(2,0) * mat(1,1) * mat(0,2) -
           mat(1,0) * mat(0,1) * mat(2,2);
}

template<typename realT>
Matrix<realT, 3, 3> inverse(const Matrix<realT, 3, 3>& mat) noexcept
{
    const auto det_inv = 1e0 / determinant(mat);

    Matrix<realT, 3, 3> inv;
    inv(0,0) = det_inv * (mat(1,1) * mat(2,2) - mat(1,2) * mat(2,1));
    inv(1,1) = det_inv * (mat(0,0) * mat(2,2) - mat(0,2) * mat(2,0));
    inv(2,2) = det_inv * (mat(0,0) * mat(1,1) - mat(0,1) * mat(1,0));

    inv(0,1) = det_inv * (mat(0,2) * mat(2,1) - mat(0,1) * mat(2,2));
    inv(0,2) = det_inv * (mat(0,1) * mat(1,2) - mat(0,2) * mat(1,1));
    inv(1,2) = det_inv * (mat(0,2) * mat(1,0) - mat(0,0) * mat(1,2));

    inv(1,0) = det_inv * (mat(1,2) * mat(2,0) - mat(1,0) * mat(2,2));
    inv(2,0) = det_inv * (mat(1,0) * mat(2,1) - mat(2,0) * mat(1,1));
    inv(2,1) = det_inv * (mat(2,0) * mat(0,1) - mat(0,0) * mat(2,1));
    return inv;
}

} // mill
#endif /* COFFEE_MILL_MATRIX */
