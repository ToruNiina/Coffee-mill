#ifndef COFFEE_MILL_UTIL_READ_BINARY_AS_HPP
#define COFFEE_MILL_UTIL_READ_BINARY_AS_HPP
#include <type_traits>
#include <utility>
#include <memory>
#include <cstring>

namespace mill
{

template<typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T>
read_binary_as(const char* src) noexcept
{
    T dst;
    std::memcpy(reinterpret_cast<char*>(std::addressof(dst)), src, sizeof(T));
    return dst;
}
template<typename T>
std::enable_if_t<std::is_arithmetic<T>::value, T>
read_binary_as(std::istream& is) noexcept
{
    T dst;
    is.read(reinterpret_cast<char*>(std::addressof(dst)), sizeof(T));
    return dst;
}

} // mill
#endif// COFFEE_MILL_UTIL_READ_BINARY_AS_HPP
