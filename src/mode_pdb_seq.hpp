#ifndef COFFEE_MILL_PDB_SEQ
#define COFFEE_MILL_PDB_SEQ
#include <mill/data/PDBChain.hpp>
#include <mill/util/string.hpp>
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

struct amino_acid_code
{
    char operator()(std::string const& s) const {return codes.at(s);}
    std::string const& operator()(char s) const
    {
        return std::find_if(codes.cbegin(), codes.cend(),
            [=](const std::pair<std::string, char>& p){return p.second==s;}
            )->first;
    }

    char               at(const std::string& s) const {return codes.at(s);}
    std::string const& at(const char s) const
    {
        const auto found = std::find_if(codes.cbegin(), codes.cend(),
            [=](const std::pair<std::string, char>& p){return p.second==s;}
            );
        if(found != codes.cend())
        {
            return codes.at(s);
        }
        else
        {
            throw std::out_of_range("invalid amino acid code");
        }
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
            return amino.at(res);
        }
        else
            throw std::invalid_argument("unknown sequence: " + res);
    }
    amino_acid_code amino;
};

// argv := {"seq", "file.pdb"}
template<typename vectorT>
int mode_pdb_seq(int argument_c, char **argument_v)
{
    if(argument_c < 2)
    {
        std::cerr << "error: mill pdb seq: too few arguments\n";
        std::cerr << pdb_seq_usage() << std::endl;
        return 1;
    }
    const std::string pdbname(argument_v[1]);

    if(pdbname == "help")
    {
        std::cerr << pdb_seq_usage() << std::endl;
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
