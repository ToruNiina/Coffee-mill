#ifndef COFFEE_MILL_DCD_HELP_MODE
#define COFFEE_MILL_DCD_HELP_MODE
#include <deque>
#include <string_view>

namespace mill
{

const char* mode_dcd_help_usage() noexcept;

//! this function forwards the arguments to different modes.
// argv := { "help" or "dcd", {args...} }
int mode_dcd_help(std::deque<std::string_view> args);

} // mill
#endif// COFFEE_MILL_HELP_MODE
