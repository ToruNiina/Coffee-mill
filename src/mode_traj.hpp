#ifndef COFFEE_MILL_TRAJ_MODE_HPP
#define COFFEE_MILL_TRAJ_MODE_HPP
#include <string_view>
#include <deque>

namespace mill
{
// argv := arrayof{ "traj", "command-name", {rests...} }
int mode_traj(std::deque<std::string_view> args);
} // mill
#endif /* COFFEE_MILL_TRAJ_MODE */
