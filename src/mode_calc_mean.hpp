#ifndef COFFEE_MILL_CALC_MEAN
#define COFFEE_MILL_CALC_MEAN

#include <string_view>
#include <deque>

namespace mill
{

const char* mode_calc_mean_usage() noexcept;
int mode_calc_mean(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_CALC_MEAN // mill
