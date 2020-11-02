#ifndef COFFEE_MILL_CALC_OBB_HPP
#define COFFEE_MILL_CALC_OBB_HPP

#include <deque>
#include <string_view>

namespace mill
{

const char* mode_calc_obb_usage() noexcept;
int mode_calc_obb(std::deque<std::string_view> args);

}//mill
#endif//COFFEE_MILL_CALC_OBB_HPP
