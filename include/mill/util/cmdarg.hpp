#ifndef COFFEE_MILL_UTIL_CMD_ARG_HPP
#define COFFEE_MILL_UTIL_CMD_ARG_HPP
#include <algorithm>
#include <deque>
#include <optional>
#include <sstream>
#include <string_view>

namespace mill
{

// It pops `--arg=value` from `args`. It converts the `value` via istream.
template<typename T>
std::optional<T>
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


} // mil
#endif//COFFEE_MILL_UTIL_CMD_ARG_HPP
