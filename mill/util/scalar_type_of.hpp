#ifndef COFFEE_MILL_UTIL_SCALAR_TYPE_OF_HPP
#define COFFEE_MILL_UTIL_SCALAR_TYPE_OF_HPP

namespace mill
{
template<typename vectorT>
struct scalar_type_of
{
    typedef typename vectorT::scalar_type type;
};
} // mill
#endif /* COFFEE_MILL_SCALAR_TYPE_EXTRACTOR */
