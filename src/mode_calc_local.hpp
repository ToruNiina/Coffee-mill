#ifndef COFFEE_MILL_CALC_LOCAL_HPP
#define COFFEE_MILL_CALC_LOCAL_HPP
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_calc_local_usage() noexcept;
int mode_calc_local(std::deque<std::string_view>);

}//mill
#endif /* COFFEE_MILL_CALC_LOCAL */
