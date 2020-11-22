#ifndef COFFEE_MILL_UTIL_STRING_HPP
#define COFFEE_MILL_UTIL_STRING_HPP
#include <string>
#include <string_view>
#include <algorithm>

namespace mill
{

template<typename charT, typename Traits, typename Alloc>
std::basic_string<charT, Traits, Alloc>
remove_indent(const std::basic_string<charT, Traits, Alloc>& str)
{
    auto iter = str.cbegin();
    while(iter != str.cend())
    {
        if(const char c = *iter; (c != ' ' && c != '\t'))
        {
            break;
        }
        ++iter;
    }
    return std::basic_string<charT, Traits, Alloc>(iter, str.cend());
}

template<typename charT, typename Traits, typename Alloc>
std::basic_string<charT, Traits, Alloc>
remove_all(const charT c, const std::basic_string<charT, Traits, Alloc>& str)
{
    auto retval = str;
    retval.erase(std::remove(retval.begin(), retval.end(), c), retval.end());
    return retval;
}

constexpr inline bool starts_with(std::string_view src, std::string_view tgt)
{
    if(src.size() < tgt.size())
    {
        return false;
    }
    return src.substr(0, tgt.size()) == tgt;
}

} // mill
#endif /* COFFEE_MILL_UTILITY */
