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

#ifndef COFFEE_MILL_NINFO_BASE
#define COFFEE_MILL_NINFO_BASE
#include <string>
#include <array>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace mill
{

template<typename realT>
class NinfoBase
{
  public:

    using size_type  = std::size_t;
    using index_type = size_type;
    using real_type  = realT;

  public:

    NinfoBase() = default;
    virtual ~NinfoBase() = default;

    //! header(ex, "bond", "angl", "aicg13")
    virtual const std::string& header() const = 0;
    virtual       std::string& header()       = 0;

    virtual std::size_t num_bodies() const = 0;
    virtual std::size_t num_coefs()  const = 0;

    //! ID of the native information.
    virtual std::size_t  id() const = 0;
    virtual std::size_t& id()       = 0;

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
    virtual std::vector<std::string> const& kind()  const = 0;
    virtual std::vector<std::string>      & kind()        = 0;
};

//! NinfoElement class.
/*!
 *  NinfoElement is subclass of NinfoBase.
 *  this contains the information that is written in one line in ninfo file.
 *  @tparam N_bodies the number of bodies corresponding to the interaction.
 *  @tparam N_coefs  the number of coefficients corresponding to the interaction.
 */
template<std::size_t N_bodies, std::size_t N_coefs, typename realT = double>
class NinfoElement : public NinfoBase<realT>
{
  public:
    using base_type    = NinfoBase<realT>;
    using size_type    = typename base_type::size_type;
    using index_type   = typename base_type::index_type;
    using real_type    = typename base_type::real_type;
    using units_type   = std::array<index_type, 2>;
    using indices_type = std::array<index_type, N_bodies>;
    using coefs_type   = std::array<real_type, N_coefs>;

    constexpr static size_type num_bodies = N_bodies;
    constexpr static size_type num_coefs  = N_coefs;

  public:

    NinfoElement() = default;
    ~NinfoElement() override = default;

    //! return the number of bodies correspoinding to the interaction.
    size_type n_bodies() const override {return num_bodies;}
    size_type n_coefs()  const override {return num_coefs;}

    //! header. like "bond"
    std::string const& header() const override {return header_;}
    std::string&       header()       override {return header_;}

    //! native interaction id.
    index_type  id() const override {return id_;}
    index_type& id()       override {return id_;}

    //! array of iunit.
    units_type const& units() const {return units_;}
    units_type&       units()       {return units_;}

    //! iunit.
    index_type  unit_at(index_type i) const override {return units_.at(i);}
    index_type& unit_at(index_type i)       override {return units_.at(i);}

    //! array of imp.
    indices_type const& global_imps() const {return g_imp_;}
    indices_type&       global_imps()       {return g_imp_;}

    //! imp.
    index_type  global_imp_at(index_type i) const override {return g_imp_.at(i);}
    index_type& global_imp_at(index_type i)       override {return g_imp_.at(i);}

    //! array of impun.
    indices_type const& local_imps() const {return l_imp_;}
    indices_type&       local_imps()       {return l_imp_;}

    //! impun.
    index_type  local_imp_at(index_type i) const override {return l_imp_.at(i);}
    index_type& local_imp_at(index_type i)       override {return l_imp_.at(i);}

    //! array of coef
    coefs_type const& coefs() const {return coefs_;}
    coefs_type&       coefs()       {return coefs_;}

    //! coef
    real_type  coef_at(index_type i) const override {return coefs_.at(i);}
    real_type& coef_at(index_type i)       override {return coefs_.at(i);}

    //! vector of kind.
    std::vector<std::string> const& kind() const override {return kind_;}
    std::vector<std::string>&       kind()       override {return kind_;}

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
enum class NinfoKind
{
    Bond,
    Angl,
    Aicg13,
    Dihd,
    Aicg14,
    Aicgdih,
    Contact,
    BasePair,
    BaseStack,
    Unknown,
};

using NinfoBond      = NinfoElement<2, 4, double>;
using NinfoAngl      = NinfoElement<3, 4, double>;
using NinfoAicg13    = NinfoElement<3, 5, double>;
using NinfoDihd      = NinfoElement<4, 5, double>;
using NinfoAicg14    = NinfoElement<4, 5, double>;
using NinfoAicgdih   = NinfoElement<4, 5, double>;
using NinfoContact   = NinfoElement<2, 4, double>;
using NinfoBasePair  = NinfoElement<2, 4, double>;
using NinfoBaseStack = NinfoElement<2, 4, double>;

//! output NinfoElement.
/*!
 *  output ninfo line as ninfo format.
 *  @tparam N_bodies template argument of NinfoElement.
 *  @tparam N_coefs  template argument of NinfoElement.
 *  @sa     NinfoElement
 */
template<std::size_t N_bodies, std::size_t N_coefs, typename real_type> 
std::basic_ostream<char>&
operator<<(std::basic_ostream<char>& os,
           NinfoElement<N_bodies, N_coefs, real_type> const& ninfo)
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

//! read NinfoElement from string.
/*!
 *  read ninfo line as ninfo format.
 *  @tparam N_bodies template argument of NinfoElement.
 *  @tparam N_coefs  template argument of NinfoElement.
 *  @sa     NinfoElement
 */   
template<std::size_t N_bodies, std::size_t N_coefs, typename real_type>
std::basic_istream<char>&
operator>>(std::basic_istream<char>& is,
           NinfoElement<N_bodies, N_coefs, real_type>& ninfo)
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

}//mill

#endif //COFFEE_MILL_NINFO_BASE
