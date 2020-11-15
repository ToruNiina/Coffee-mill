#ifndef COFFEE_MILL_CALC_AABB_HPP
#define COFFEE_MILL_CALC_AABB_HPP
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_calc_aabb_usage() noexcept;
int mode_calc_aabb(std::deque<std::string_view> args);

}//mill
#endif /* COFFEE_MILL_CALC_AABB */
