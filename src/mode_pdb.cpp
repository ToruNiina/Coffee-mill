#include "mode_pdb.hpp"

#include <mill/pdb/PDBReader.hpp>
#include <mill/pdb/PDBWriter.hpp>
#include <src/mode_pdb_help.hpp>
#include <src/mode_pdb_seq.hpp>

namespace mill
{

// argv := arrayof{ "pdb", "command-name", {rests...} }
int mode_pdb(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("mill pdb mode: too few arguments");
        mode_pdb_help({});
        return 1;
    }

    const auto command = args.front();
    args.pop_front();
    if(command == "seq")
    {
        return mode_pdb_seq(std::move(args));
    }
    else if(command == "help")
    {
        return mode_pdb_help(std::move(args));
    }
    else
    {
        log::error("mill pdb mode: unknown command: ", command);
        log::error(mode_pdb_help_usage());
        return 1;
    }
}

} // mill
