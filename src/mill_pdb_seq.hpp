#ifndef COFFEE_MILL_PDB_SEQ
#define COFFEE_MILL_PDB_SEQ
#include <mill/data/PDBChain.hpp>
#include <mill/util/string.hpp>
#include <mill/util/amino_acid_code.hpp>

namespace mill
{

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
        else if(res[0] == 'D' && res.size() == 2)
        {
            return res[1];
        }
        else if(res.size() == 3)
        {
            return amino(res);
        }
        else
            throw std::invalid_argument("unknown sequence: " + res);
    }

    amino_acid_code amino;
};

// argv := {"seq", "file.pdb"}
template<typename vectorT>
int mill_pdb_seq(int argument_c, char **argument_v)
{
    if(argument_c < 2)
    {
        std::cerr << "error: mill pdb seq: too few commands" << std::endl;
        return 1;
    }

    const std::string pdbname(argument_v[1]);
    PDBReader<vectorT> reader;
    const auto chains = reader.parse(reader.read(pdbname));

    for(auto&& chain : chains)
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
