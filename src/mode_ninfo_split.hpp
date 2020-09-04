#ifndef COFFEE_MILL_NINFO_SPLIT
#define COFFEE_MILL_NINFO_SPLIT
#include <deque>
#include <string_view>

namespace mill
{

const char* ninfo_split_usage() noexcept;
int mode_ninfo_split(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_NINFO_SPLIT
