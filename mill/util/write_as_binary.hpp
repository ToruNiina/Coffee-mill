#ifndef COFFEE_MILL_UTIL_WRITE_AS_BINARY_HPP
#define COFFEE_MILL_UTIL_WRITE_AS_BINARY_HPP
#include <type_traits>
#include <utility>
#include <memory>
#include <iosfwd>

namespace mill
{

template<typename T>
void write_as_binary(std::ostream& os, const T& v) noexcept
{
    using Type = std::remove_reference_t<T>;
    os.write(reinterpret_cast<const char*>(std::addressof(v)), sizeof(Type));
    return dst;
}

} // mill
#endif//COFFEE_MILL_UTIL_WRITE_AS_BINARY_HPP
