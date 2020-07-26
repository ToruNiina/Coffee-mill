#ifndef COFFEE_MILL_MATH_RMSD_CALCULATOR_HPP
#define COFFEE_MILL_MATH_RMSD_CALCULATOR_HPP
#include "Vector.hpp"
#include <vector>

namespace mill
{

template<typename realT, std::size_t N>
realT rmsd(const std::vector<Vector<realT, N>>& lhs,
           const std::vector<Vector<realT, N>>& rhs)
{
    const std::size_t s = lhs.size();
    if(s != rhs.size())
    {
        throw std::invalid_argument("rmsd: different size");
    }

    realT sigma = 0.;
    for(std::size_t i=0; i<s; ++i)
    {
        sigma += length_sq(lhs[i] - rhs[i]);
    }
    return std::sqrt(sigma / static_cast<realT>(s));
}

}// mill

#endif //COFFEE_MILL_RMSD_CALCLATOR
