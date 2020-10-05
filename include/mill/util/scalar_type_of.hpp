#ifndef COFFEE_MILL_UTIL_SCALAR_TYPE_OF_HPP
#define COFFEE_MILL_UTIL_SCALAR_TYPE_OF_HPP

namespace mill
{
template<typename vectorT>
struct scalar_type_of
{
    using type = typename vectorT::scalar_type;
};
template<typename vectorT>
using scalar_type_of_t = typename scalar_type_of<vectorT>::type;

} // mill
#endif /* COFFEE_MILL_SCALAR_TYPE_EXTRACTOR */
