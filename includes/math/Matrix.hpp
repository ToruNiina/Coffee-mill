#ifndef COFFEE_MILL_MATRIX
#define COFFEE_MILL_MATRIX
#include "detail/variadic_template.hpp"
#include <array>

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

  public:
    Matrix() : values_{{}}{}
    ~Matrix() = default;

    template<typename ... T_args, class = typename std::enable_if<
        (sizeof...(T_args) == number_of_element) &&
        is_all<std::is_convertible, realT, T_args...>::value>::type>
    Matrix(T_args ... args) : values_{{args...}}{}

    Matrix(const Matrix& mat) = default;
    Matrix& operator=(const Matrix& mat) = default;

    Matrix& operator+=(const Matrix& mat);
    Matrix& operator-=(const Matrix& mat);
    Matrix& operator*=(const scalar_type& scl);
    Matrix& operator/=(const scalar_type& scl);

    scalar_type  at(const std::size_t i, const std::size_t j) const;
    scalar_type& at(const std::size_t i, const std::size_t j);
    scalar_type  operator()(const std::size_t i, const std::size_t j) const;
    scalar_type& operator()(const std::size_t i, const std::size_t j);

    scalar_type  at(const std::size_t i) const {return values_.at(i);}
    scalar_type& at(const std::size_t i)       {return values_.at(i);}
    scalar_type  operator[](const std::size_t i) const {return values_[i];}
    scalar_type& operator[](const std::size_t i)       {return values_[i];}

  private:
    std::array<real_type, number_of_element> values_;
};

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>&
Matrix<realT, R, C>::operator+=(const Matrix<realT, R, C>& mat)
{
    for(std::size_t i=0; i<R; ++i)
        for(std::size_t j=0; j<C; ++j)
            (*this)(i, j) += mat(i, j);
    return *this;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>&
Matrix<realT, R, C>::operator-=(const Matrix<realT, R, C>& mat)
{
    for(std::size_t i=0; i<R; ++i)
        for(std::size_t j=0; j<C; ++j)
            (*this)(i, j) -= mat(i, j);
    return *this;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>&
Matrix<realT, R, C>::operator*=(const scalar_type& s)
{
    for(std::size_t i=0; i<R; ++i)
        for(std::size_t j=0; j<C; ++j)
            (*this)(i, j) *= s;
    return *this;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>&
Matrix<realT, R, C>::operator/=(const scalar_type& s)
{
    for(std::size_t i=0; i<R; ++i)
        for(std::size_t j=0; j<C; ++j)
            (*this)(i, j) /= s;
    return *this;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator+(const Matrix<realT, R, C>& lhs, const Matrix<realT, R, C>& rhs)
{
    Matrix<realT, R, C> retval;
    for(std::size_t i=0; i<R; ++i)
        for(std::size_t j=0; j<C; ++j)
            retval(i, j) = lhs(i, j) + rhs(i, j);
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator-(const Matrix<realT, R, C>& lhs, const Matrix<realT, R, C>& rhs)
{
    Matrix<realT, R, C> retval;
    for(std::size_t i=0; i<R; ++i)
        for(std::size_t j=0; j<C; ++j)
            retval(i, j) = lhs(i, j) - rhs(i, j);
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator*(const Matrix<realT, R, C>& lhs, const realT rhs)
{
    Matrix<realT, R, C> retval;
    for(std::size_t i=0; i<R; ++i)
        for(std::size_t j=0; j<C; ++j)
            retval(i, j) = lhs(i, j) * rhs;
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator*(const realT lhs, const Matrix<realT, R, C>& rhs)
{
    Matrix<realT, R, C> retval;
    for(std::size_t i=0; i<R; ++i)
        for(std::size_t j=0; j<C; ++j)
            retval(i, j) = rhs(i, j) * lhs;
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, R, C>
operator/(const Matrix<realT, R, C>& lhs, const realT rhs)
{
    Matrix<realT, R, C> retval;
    for(std::size_t i=0; i<R; ++i)
        for(std::size_t j=0; j<C; ++j)
            retval(i, j) = lhs(i, j) / rhs;
    return retval;
}

template<typename realT, std::size_t L, std::size_t M, std::size_t N>
Matrix<realT, L, N>
operator*(const Matrix<realT, L, M>& lhs, const Matrix<realT, M, N>& rhs)
{
    Matrix<realT, L, N> retval;
    for(std::size_t i=0; i < L; ++i)
        for(std::size_t j=0; j < N; ++j)
            for(std::size_t k=0; k < M; ++k)
                retval(i, j) += lhs(i, k) * rhs(k, j);
    return retval;
}

template<typename realT, std::size_t R, std::size_t C>
typename Matrix<realT, R, C>::scalar_type
Matrix<realT, R, C>::at(const std::size_t i, const std::size_t j) const
{
    return this->values_.at(i * C + j);
}

template<typename realT, std::size_t R, std::size_t C>
typename Matrix<realT, R, C>::scalar_type&
Matrix<realT, R, C>::at(const std::size_t i, const std::size_t j)
{
    return this->values_.at(i * C + j);
}

template<typename realT, std::size_t R, std::size_t C>
typename Matrix<realT, R, C>::scalar_type
Matrix<realT, R, C>::operator()(const std::size_t i, const std::size_t j) const
{
    return this->values_[i * C + j];
}

template<typename realT, std::size_t R, std::size_t C>
typename Matrix<realT, R, C>::scalar_type&
Matrix<realT, R, C>::operator()(const std::size_t i, const std::size_t j)
{
    return this->values_[i * C + j];
}

template<typename realT, std::size_t R, std::size_t C>
Matrix<realT, C, R> transpose(const Matrix<realT, R, C>& mat)
{
    Matrix<realT, C, R> retval;
    for(std::size_t i=0; i<R; ++i)
        for(std::size_t j=0; j<C; ++j)
            retval(j, i) = mat(i, j);
    return retval;
}

// determinant and inverse is not so efficient.

template<typename realT>
inline realT determinant(const Matrix<realT, 3, 3>& mat)
{
    return mat(0,0) * mat(1,1) * mat(2,2) +
           mat(1,0) * mat(2,1) * mat(0,2) +
           mat(2,0) * mat(0,1) * mat(1,2) -
           mat(0,0) * mat(2,1) * mat(1,2) -
           mat(2,0) * mat(1,1) * mat(0,2) -
           mat(1,0) * mat(0,1) * mat(2,2);
}

template<typename realT>
Matrix<realT, 3, 3> inverse(const Matrix<realT, 3, 3>& mat)
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
