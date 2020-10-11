#ifndef COFFEE_MILL_TRAJ_HELP_MODE
#define COFFEE_MILL_TRAJ_HELP_MODE
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_traj_help_usage() noexcept;

//! this function forwards the arguments to different modes.
// argv := { "help" or "traj", {args...} }
int mode_traj_help(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_HELP_MODE
