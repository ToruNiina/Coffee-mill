#ifndef COFFEE_MILL_PDB_MUTATE
#define COFFEE_MILL_PDB_MUTATE
#include <mill/pdb/PDBChain.hpp>
#include <mill/util/string.hpp>
#include <mill/util/amino_acid_code.hpp>
#include <toml/toml.hpp>

namespace mill
{

// argv := {"mutate", "filename.toml"}
template<typename vectorT>
int mill_pdb_mutate(int argument_c, char **argument_v)
{
    if(argument_c < 2)
    {
        std::cerr << "error: mill pdb mutate: too few commands" << std::endl;
        return 1;
    }

    const std::string fname(argument_v[1]);
    if(fname.substr(fname.size()-5, 5) != ".toml")
    {
        std::cerr << "error: mill pdb mutate: toml file is required" << std::endl;
        return 1;
    }
    std::ifstream ifs(fname);
    if(!ifs.good())
    {
        std::cerr << "error: mill pdb mutate: file cannot open "
                  << fname << std::endl;
        return 1;
    }
    const auto data = toml::parse(ifs);

    const std::string template_fname = toml::find<std::string>(data, "pdbfile");
    const std::string output_fname   = toml::find<std::string>(data, "output");
    const auto        sequence_table = toml::find(data, "sequences");

    PDBReader<vectorT> reader;
    auto chains = reader.parse(reader.read(template_fname));

    amino_acid_code aacode;
    for(auto& chain : chains)
    {
        const auto seq = toml::find<std::string>(sequence_table, chain.chain_id());

        if(seq.size() != chain.size())
        {
            std::cerr << "error: mill pdb mutate:"
                      << "length of sequence differs from pdb" << std::endl;
            return 1;
        }

        if('D' == remove_all(' ', chain.front().residue_name())[0])
        {
            for(std::size_t i=0; i<seq.size(); ++i)
            {
                const std::string res_seq = std::string("D") + seq[i];
                for(auto& atom : chain.at(i))
                {
                    atom.residue_name = res_seq;
                }
            }
        }
        else if(remove_all(' ', chain.front().residue_name()).size() == 3)
        {
            for(std::size_t i=0; i<seq.size(); ++i)
            {
                const std::string res_seq = aacode(seq[i]);
                for(auto& atom : chain.at(i))
                {
                    atom.residue_name = res_seq;
                }
            }
        }
    }

    PDBWriter<vectorT> writer;
    writer.write(output_fname, chains);

    return 0;
}



} // mill
#endif /* COFFEE_MILL_PDB_MUTATE */
