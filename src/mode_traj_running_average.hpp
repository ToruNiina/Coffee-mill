#ifndef COFFEE_MILL_TRAJ_RUNNING_AVERAGE
#define COFFEE_MILL_TRAJ_RUNNING_AVERAGE

#include <string_view>
#include <deque>

namespace mill
{

const char* mode_traj_running_average_usage() noexcept;
int mode_traj_running_average(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_TRAJ_IMPOSE
