#include "mode_pdb.hpp"

#include <mill/pdb/PDBReader.hpp>
#include <mill/pdb/PDBWriter.hpp>
#include <src/mode_pdb_help.hpp>
#include <src/mode_pdb_seq.hpp>

namespace mill
{

// argv := arrayof{ "pdb", "command-name", {rests...} }
int mode_pdb(int argument_c, const char **argument_v)
{
    if(argument_c < 2)
    {
        log::error("mill pdb mode: too few arguments");
        mode_pdb_help({});
        return 1;
    }

    const std::string command(argument_v[1]);
    if(command == "seq")
    {
        return mode_pdb_seq(--argument_c, ++argument_v);
    }
    else if(command == "help")
    {
        log::info(mode_pdb_help_usage());
        return 0;
    }
    else
    {
        log::error("mill pdb mode: unknown command: ", command);
        log::error(mode_pdb_help_usage());
        return 1;
    }
}

} // mill
