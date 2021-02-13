#ifndef COFFEE_MILL_TRAJ_PROJECTION_HPP
#define COFFEE_MILL_TRAJ_PROJECTION_HPP
#include <deque>
#include <string_view>

namespace mill
{
const char* mode_traj_projection_usage() noexcept;
int mode_traj_projection(std::deque<std::string_view>);
} // mill
#endif // COFFEE_MILL_TRAJ_PROJECTION_HPP
