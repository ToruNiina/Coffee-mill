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

template<class T_target, class T_reference>
bool is_composed_of(const T_target& target, const T_reference& ref)
{
    for(auto iter = target.cbegin(); iter != target.cend(); ++iter)
    {
        bool found = false;
        for(auto reference = ref.cbegin(); reference != ref.cend(); ++reference)
            if(*iter == *reference) found = true;

        if(!found) return false;
    }
    return true;
}

}

#endif /* COFFEE_MILL_UTILITY */
