#ifndef COFFEE_MILL_TRAJ_EXTRACT_HPP
#define COFFEE_MILL_TRAJ_EXTRACT_HPP
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_traj_extract_usage() noexcept;
int mode_traj_extract(std::deque<std::string_view> args);

}//mill
#endif /* COFFEE_MILL_TRAJ_EXTRACT_HPP */
