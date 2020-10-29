#ifndef COFFEE_MILL_MODE_TRAJ_ROTATE_HPP
#define COFFEE_MILL_MODE_TRAJ_ROTATE_HPP
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_traj_rotate_usage() noexcept;
int mode_traj_rotate(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_MODE_TRAJ_ROTATE_HPP
