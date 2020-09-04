#ifndef COFFEE_MILL_NINFO_HELP_MODE
#define COFFEE_MILL_NINFO_HELP_MODE
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_ninfo_help_usage() noexcept;
int mode_ninfo_help(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_NINFO_HELP_MODE
