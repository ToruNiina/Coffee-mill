#ifndef COFFEE_MILL_UTILITY
#define COFFEE_MILL_UTILITY
#include <string>

namespace coffeemill
{

template<char del>
std::string remove_all(const std::string& str)
{
    std::string retval;
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        if(*iter != del) retval += *iter;
    return retval;
}

template<char del>
std::string remove_front(const std::string& str)
{
    auto iter = str.cbegin();
    while(*iter == del)
        ++iter;
    return std::string(iter, str.cend());
}

template<char del>
std::string remove_back(const std::string& str)
{
    auto iter = str.crbegin();
    while(*iter == del)
        ++iter;
    const std::string rev(iter, str.cend());
    return std::string(rev.crbegin(), rev.crend());
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

inline bool is_contain(const std::string& target, const std::string& ref)
{
    if(target.size() < ref.size()) return false;
    for(std::size_t i=0; i <= (target.size() - ref.size()); ++i)
        if(target.substr(i, ref.size()) == ref) return true;
    return false;
}

}

#endif /* COFFEE_MILL_UTILITY */
