#ifndef COFFEE_MILL_SCALAR_TYPE_EXTRACTOR
#define COFFEE_MILL_SCALAR_TYPE_EXTRACTOR
#include "math/LinearAlgebra.hpp"

namespace mill
{
template<typename vectorT>
struct scalar_type_extractor
{
    typedef typename vectorT::scalar_type type;
};

template<typename realT, std::size_t N>
struct scalar_type_extractor<ax::Vector<realT, N>>
{
    typedef realT type;
};

}

#endif /* COFFEE_MILL_SCALAR_TYPE_EXTRACTOR */
