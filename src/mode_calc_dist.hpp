#ifndef COFFEE_MILL_CALC_DIST_HPP
#define COFFEE_MILL_CALC_DIST_HPP
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_calc_dist_usage() noexcept;
int mode_calc_dist(std::deque<std::string_view>);

}//mill
#endif /* COFFEE_MILL_CALC_DIST */
