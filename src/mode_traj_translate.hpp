#ifndef COFFEE_MILL_TRAJ_TRANSLATE_HPP
#define COFFEE_MILL_TRAJ_TRANSLATE_HPP
#include <deque>
#include <string_view>

namespace mill
{
const char* mode_traj_translate_usage() noexcept;
int mode_traj_translate(std::deque<std::string_view>);
} // mill
#endif // COFFEE_MILL_TRAJ_TRANSLATE_HPP
