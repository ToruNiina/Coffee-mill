#ifndef COFFEE_MILL_TRAJ_SPLIT_HPP
#define COFFEE_MILL_TRAJ_SPLIT_HPP
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_traj_split_usage() noexcept;
int mode_traj_split(std::deque<std::string_view> args);

}//mill
#endif /* COFFEE_MILL_TRAJ_SPLIT */
