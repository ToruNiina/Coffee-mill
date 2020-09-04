#ifndef COFFEE_MILL_PDB_SEQ
#define COFFEE_MILL_PDB_SEQ
#include <deque>
#include <string_view>

namespace mill
{

const char* pdb_seq_usage() noexcept;
int mode_pdb_seq(std::deque<std::string_view> args);

} // mill
#endif /* COFFEE_MILL_PDB_SEQ */
