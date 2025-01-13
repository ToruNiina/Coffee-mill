#ifndef COFFEE_MILL_CALC_NEIGHBOR_HPP
#define COFFEE_MILL_CALC_NEIGHBOR_HPP

#include <string_view>
#include <deque>

namespace mill
{

const char* mode_calc_neighbor_usage() noexcept;
int mode_calc_neighbor(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_CALC_NEIGHBOR_HPP // mill
