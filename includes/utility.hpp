#ifndef COFFEE_MILL_UTILITY
#define COFFEE_MILL_UTILITY
#include <string>

namespace coffeemill
{

template<char del>
inline std::string remove_all(const std::string& str)
{
    std::string retval;
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        if(*iter != del) retval += *iter;
    return retval;
}

}

#endif /* COFFEE_MILL_UTILITY */
