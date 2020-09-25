/*!
  @file NinfoBase.hpp
  @brief definition of a NinfoBase class and its subclass.

  definition of class NinfoBase that is abstract class and its template subclass.
  NinfoBase has pure virtual member functions that provides an access to member
  variables. Subclass NinfoElement is a template class and contain a native
  information.

  @author Toru Niina (niina.toru.68@gmail.com)
  @date 2016-06-09 17:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_NINFO_BASE_HPP
#define COFFEE_MILL_NINFO_BASE_HPP
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace mill
{

class NinfoBase
{
  public:

    using size_type  = std::size_t;
    using index_type = size_type;
    using real_type  = double;

  public:

    NinfoBase() = default;
    virtual ~NinfoBase() = default;

    //! header(ex, "bond", "angl", "aicg13")
    virtual const std::string& header() const noexcept = 0;
    virtual       std::string& header()       noexcept = 0;

    virtual std::size_t num_bodies() const noexcept = 0;
    virtual std::size_t num_coefs()  const noexcept = 0;

    //! ID of the native information.
    virtual std::size_t  id() const noexcept = 0;
    virtual std::size_t& id()       noexcept = 0;

    //! unit(chain) index.
    /*!
     *  index of units. represents iunit1 and iunit2.
     *  Note that coffee-mill uses 0-based index. iunit1 = unit_at(0).
     *  in the case of contact information, the values are different.
     *  @param i 0 or 1. size of units is always 2.
     */
    virtual std::size_t  unit_at(index_type i)       const = 0;
    virtual std::size_t& unit_at(index_type i)             = 0;

    //! imps.
    /*!
     *  index of mass points. corresponding to imp1, imp2, ...
     *  Note that coffee-mill uses 0-based index. imp1 = imp_at(0).
     *  @param i in the range [0, n_bodies)
     */
    virtual std::size_t  global_imp_at(index_type i) const = 0;
    virtual std::size_t& global_imp_at(index_type i)       = 0;

    //! impuns.
    /*!
     *  index of mass points in the unit. corresponding to impun1, impun2, ...
     *  Note that coffee-mill uses 0-based index. impun1 = local_imp_at(0).
     *  @param i in the range [0, n_bodies)
     */
    virtual std::size_t  local_imp_at(index_type i)  const = 0;
    virtual std::size_t& local_imp_at(index_type i)        = 0;

    //! coefficients.
    /*!
     *  including all type of coefficients, for example,
     *  native_value, coef_go, correct_mgo, etc...
     */
    virtual real_type  coef_at(index_type i)        const = 0;
    virtual real_type& coef_at(index_type i)              = 0;

    //! ninfo kind. like "ppp".
    virtual std::vector<std::string> const& kind()  const noexcept = 0;
    virtual std::vector<std::string>      & kind()        noexcept = 0;
};

//! NinfoElement class.
/*!
 *  NinfoElement is subclass of NinfoBase.
 *  this contains the information that is written in one line in ninfo file.
 *  @tparam N_bodies the number of bodies corresponding to the interaction.
 *  @tparam N_coefs  the number of coefficients corresponding to the interaction.
 */
template<std::size_t N_bodies, std::size_t N_coefs>
class NinfoElement : public NinfoBase
{
  public:
    using base_type    = NinfoBase;
    using size_type    = typename base_type::size_type;
    using index_type   = typename base_type::index_type;
    using real_type    = typename base_type::real_type;
    using units_type   = std::array<index_type, 2>;
    using indices_type = std::array<index_type, N_bodies>;
    using coefs_type   = std::array<real_type, N_coefs>;

    constexpr static size_type number_of_bodies = N_bodies;
    constexpr static size_type number_of_coefs  = N_coefs;

  public:

    NinfoElement() = default;
    ~NinfoElement() override = default;

    //! return the number of bodies correspoinding to the interaction.
    size_type num_bodies() const noexcept override {return number_of_bodies;}
    size_type num_coefs()  const noexcept override {return number_of_coefs;}

    //! header. like "bond"
    std::string const& header() const noexcept override {return header_;}
    std::string&       header()       noexcept override {return header_;}

    //! native interaction id.
    index_type  id() const noexcept override {return id_;}
    index_type& id()       noexcept override {return id_;}

    //! array of iunit.
    units_type const& units() const noexcept {return units_;}
    units_type&       units()       noexcept {return units_;}

    //! iunit.
    index_type  unit_at(index_type i) const override {return units_.at(i);}
    index_type& unit_at(index_type i)       override {return units_.at(i);}

    //! array of imp.
    indices_type const& global_imps() const noexcept {return g_imp_;}
    indices_type&       global_imps()       noexcept {return g_imp_;}

    //! imp.
    index_type  global_imp_at(index_type i) const override {return g_imp_.at(i);}
    index_type& global_imp_at(index_type i)       override {return g_imp_.at(i);}

    //! array of impun.
    indices_type const& local_imps() const noexcept {return l_imp_;}
    indices_type&       local_imps()       noexcept {return l_imp_;}

