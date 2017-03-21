#ifndef COFFEE_MILL_SCALAR_TYPE_EXTRACTOR
#define COFFEE_MILL_SCALAR_TYPE_EXTRACTOR

namespace mill
{
template<typename vectorT>
struct scalar_type_of
{
    typedef typename vectorT::scalar_type type;
};

}

#endif /* COFFEE_MILL_SCALAR_TYPE_EXTRACTOR */
