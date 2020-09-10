#ifndef COFFEE_MILL_DCD_MODE_HPP
#define COFFEE_MILL_DCD_MODE_HPP
#include <string_view>
#include <deque>

namespace mill
{
// argv := arrayof{ "dcd", "command-name", {rests...} }
int mode_dcd(std::deque<std::string_view> args);

} // mill
#endif /* COFFEE_MILL_PDB_MODE */