    //! impun.
    index_type  local_imp_at(index_type i) const override {return l_imp_.at(i);}
    index_type& local_imp_at(index_type i)       override {return l_imp_.at(i);}

    //! array of coef
    coefs_type const& coefs() const noexcept {return coefs_;}
    coefs_type&       coefs()       noexcept {return coefs_;}

    //! coef
    real_type  coef_at(index_type i) const override {return coefs_.at(i);}
    real_type& coef_at(index_type i)       override {return coefs_.at(i);}

    //! vector of kind.
    std::vector<std::string> const& kind() const noexcept override {return kind_;}
    std::vector<std::string>&       kind()       noexcept override {return kind_;}

  private:

    index_type   id_;     //!< ninfo id.
    std::string  header_; //!< header. like "bond"
    units_type   units_;  //!< array of iunits
    indices_type g_imp_;  //!< array of imp
    indices_type l_imp_;  //!< array of impun
    coefs_type   coefs_;  //!< array of coefficient
    std::vector<std::string> kind_;  //!< footer. like "pp"
};

//! NinfoKind. Bond, Angl, ...
enum class NinfoKind : int
{
    Bond      = 1,
    Angl      = 2,
    Aicg13    = 3,
    Dihd      = 4,
    Aicg14    = 5,
    Aicgdih   = 6,
    Contact   = 7,
    BasePair  = 8,
    BaseStack = 9,
    Unknown   = 0,
};

template<typename charT, typename traitT>
std::basic_ostream<charT, traitT>&
operator<<(std::basic_ostream<charT, traitT>& os, const NinfoKind kd)
{
    switch(kd)
    {
        case NinfoKind::Bond:      os << "bond";      break;
        case NinfoKind::Angl:      os << "angl";      break;
        case NinfoKind::Aicg13:    os << "aicg13";    break;
        case NinfoKind::Dihd:      os << "dihd";      break;
        case NinfoKind::Aicg14:    os << "aicg14";    break;
        case NinfoKind::Aicgdih:   os << "aicgdih";   break;
        case NinfoKind::Contact:   os << "contact";   break;
        case NinfoKind::BasePair:  os << "basepair";  break;
        case NinfoKind::BaseStack: os << "basestack"; break;
        case NinfoKind::Unknown:   os << "unknown";   break;
    }
    return os;
}


using NinfoBond      = NinfoElement<2, 4>;
using NinfoAngl      = NinfoElement<3, 4>;
using NinfoAicg13    = NinfoElement<3, 5>;
using NinfoDihd      = NinfoElement<4, 5>;
using NinfoAicg14    = NinfoElement<4, 5>;
using NinfoAicgdih   = NinfoElement<4, 5>;
using NinfoContact   = NinfoElement<2, 4>;
using NinfoBasePair  = NinfoElement<2, 4>;
using NinfoBaseStack = NinfoElement<2, 4>;

//! output NinfoElement.
/*!
 *  output ninfo line as ninfo format.
 *  @tparam N_bodies template argument of NinfoElement.
 *  @tparam N_coefs  template argument of NinfoElement.
 *  @sa     NinfoElement
 */
template<std::size_t N_bodies, std::size_t N_coefs>
std::ostream&
operator<<(std::ostream& os, const NinfoElement<N_bodies, N_coefs>& ninfo)
{
    os << ninfo.header();
    os << std::setw(7) << std::right << ninfo.id();

    for(auto item : ninfo.units())
    {
        os << std::setw(7) << std::right << item;
    }
    for(auto item : ninfo.global_imps())
    {
        os << std::setw(7) << std::right << item;
    }
    for(auto item : ninfo.local_imps())
    {
        os << std::setw(7) << std::right << item;
    }
    for(auto item : ninfo.coefs())
    {
        os << std::setw(13) << std::right << std::fixed
           << std::setprecision(4) << item;
    }
    for(auto item : ninfo.kind())
    {
        os << " " << item;
    }

    return os;
}

//! read NinfoElement from string.
/*!
 *  read ninfo line as ninfo format.
 *  @tparam N_bodies template argument of NinfoElement.
 *  @tparam N_coefs  template argument of NinfoElement.
 *  @sa     NinfoElement
 */
template<std::size_t N_bodies, std::size_t N_coefs>
std::istream&
operator>>(std::istream& is, NinfoElement<N_bodies, N_coefs>& ninfo)
{
    std::string line;
    std::getline(is, line);
    std::istringstream iss(line);

    iss >> ninfo.header();
    iss >> ninfo.id();
    for(auto& item : ninfo.units())
    {
        iss >> item;
    }
    for(auto& item : ninfo.global_imps())
    {
        iss >> item;
    }
    for(auto& item : ninfo.local_imps())
    {
        iss >> item;
    }
    for(auto& item : ninfo.coefs())
    {
        iss >> item;
    }
    while(!iss.eof())
    {
        std::string kind;
        iss >> kind;
        ninfo.kind().push_back(kind);
    }
    return is;
}

}//mill
#endif //COFFEE_MILL_NINFO_BASE
