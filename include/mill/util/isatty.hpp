#ifndef COFFEE_MILL_UTILITY_IO_HPP
#define COFFEE_MILL_UTILITY_IO_HPP

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
#define COFFEE_MILL_ON_POSIX_ENVIRONMENT 1
#endif

#include <cstdio>
#include <iostream>
#include <memory>
#ifdef COFFEE_MILL_ON_POSIX_ENVIRONMENT
#include <unistd.h> // ::isatty
#endif

namespace mill
{

template<typename charT, typename traits>
bool isatty(const std::basic_ostream<charT, traits>& os) noexcept
{
#if COFFEE_MILL_ON_POSIX_ENVIRONMENT
    if(std::addressof(os) == std::addressof(std::cout))
    {
        return ::isatty(::fileno(stdout)) != 0;
    }
    else if(std::addressof(os) == std::addressof(std::cerr))
    {
        return ::isatty(::fileno(stderr)) != 0;
    }
    return false;
#else
    // on windows or other envs, do nothing.
    return true;
#endif
}

} // mill
#endif //MILL_UTILITY_IO_HPP
