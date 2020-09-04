#include "mode_pdb_seq.hpp"

#include <mill/util/string.hpp>
#include <mill/util/logger.hpp>
#include <mill/math/Vector.hpp>
#include <mill/common/AminoAcidCode.hpp>
#include <mill/pdb/PDBReader.hpp>
#include <algorithm>
#include <map>

namespace mill
{

inline const char* pdb_seq_usage() noexcept
{
    return "usage: mill pdb seq [parameters...]\n"
           "    $ mill pdb seq model.pdb\n"
           "      print sequences for each chain\n";
}

// argv := {"seq", "file.pdb"}
int mode_pdb_seq(std::deque<std::string_view> args)
{
    if(args.size() != 2)
    {
        log::error("error: mill pdb seq: too few arguments");
        log::error(pdb_seq_usage());
        return 1;
    }

    const auto pdbname = args.at(1);

    if(pdbname == "help")
    {
        log::info(pdb_seq_usage());
        return 0;
    }

    PDBReader<mill::Vector<double, 3>> reader;
    for(auto&& chain : reader.parse(reader.read(std::string(pdbname))))
    {
        std::string sequence(chain.size(), ' ');
        std::transform(chain.cbegin(), chain.cend(), sequence.begin(),
            [](const auto& residue) -> char {
                const auto resname = remove_all(' ', residue.residue_name());
                if(resname.size() == 1)
                {
                    return resname.front();
                }
                else if(resname.size() == 2 && resname.at(0) == 'D')
                {
                    // assuming DA, DT, DC, DG
                    return resname.at(1);
                }
                else if(resname.size() == 3 && amino_acid_3to1.count(resname) != 0)
                {
                    return amino_acid_3to1.at(resname);
                }
                else
                {
                    throw std::invalid_argument("unknown sequence: " + resname);
                }
            });
        std::cout << "chain " << chain.chain_id() << ": " << sequence << std::endl;
    }
    return 0;
}

} // mill
