#ifndef COFFEE_MILL_PDB_SEQ
#define COFFEE_MILL_PDB_SEQ
#include <mill/data/PDBChain.hpp>
#include <mill/util/string.hpp>
#include <string>
#include <map>

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

struct amino_acid_code
{
    char operator()(std::string const& s) const {return codes.at(s);}
    std::string const& operator()(char s) const
    {
        return std::find_if(codes.cbegin(), codes.cend(),
            [=](const std::pair<std::string, char>& p){return p.second==s;}
            )->first;
    }

    std::map<std::string, char> codes = {
        {"ALA", 'A'},
        {"ASX", 'B'},
        {"CYS", 'C'},
        {"ASP", 'D'},
        {"GLU", 'E'},
        {"PHE", 'F'},
        {"GLY", 'G'},
        {"HIS", 'H'},
        {"ILE", 'I'},
        {"LYS", 'K'},
        {"LEU", 'L'},
        {"MET", 'M'},
        {"ASN", 'N'},
        {"PRO", 'P'},
        {"GLN", 'Q'},
        {"ARG", 'R'},
        {"SER", 'S'},
        {"THR", 'T'},
        {"SEC", 'U'},
        {"VAL", 'V'},
        {"TRP", 'W'},
        {"XAA", 'X'},
        {"TYR", 'Y'},
        {"GLX", 'Z'}
    };
};

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
