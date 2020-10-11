#ifndef COFFEE_MILL_TRAJ_CONVERT_MODE
#define COFFEE_MILL_TRAJ_CONVERT_MODE
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_traj_convert_usage() noexcept;

int mode_traj_convert(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_CONVERT_MODE
