#ifndef COFFEE_MILL_UTILITY
#define COFFEE_MILL_UTILITY
#include <string>
#include <algorithm>

namespace mill
{

template<typename charT>
inline bool is_whitespace(charT c)
{
    return (c == ' ') || (c == '\t');
}

template<typename charT>
std::basic_string<charT>
remove_indent(std::basic_string<charT> const& str)
{
    auto iter = str.cbegin();
    while(is_whitespace(*iter) && (iter != str.cend())){++iter;}
    return std::basic_string<charT>(iter, str.cend());
}

template<typename charT>
std::basic_string<charT>
remove_all(const charT c, const std::basic_string<charT>& str)
{
    std::basic_string<charT> retval;
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        if(*iter != c) retval += *iter;
    return retval;
}

} // mill

#endif /* COFFEE_MILL_UTILITY */
