#ifndef COFFEE_MILL_CALC_CENTER_HPP
#define COFFEE_MILL_CALC_CENTER_HPP
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_calc_center_usage() noexcept;
int mode_calc_center(std::deque<std::string_view> args);

}//mill
#endif /* COFFEE_MILL_CALC_CENTER */
