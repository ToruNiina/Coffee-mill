#ifndef COFFEE_MILL_TRAJ_JOIN_MODE_HPP
#define COFFEE_MILL_TRAJ_JOIN_MODE_HPP
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_traj_join_usage() noexcept;

int mode_traj_join(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_JOIN_MODE_HPP
