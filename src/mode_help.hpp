#ifndef COFFEE_MILL_HELP_MODE
#define COFFEE_MILL_HELP_MODE
#include <deque>
#include <string_view>

namespace mill
{

const char* main_usage() noexcept;

int mode_help(std::deque<std::string_view>);

} // mill
#endif// COFFEE_MILL_HELP_MODE
