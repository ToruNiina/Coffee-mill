#ifndef COFFEE_MILL_CALC_HELP_MODE
#define COFFEE_MILL_CALC_HELP_MODE
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_calc_help_usage() noexcept;
int mode_calc_help(std::deque<std::string_view>);

} // mill
#endif// COFFEE_MILL_CALC_HELP_MODE
