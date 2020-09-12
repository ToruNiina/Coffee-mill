#ifndef COFFEE_MILL_UTIL_FILE_EXTENSION_HPP
#define COFFEE_MILL_UTIL_FILE_EXTENSION_HPP
#include <mill/util/logger.hpp>
#include <string_view>

namespace mill
{

// the resulting string_view contains the period.
inline std::string_view extension_of(const std::string_view filename)
{
    const auto dot = filename.rfind('.');
    if(dot == std::string_view::npos)
    {
        using namespace std::literals::string_view_literals;
        log::error("no file extension in ", filename);
        return ""sv;
    }
    return filename.substr(dot, filename.size() - dot);
}

// the resulting string_view does NOT contains the period.
inline std::string_view base_name_of(const std::string_view filename)
{
    const auto dot = filename.rfind('.');
    if(dot == std::string_view::npos)
    {
        using namespace std::literals::string_view_literals;
        log::error("no file extension in ", filename);
        return ""sv;
    }
    return filename.substr(0, dot);
}

} // mill
#endif// COFFEE_MILL_UTIL_FILE_EXTENSION_HPP
