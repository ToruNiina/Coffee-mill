#ifndef COFFEE_MILL_PDB_SEQ
#define COFFEE_MILL_PDB_SEQ
#include <mill/pdb/PDBChain.hpp>
#include <mill/util/string.hpp>
#include <mill/common/AminoAcidCode.hpp>
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

template<typename Iterator>
struct residue_name_iterator
{
    residue_name_iterator(Iterator iter) : iter_(iter){}

    std::string operator*() const
    {
        return remove_all(' ', iter_->residue_name());
    }

    residue_name_iterator& operator++()
    {
        ++iter_;
        return *this;
    }
    residue_name_iterator& operator++(int)
    {
        const auto tmp = *this;
        ++(*this);
        return tmp;
    }

    Iterator iter_;
};

template<typename Iterator>
inline residue_name_iterator<Iterator>
wrap_residue_name(Iterator iter)
{
    return residue_name_iterator<Iterator>(iter);
}

template<typename Iterator>
inline bool operator==(residue_name_iterator<Iterator> const& lhs,
                       residue_name_iterator<Iterator> const& rhs)
{
    return lhs.iter_ == rhs.iter_;
}

template<typename Iterator>
inline bool operator<(residue_name_iterator<Iterator> const& lhs,
                      residue_name_iterator<Iterator> const& rhs)
{
    return lhs.iter_ < rhs.iter_;
}

template<typename Iterator>
inline bool operator!=(residue_name_iterator<Iterator> const& lhs,
                       residue_name_iterator<Iterator> const& rhs)
{
    return !(lhs == rhs);
}

template<typename Iterator>
inline bool operator<=(residue_name_iterator<Iterator> const& lhs,
                       residue_name_iterator<Iterator> const& rhs)
{
    return (lhs < rhs) || (lhs == rhs);
}

template<typename Iterator>
inline bool operator>(residue_name_iterator<Iterator> const& lhs,
                       residue_name_iterator<Iterator> const& rhs)
{
    return !(lhs <= rhs);
}

template<typename Iterator>
inline bool operator>=(residue_name_iterator<Iterator> const& lhs,
                       residue_name_iterator<Iterator> const& rhs)
{
    return !(lhs < rhs);
}

struct generate_sequence
{
    char operator()(const std::string& res) const
    {
        if(res.size() == 1)
        {
            return res.front();
        }
        else if(res.size() == 2 && res.at(0) == 'D')
        {
            return res.at(1);
        }
        else if(res.size() == 3 && amino_acid_3to1.count(res) != 0)
        {
            return amino_acid_3to1.at(res);
        }
        else
        {
            throw std::invalid_argument("unknown sequence: " + res);
        }
    }
};

// argv := {"seq", "file.pdb"}
template<typename vectorT>
int mode_pdb_seq(int argument_c, const char **argument_v)
{
    if(argument_c != 2)
    {
        log::error("error: mill pdb seq: too few arguments");
        log::error(pdb_seq_usage());
        return 1;
    }
    const std::string pdbname(argument_v[1]);

    if(pdbname == "help")
    {
        log::info(pdb_seq_usage());
        return 0;
    }

    PDBReader<vectorT> reader;
    for(auto&& chain : reader.parse(reader.read(pdbname)))
    {
        std::string sequence(chain.size(), ' ');
        std::transform(wrap_residue_name(chain.cbegin()),
                       wrap_residue_name(chain.cend()),
                       sequence.begin(),
                       generate_sequence());
        std::cout << "chain " << chain.chain_id() << ": " << sequence << std::endl;
    }
    return 0;
}

} // mill
#endif /* COFFEE_MILL_PDB_SEQ */
