#ifndef COFFEE_MILL_DEFAULT_TRAITS
#define COFFEE_MILL_DEFAULT_TRAITS
#include "math/LinearAlgebra.hpp"
#include <string>

namespace coffeemill
{

struct DefaultTraits
{
    typedef char_type      char;
    typedef string_type    std::stirng;
    typedef int_type       int;
    typedef size_type      std::size_t;
    typedef index_type     size_type;
    typedef real_type      double;
    typedef position_type  ax::Vector<real_type, 3>;
    typedef direction_type ax::Vector<real_type, 3>;
};

}// coffeemill

#endif /* COFFEE_MILL_DEFAULT_TRAITS */
