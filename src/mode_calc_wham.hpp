#ifndef COFFEE_MILL_CALC_WHAM
#define COFFEE_MILL_CALC_WHAM
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_calc_wham_usage() noexcept;
int mode_calc_wham(std::deque<std::string_view> args);

}//mill
#endif /* COFFEE_MILL_CALC_RMSD */
