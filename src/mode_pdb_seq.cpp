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

std::optional<char> residue_one_letter(const Particle& particle)
{
    const auto resname = remove_all(' ', particle.at("res_name").as_string());
    log::debug("new residue = ", resname, ", that is ", particle.at("res_name"));

    if(particle.at("record").as_string() == "HETATM")
    {
        return std::nullopt; // skip hetero atoms
    }

    if(resname.size() == 1)
    {
        return resname.front();
    }
    else if(resname.size() == 2 && resname.at(0) == 'D')
    {
        // assuming DNA(DA, DT, DC, DG)
        return resname.back();
    }
    else if(resname.size() == 3 && amino_acid_3to1.count(resname) != 0)
    {
        return amino_acid_3to1.at(resname);
    }
    else
    {
        log::warn("pdb seq: unknown residue: ", resname);
        return std::nullopt;
    }
}

// argv := {"seq", "file.pdb"}
int mode_pdb_seq(std::deque<std::string_view> args)
{
    if(args.empty())
    {
        log::error("error: mill pdb seq: too few arguments");
        log::error(pdb_seq_usage());
        return 1;
    }

    const auto pdbname = args.front();
    if(pdbname == "help")
    {
        log::info(pdb_seq_usage());
        return 0;
    }

    PDBReader reader(pdbname);
    std::ignore = reader.read_header();
    const auto frame = *reader.read_frame();

    auto chain_id   = frame.particles().front().at("chain_id").as_string();
    auto residue_id = frame.particles().front().at("res_seq").as_integer();
    std::string sequence;
    if(const auto sq = residue_one_letter(frame.particles().front()))
    {
        sequence += *sq;
    }

    log::debug("seq = ", sequence);

    for(const auto particle : frame.particles())
    {
        if(chain_id != particle.at("chain_id").as_string())
        {
            if(not sequence.empty())
            {
                std::cout << "chain " << chain_id << ": " << sequence << std::endl;
            }
            sequence = "";
            chain_id = particle.at("chain_id").as_string();
        }

        if(residue_id != particle.at("res_seq").as_integer())
        {
            if(const auto sq = residue_one_letter(particle))
            {
                sequence += *sq;
            }
            residue_id = particle.at("res_seq").as_integer();
        }
        else
        {
            log::debug("residue_id = ", residue_id, ", particle = ", particle.at("res_seq").as_integer());
        }
    }
    if(not sequence.empty())
    {
        std::cout << "chain " << chain_id << ": " << sequence << std::endl;
    }
    return 0;
}

} // mill
