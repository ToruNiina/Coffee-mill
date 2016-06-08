#ifndef COFFEE_MILL_NINFO_BASE
#define COFFEE_MILL_NINFO_BASE
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace coffeemill
{

class NinfoBase
{
  public:

    using size_type    = std::size_t;
    using index_type   = size_type;
    using coef_type    = double;

  public:

    NinfoBase(){}
    virtual ~NinfoBase() = default;

    virtual const std::string& header() const = 0;
    virtual       std::string& header()       = 0;

    virtual size_type bodies() const = 0;
    virtual size_type coefs()  const = 0;

    virtual index_type  id() const = 0;
    virtual index_type& id()       = 0;

    virtual index_type  unit_at(index_type i)      const = 0;
    virtual index_type& unit_at(index_type i)            = 0;
    virtual index_type  global_imp_at(index_type i) const = 0;
    virtual index_type& global_imp_at(index_type i)       = 0;
    virtual index_type  local_imp_at(index_type i)  const = 0;
    virtual index_type& local_imp_at(index_type i)        = 0;

    virtual const std::vector<std::string>& kind()  const = 0;
    virtual       std::vector<std::string>& kind()        = 0;
};

template<std::size_t N_bodies, std::size_t N_coefs>
class NinfoElement : public NinfoBase
{
  public:
    using base_type    = NinfoBase;
    using size_type    = base_type::size_type;
    using index_type   = base_type::index_type;
    using coef_type    = base_type::coef_type;
    using units_type   = std::array<index_type, 2>;
    using indices_type = std::array<index_type, N_bodies>;
    using coefs_type   = std::array<coef_type, N_coefs>;

    constexpr static size_type num_bodies = N_bodies;
    constexpr static size_type num_coefs  = N_coefs;

  public:

    NinfoElement(){}
    ~NinfoElement() override = default;

    size_type bodies() const override {return num_bodies;}
    size_type coefs()  const override {return num_coefs;}

    const std::string& header() const override {return header_;}
          std::string& header()       override {return header_;}

    index_type  id() const override {return id_;}
    index_type& id()       override {return id_;}

    const indices_type& units() const {return units_;}
          indices_type& units()       {return units_;}
    index_type  unit_at(index_type i) const override {return units_.at(i);}
    index_type& unit_at(index_type i)       override {return units_.at(i);}

    const indices_type& global_imps() const {return g_imp_;}
          indices_type& global_imps()       {return g_imp_;}
    index_type  global_imp_at(index_type i) const override {return g_imp_.at(i);}
    index_type& global_imp_at(index_type i)       override {return g_imp_.at(i);}

    const indices_type& local_imps() const {return l_imp_;}
          indices_type& local_imps()       {return l_imp_;}
    index_type  local_imp_at(index_type i) const override {return l_imp_.at(i);}
    index_type& local_imp_at(index_type i)       override {return l_imp_.at(i);}

    const std::vector<std::string>& kind() const override {return kind_;}
          std::vector<std::string>& kind()       override {return kind_;}

  private:

    index_type   id_;
    std::string  header_;
    units_type   units_;
    indices_type g_imp_;
    indices_type l_imp_;
    coefs_type   coefs_;
    std::vector<std::string> kind_;
};

enum class NinfoKind
{
    Bond,
    Angl,
    Aicg13,
    Dihd,
    Aicg14,
    Aicg14p,
    Contact,
    BasePair,
    BaseStack,
    Unknown,
};

using NinfoBond      = NinfoElement<2, 4>;
using NinfoAngl      = NinfoElement<3, 4>;
using NinfoAicg13    = NinfoElement<3, 5>;
using NinfoDihd      = NinfoElement<4, 5>;
using NinfoAicg14    = NinfoElement<4, 5>;
using NinfoAicg14p   = NinfoElement<4, 5>;
using NinfoContact   = NinfoElement<2, 4>;
using NinfoBasePair  = NinfoElement<2, 4>;
using NinfoBaseStack = NinfoElement<2, 4>;

template<std::size_t N_bodies, std::size_t N_coefs> 
std::ostream operator<<(std::ostream& os,
                        const NinfoElement<N_bodies, N_coefs>& ninfo)
{
    os << ninfo.header();
    os << std::setw(7) << std::right << ninfo.id();

    for(auto iter = ninfo.units().cbegin();
            iter != ninfo.units().cend(); ++iter)
        os << std::setw(7) << std::right << *iter;

    for(auto iter = ninfo.global_imps().cbegin();
            iter != ninfo.global_imps().cend(); ++iter)
        os << std::setw(7) << std::right << *iter;

    for(auto iter = ninfo.local_imps().cbegin();
            iter != ninfo.local_imps().cend(); ++iter)
        os << std::setw(7) << std::right << *iter;

    for(auto iter = ninfo.coefs().cbegin();
            iter != ninfo.coefs().cend(); ++iter)
        os << std::setw(13) << std::right << std::fixed
           << std::setprecision(4) << *iter;

    for(auto iter = ninfo.kind().cbegin();
            iter != ninfo.kind().cend(); ++iter)
        os << " " << *iter;

    return os;
}
    
template<std::size_t N_bodies, std::size_t N_coefs> 
void operator>>(const std::string& line,
                NinfoElement<N_bodies, N_coefs>& ninfo)
{
    std::istringstream iss(line);

    iss >> ninfo.header();
    iss >> ninfo.id();
    for(auto iter = ninfo.units().begin();
            iter != ninfo.units().end(); ++iter)
        iss >> *iter;

    for(auto iter = ninfo.global_imps().begin();
            iter != ninfo.global_imps().end(); ++iter)
        iss >> *iter;

    for(auto iter = ninfo.local_imps().begin();
            iter != ninfo.local_imps().end(); ++iter)
        iss >> *iter;

    for(auto iter = ninfo.coefs().begin();
            iter != ninfo.coefs().end(); ++iter)
        iss >> *iter;

    while(!iss.eof())
    {
        std::string kind;
        iss >> kind;
        ninfo.kind().push_back(kind);
    }

    return;
}

//! read one line using std::getline
template<std::size_t N_bodies, std::size_t N_coefs> 
std::istream operator>>(std::istream& is,
                        NinfoElement<N_bodies, N_coefs>& ninfo)
{
    std::string line;
    std::getline(is, line);
    line >> ninfo;

    return is;
}


}
#endif //COFFEE_MILL_NINFO_BASE
