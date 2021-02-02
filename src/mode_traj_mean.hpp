#ifndef COFFEE_MILL_TRAJ_MEAN
#define COFFEE_MILL_TRAJ_MEAN

#include <string_view>
#include <deque>

namespace mill
{

const char* mode_traj_mean_usage() noexcept;
int mode_traj_mean(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_TRAJ_MEAN // mill
