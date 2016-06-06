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

template<std::size_t N_bodies, std::size_t N_coefs>
class NinfoData
{
  public:
    using size_type    = std::size_t;
    using index_type   = size_type;
    using coef_type    = double;
    using units_type   = std::array<index_type, 2>;
    using indices_type = std::array<index_type, N_bodies>;
    using coefs_type   = std::array<coef_type, N_coefs>;

    constexpr static std::size_t num_bodies = N_bodies;
    constexpr static std::size_t num_coefs = N_coefs;

  public:
    NinfoData(){}
    ~NinfoData() = default;

    const std::string& header() const {return header_;}
          std::string& header()       {return header_;}

    size_type  id() const {return id_;}
    size_type& id()       {return id_;}

    const indices_type& units() const {return units_;}
          indices_type& units()       {return units_;}
    const indices_type& global_imps() const {return global_mass_points_;}
          indices_type& global_imps()       {return global_mass_points_;}
    const indices_type& local_imps() const {return local_mass_points_;}
          indices_type& local_imps()       {return local_mass_points_;}

    const std::vector<std::string>& kind() const {return kind_;}
          std::vector<std::string>& kind()       {return kind_;}

  private:

    std::size_t  id_;
    std::string  header_;
    units_type units_;
    indices_type global_mass_points_;
    indices_type local_mass_points_;
    coefs_type   coefs_;
    std::vector<std::string> kind_;
};

using NinfoBond      = NinfoData<2, 4>;
using NinfoAngl      = NinfoData<3, 4>;
using NinfoAicg13    = NinfoData<3, 5>;
using NinfoDihd      = NinfoData<4, 5>;
using NinfoAicg14    = NinfoData<4, 5>;
using NinfoContact   = NinfoData<2, 4>;
using NinfoBasePair  = NinfoData<2, 4>;
using NinfoBaseStuck = NinfoData<2, 4>;

template<std::size_t N_bodies, std::size_t N_coefs> 
std::ostream operator<<(std::ostream& os,
                        const NinfoData<N_bodies, N_coefs>& ninfo)
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

    os << " ";
    for(auto iter = ninfo.kind().cbegin();
            iter != ninfo.kind().cend(); ++iter)
    os << *iter << " ";

    return os;
}

//! read one line using std::getline
template<std::size_t N_bodies, std::size_t N_coefs> 
std::istream operator>>(std::istream& is,
                        NinfoData<N_bodies, N_coefs>& ninfo)
{
    std::string line;
    std::getline(is, line);
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

    return is;
}


}
#endif //COFFEE_MILL_NINFO_BASE
