#ifndef COFFEE_MILL_TRAJ_IMPOSE
#define COFFEE_MILL_TRAJ_IMPOSE

#include <string_view>
#include <deque>

namespace mill
{

const char* mode_traj_impose_usage() noexcept;
int mode_traj_impose(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_TRAJ_IMPOSE
