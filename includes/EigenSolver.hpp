#ifndef COFFEE_MILL_EIGEN_SOLVER
#define COFFEE_MILL_EIGEN_SOLVER

namespace mill
{

class JacobiEigenSolver
{
  public:
    constexpr static std::size_t max_loop = 10000;

  public:
    JacobiMethod() = default;
    ~JacobiMethod() = default;

    template<typename scalarT, std::size_t N,
             template<typename sT, std::size_t R, std::size_t C> class matrixT>
    std::array<std::pair<scalarT, matrixT<scalarT, N, 1>>, N>
    solve(const matrixT<scalarT, N, N>& mat) const;

  private:
    
    template<typename scalarT, std::size_t N,
             template<typename sT, std::size_t R, std::size_t C> class matrixT>
    bool is_symmetric(const matrixT<scalarT, N, N>& mat) const;

    template<typename scalarT, std::size_t N,
             template<typename sT, std::size_t R, std::size_t C> class matrixT>
    std::pair<std::size_t, std::size_t>
    max_element(const matrixT<scalarT, N, N>& mat) const;

    template<typename scalarT, std::size_t N,
             template<typename sT, std::size_t R, std::size_t C> class matrixT>
    scalarT max_relative_diff(const matrixT<scalarT, N, N>& lhs,
                              const matrixT<scalarT, N, N>& rhs) const;

    template<typename scalarT>
    static scalarT relative_tolerance();
    template<typename scalarT>
    static scalarT absolute_tolerance();
};

template<>
double JacobiEigenSolver::relative_tolerance<double>(){return 1e-12;}
template<>
float  JacobiEigenSolver::relative_tolerance<float>(){return 1e-5;}

template<>
double JacobiEigenSolver::absolute_tolerance<double>(){return 1e-12;}
template<>
float  JacobiEigenSolver::absolute_tolerance<float>(){return 1e-5;}

template<typename scalarT, std::size_t N,
         template<typename sT, std::size_t R, std::size_t C> class matrixT>
std::array<std::pair<scalarT, matrixT<scalarT, N, 1>>, N>
JacobiEigenSolver::solve(const matrixT<scalarT, N, N>& mat) const
{
    if(not this->is_symmetric(mat))
        throw std::invalid_argument("asymmetric matrix");

    typedef matrixT<scalarT, N, N> Matrix;
    typedef matrixT<scalarT, N, 1> Vector;
    typedef scalarT                Real;

    Matrix m = mat;
    Matrix Ps;
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

        Matrix P;
        for(std::size_t i=0; i<N; ++i)
            for(std::size_t j=0; j<N; ++j)
                P(i, j) = (i == j) ? 1. : 0.;
        P(index.first,  index.first)  =  cos_t;
        P(index.first,  index.second) =  sin_t;
        P(index.second, index.first)  = -sin_t;
        P(index.second, index.second) =  cos_t;

        Matrix tmp = transpose(P) * m * P;
        tmp(index.first,  index.second) = 0.;
        tmp(index.second, index.first)  = 0.;

        if(max_relative_diff(m, tmp) < relative_tolerance<Real>()) break;

        m = tmp;
        tmp = P * Ps;
        Ps = tmp;
    }
    if(loop == max_loop)
        throw std::logic_error("cannot solve with the tolerance");

    std::array<std::pair<Real, Vector>, N> retval;
    for(std::size_t i=0; i<N; ++i)
    {
        Vector eigen;
        for(std::size_t j=0; j<N; ++j)
            eigen[j] = Ps(j, i);
        retval[i] = std::make_pair(m(i, i), eigen);
    }
    return retval;
}


template<typename scalarT, std::size_t N,
         template<typename sT, std::size_t R, std::size_t C> class matrixT>
bool JacobiEigenSolver::is_symmetric(const matrixT<scalarT, N, N>& mat) const
{
    for(std::size_t i=0; i<N-1; ++i)
    for(std::size_t j=i+1; j<N; ++j)
        if(std::abs(mat(i, j)/mat(j, i) - 1.) > relative_tolerance<scalarT>())
            return false;
    return true;
}

template<typename scalarT, std::size_t N,
         template<typename sT, std::size_t R, std::size_t C> class matrixT>
std::pair<std::size_t, std::size_t>
JacobiEigenSolver::max_element(const matrixT<scalarT, N, N>& mat) const
{
    scalarT max_elem = -1. * std::numeric_limits<scalarT>::max();
    std::pair<std::size_t, std::size_t> retval;
    for(std::size_t i=0; i<N; ++i)
    for(std::size_t j=i; j<N; ++j)
        if(max_elem < mat(i, j))
        {
            max_elem = mat(i, j);
            retval = std::make_pair(i, j);
        }
    return retval;
}

template<typename scalarT, std::size_t N,
         template<typename sT, std::size_t R, std::size_t C> class matrixT>
scalarT JacobiEigenSolver::max_relative_diff(const matrixT<scalarT, N, N>& lhs,
        const matrixT<scalarT, N, N>& rhs) const
{
    scalarT retval = 0.0;
    for(std::size_t i=0; i<N; ++i)
    {
        const scalar tmp = std::abs(lhs(i, i) / rhs(i, i));
        if(retval < tmp) retval = tmp;
    }
    return retval;
}



} // mill

#endif /* COFFEE_MILL_EIGEN_SOLVER */
