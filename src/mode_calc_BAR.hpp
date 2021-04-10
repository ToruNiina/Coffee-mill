#ifndef COFFEE_MILL_CALC_BAR_HPP
#define COFFEE_MILL_CALC_BAR_HPP

#include <string_view>
#include <deque>

namespace mill
{

const char* mode_calc_BAR_usage() noexcept;
int mode_calc_BAR(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_CALC_BAR_HPP // mill
