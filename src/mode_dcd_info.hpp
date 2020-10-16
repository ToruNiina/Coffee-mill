#ifndef COFFEE_MILL_DCD_INFO_HPP
#define COFFEE_MILL_DCD_INFO_HPP
#include <string_view>
#include <deque>

namespace mill
{

const char* mode_dcd_info_usage() noexcept;
int mode_dcd_info(std::deque<std::string_view> args);

} // mill
#endif  //COFFEE_MILL_DCD_INFO
