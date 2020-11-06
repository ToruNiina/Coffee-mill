#ifndef COFFEE_MILL_UTIL_COLOR_HPP
#define COFFEE_MILL_UTIL_COLOR_HPP
#include <mill/util/isatty.hpp>
#include <ostream>

namespace mill
{
namespace color
{

inline std::ostream& bold(std::ostream& os)
{
    if(isatty(os)) {os << "\x1b[01m";}
    return os;
}
inline std::ostream& red(std::ostream& os)
{
    if(isatty(os)) {os << "\x1b[31m";}
    return os;
}
inline std::ostream& green(std::ostream& os)
{
    if(isatty(os)) {os << "\x1b[32m";}
    return os;
}
inline std::ostream& yellow(std::ostream& os)
{
    if(isatty(os)) {os << "\x1b[33m";}
    return os;
}
inline std::ostream& blue(std::ostream& os)
{
    if(isatty(os)) {os << "\x1b[34m";}
    return os;
}
inline std::ostream& magenta(std::ostream& os)
{
    if(isatty(os)) {os << "\x1b[35m";}
    return os;
}
inline std::ostream& cyan(std::ostream& os)
{
    if(isatty(os)) {os << "\x1b[36m";}
    return os;
}
inline std::ostream& white(std::ostream& os)
{
    if(isatty(os)) {os << "\x1b[37m";}
    return os;
}
inline std::ostream& clear(std::ostream& os)
{
    if(isatty(os)) {os << "\x1b[0m";}
    return os;
}

} // color
} // mill
#endif//COFFEE_MILL_UTIL_COLOR_HPP
