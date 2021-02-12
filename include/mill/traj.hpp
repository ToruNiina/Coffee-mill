#ifndef COFFEE_MILL_TRAJ_HPP
#define COFFEE_MILL_TRAJ_HPP

#include "dcd/DCDReader.hpp"
#include "dcd/DCDWriter.hpp"

#include "pdb/PDBReader.hpp"
#include "pdb/PDBWriter.hpp"

#include "trr/TRRReader.hpp"
#include "trr/TRRWriter.hpp"

#include "xyz/XYZReader.hpp"
#include "xyz/XYZWriter.hpp"

#include "util/file_extension.hpp"

namespace mill
{

inline bool is_readable_extension(std::string_view ext)
{
    return ext == ".dcd" ||
           ext == ".pdb" ||
           ext == ".trr" ||
           ext == ".xyz";
}

inline DeferedReader reader(std::string_view filename)
{
    const auto ext = extension_of(filename);
    if(ext == ".dcd")
    {
        return DeferedReader(std::make_unique<DCDReader>(std::string(filename)));
    }
    else if(ext == ".pdb")
    {
        return DeferedReader(std::make_unique<PDBReader>(std::string(filename)));
    }
    else if(ext == ".trr")
    {
        return DeferedReader(std::make_unique<TRRReader>(std::string(filename)));
    }
    else if(ext == ".xyz")
    {
        return DeferedReader(std::make_unique<XYZReader>(std::string(filename)));
    }
    else
    {
        log::fatal("from mill::reader(file):\n",
                   " -> unknown file extension \"", ext, "\".\n",
                   "    supported formats are \".dcd\", \"pdb\", \".trr\", and \".xyz\".");
    }
}

inline bool is_writable_extension(std::string_view ext)
{
    return ext == ".dcd" ||
           ext == ".pdb" ||
           ext == ".trr" ||
           ext == ".xyz";
}

inline TrajWriter writer(std::string_view filename)
{
    const auto ext = extension_of(filename);

    if(ext == ".dcd")
    {
        return TrajWriter(std::make_unique<DCDWriter>(std::string(filename)));
    }
    else if(ext == ".pdb")
    {
        return TrajWriter(std::make_unique<PDBWriter>(std::string(filename)));
    }
    else if(ext == ".trr")
    {
        return TrajWriter(std::make_unique<TRRWriter>(std::string(filename)));
    }
    else if(ext == ".xyz")
    {
        return TrajWriter(std::make_unique<XYZWriter>(std::string(filename)));
    }
    else
    {
        log::fatal("from mill::write(file):\n",
                   " -> unknown file extension \"", ext, "\".\n",
                   "    supported formats are \".dcd\", \"pdb\", \".trr\", and \".xyz\".");
    }
}


} // mill
#endif// COFFEE_MILL_TRAJ_HPP
