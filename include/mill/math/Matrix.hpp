#ifndef COFFEE_MILL_MATH_MATRIX_HPP
#define COFFEE_MILL_MATH_MATRIX_HPP
#include <array>
#include <ostream>
#include <type_traits>

namespace mill
{

template<typename realT, std::size_t Row, std::size_t Col>
class Matrix
{
  public:
    using real_type = realT;
    using scalar_type = real_type;
    constexpr static std::size_t dim_row = Row;
    constexpr static std::size_t dim_col = Col;
    constexpr static std::size_t number_of_element = Row * Col;
    using container_type = std::array<real_type, number_of_element>;

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
        static_assert(sizeof...(Ts) == number_of_element);
        static_assert(std::conjunction_v<std::is_convertible<Ts, realT>...>);
    }

    Matrix(std::initializer_list<std::initializer_list<real_type>> il)
    {
        assert(il.size() == dim_row);
        std::size_t i=0;
        for(auto&& r : il)
        {
            assert(r.size() == dim_col);
            for(auto&& e : r)
            {
                values_[i] = e;
                ++i;
            }
        }
    }

    Matrix& operator+=(const Matrix& mat)
    {
        for(std::size_t i=0; i<number_of_element; ++i)
        {
            this->values_[i] += mat[i];
        }
        return *this;
    }
    Matrix& operator-=(const Matrix& mat)
    {
        for(std::size_t i=0; i<number_of_element; ++i)
        {
            this->values_[i] -= mat[i];
        }
        return *this;
    }

    Matrix& operator*=(const scalar_type s)
    {
        for(std::size_t i=0; i<number_of_element; ++i)
        {
            this->values_[i] *= s;
        }
        return *this;
    }
    Matrix& operator/=(const scalar_type s)
    {
        for(std::size_t i=0; i<number_of_element; ++i)
        {
            this->values_[i] /= s;
        }
        return *this;
    }

    scalar_type  at(const std::size_t i, const std::size_t j) const
    {
        return this->values_.at(i*dim_col + j);
    }
    scalar_type& at(const std::size_t i, const std::size_t j)
    {
        return this->values_.at(i*dim_col + j);
    }

    scalar_type  operator()(const std::size_t i, const std::size_t j) const noexcept
    {
        return this->values_[i*dim_col + j];
    }
    scalar_type& operator()(const std::size_t i, const std::size_t j)       noexcept
    {
        return this->values_[i*dim_col + j];
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

template<typename realT, std::size_t Row, std::size_t Col>
constexpr std::size_t Matrix<realT, Row, Col>::dim_row;
template<typename realT, std::size_t Row, std::size_t Col>
constexpr std::size_t Matrix<realT, Row, Col>::dim_col;
template<typename realT, std::size_t Row, std::size_t Col>
constexpr std::size_t Matrix<realT, Row, Col>::number_of_element;

template<typename realT, std::size_t R, std::size_t C>
typename std::enable_if_t<C != 1, std::ostream>::type&
operator<<(std::ostream& os, const Matrix<realT, R, C>& mat)
{
    for(std::size_t i=0; i<R; ++i)
    {
        os << "[ ";
        for(std::size_t j=0; j<C; ++j)
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
    for(std::size_t j=0; j<R; ++j)
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
    Matrix<realT, R, C> retval;
    for(std::size_t i=0; i<R*C; ++i)
    {
        retval[i] = lhs[i] + rhs[i];
    }
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator-(const Matrix<realT, R, C>& lhs, const Matrix<realT, R, C>& rhs)
{
    Matrix<realT, R, C> retval;
    for(std::size_t i=0; i<R*C; ++i)
    {
        retval[i] = lhs[i] - rhs[i];
    }
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator*(const Matrix<realT, R, C>& lhs, const realT rhs)
{
    Matrix<realT, R, C> retval;
    for(std::size_t i=0; i<R*C; ++i)
    {
        retval[i] = lhs[i] * rhs;
    }
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator*(const realT lhs, const Matrix<realT, R, C>& rhs)
{
    Matrix<realT, R, C> retval;
    for(std::size_t i=0; i<R*C; ++i)
    {
        retval[i] = lhs * rhs[i];
    }
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator/(const Matrix<realT, R, C>& lhs, const realT rhs)
{
    Matrix<realT, R, C> retval;
    for(std::size_t i=0; i<R*C; ++i)
    {
        retval[i] = lhs[i] / rhs;
    }
    return retval;
}

template<typename realT, std::size_t L, std::size_t M, std::size_t N>
Matrix<realT, L, N>
operator*(const Matrix<realT, L, M>& lhs, const Matrix<realT, M, N>& rhs)
{
    Matrix<realT, L, N> retval;
    for(std::size_t i=0; i < L; ++i)
    {
        for(std::size_t j=0; j < N; ++j)
        {
            for(std::size_t k=0; k < M; ++k)
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
    Matrix<realT, C, R> retval;
    for(std::size_t i=0; i<R; ++i)
    {
        for(std::size_t j=0; j<C; ++j)
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
