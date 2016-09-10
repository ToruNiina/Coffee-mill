#ifndef COFFEE_MILL_UTILITY
#define COFFEE_MILL_UTILITY
#include <string>
#include <algorithm>

namespace coffeemill
{

template<typename charT>
inline bool is_whitespace(charT c)
{
    return (c == ' ') || (c == '\t');
}

template<typename charT>
inline std::basic_string<charT>
remove_indent(std::basic_string<charT> const& str)
{
    auto iter = str.cbegin();
    while(is_whitespace(*iter) && (iter != str.cend())){++iter;}
    return std::basic_string<charT>(iter, str.cend());
}

template<typename charT>
inline std::basic_string<charT>
remove_all(const charT c, const std::basic_string<charT>& str)
{
    std::basic_string<charT> retval;
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        if(*iter != c) retval += *iter;
    return retval;
}


// legacy

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
