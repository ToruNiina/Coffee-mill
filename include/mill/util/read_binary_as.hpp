#ifndef COFFEE_MILL_UTIL_READ_BINARY_AS_HPP
#define COFFEE_MILL_UTIL_READ_BINARY_AS_HPP
#include <type_traits>
#include <utility>
#include <memory>
#include <istream>

namespace mill
{

template<typename T>
T read_binary_as(std::istream& is) noexcept
{
    static_assert(std::is_arithmetic_v<T>);
    T dst;
    is.read(reinterpret_cast<char*>(std::addressof(dst)), sizeof(T));
    return dst;
}

} // mill
#endif// COFFEE_MILL_UTIL_READ_BINARY_AS_HPP
