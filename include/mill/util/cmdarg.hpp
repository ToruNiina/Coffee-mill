#ifndef COFFEE_MILL_UTIL_CMD_ARG_HPP
#define COFFEE_MILL_UTIL_CMD_ARG_HPP
#include <algorithm>
#include <deque>
#include <optional>
#include <variant>
#include <sstream>
#include <string_view>

namespace mill
{
namespace detail
{
template<typename T>
struct argumnet_default_return_value{using type = T;};
template<>
struct argumnet_default_return_value<void>{using type = std::monostate;};

template<typename T>
using argumnet_default_return_value_t = typename argumnet_default_return_value<T>::type;
} // detail

// It pops `--arg=value` from `args`. It converts the `value` via istream.
template<typename T, typename U = detail::argumnet_default_return_value_t<T>>
std::optional<U>
pop_argument(std::deque<std::string_view>& args, std::string_view name)
{
    const auto found = std::find_if(args.begin(), args.end(),
        [name](const auto arg) -> bool {
            if(arg.size() <= 3 + name.size())
            {
                return false;
            }
            if(arg.substr(0, 2) == "--" && arg.substr(2, name.size()) == name &&
                arg.at(2 + name.size()) == '=')
            {
                return true;
            }
            return false;
        });

    if(found != args.end())
    {
        std::istringstream iss(std::string(found->substr(3 + name.size())));
        T value;
        iss >> value;

        args.erase(found);

        return value;
    }
    return std::nullopt;
}

// It pops `--arg`. If exists, retval would have value.
template<>
inline std::optional<std::monostate>
pop_argument<void, std::monostate>(std::deque<std::string_view>& args, std::string_view name)
{
    const auto found = std::find_if(args.begin(), args.end(),
        [name](const auto arg) -> bool {
            if(arg.size() != 2 + name.size())
            {
                return false;
            }
            if(arg.substr(0, 2) == "--" && arg.substr(2, name.size()) == name)
            {
                return true;
            }
            return false;
        });

    if(found != args.end())
    {
        args.erase(found);
        return std::monostate{};
    }
    return std::nullopt;
}

} // mil
#endif//COFFEE_MILL_UTIL_CMD_ARG_HPP
