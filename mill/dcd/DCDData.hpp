/*!
  @file DCDData.hpp
  @brief definition of a class that is a set of data dcd file contains.

  definition of class DCDData that is a set of data dcd file contains.
  The data is composed of header information(timestep, delta_t, etc...)
  and trajectory(array of array of Vector3d). This behaves like
  std::vector<std::vector<Vector3d>> with iterator, front()/back(), at/[].

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_DATA_HPP
#define COFFEE_MILL_DCD_DATA_HPP
#include <mill/common/BoundaryCondition.hpp>
#include <mill/util/scalar_type_of.hpp>
#include <vector>
#include <string>

namespace mill
{

//! contains Header information of DCD file.
/*!
 *  @tparam realT typeof real-number parameter (normally, double).
 */
template<typename realT>
struct DCDHeader
{
    using real_type    = realT;
    using comment_type = std::vector<std::string>;

    void append_comment(const std::string& comm)
    {
        if(comm.size() == 80)
        {
            comment.push_back(comm);
        }
        else if(comm.size() < 80)
        {
            std::size_t diff = 80 - comm.size();
            std::size_t pre  = diff / 2;
            std::size_t post = diff - pre;
            comment.push_back(
                    std::string(pre, '=') + comm + std::string(post, '='));
        }
        else if(comm.size() > 80)
        {
            auto iter = comm.cbegin();

            while(std::distance(iter, comm.cend()) > 80)
            {
                comment.push_back(std::string(iter, iter + 80));
                iter = iter + 80;
            }
            append_comment(std::string(iter, comm.cend()));
        }
        return;
    }

    int nset;       //!< number of frames.
    int istart;     //!< initial step of traj. nomally zero
    int nstep_save; //!< interval of saving snapshot
    int nstep;      //!< total step number of the traj
    int nunit;      //!< total unit(chains) number
    int verCHARMM;  //!< version of CHARMM. in cafemol, 24
    int nparticle;  //!< total number of particle
    real_type    delta_t;    //!< delta t.
    std::string  signeture;  //!< CORD or VELO
    comment_type comment;    //!< header comment. vector of string.
};

//! contains whole information of DCD file.
/*!
 *  @tparam vectorT type of position
 */
template<typename vectorT>
class DCDData
{
  public:

    using vector_type     = vectorT;
    using real_type       = typename scalar_type_of<vector_type>::type;
    using position_type   = vector_type;
    using time_type       = real_type;
    using size_type       = std::size_t;
    using index_type      = size_type;
    using snapshot_type   = std::vector<position_type>;
    using trajectory_type = std::vector<snapshot_type>;
    using iterator        = typename trajectory_type::iterator;
    using const_iterator  = typename trajectory_type::const_iterator;
    using header_type     = DCDHeader<real_type>;
    using comment_type    = typename header_type::comment_type;
    using boundary_type   = BoundaryCondition<vector_type>;
    using boundary_trajectory_type = std::vector<boundary_type>;

  public:

    DCDData() = default;
    ~DCDData() = default;
    DCDData(DCDData const&) = default;
    DCDData(DCDData&&)      = default;
    DCDData& operator=(DCDData const&) = default;
    DCDData& operator=(DCDData&&)      = default;
    explicit DCDData(size_type size) : trajectory_(size){}

    int                 nset()       const noexcept {return header_.nset;}
    int&                nset()             noexcept {return header_.nset;}
    int                 istart()     const noexcept {return header_.istart;}
    int&                istart()           noexcept {return header_.istart;}
    int                 nstep_save() const noexcept {return header_.nstep_save;}
    int&                nstep_save()       noexcept {return header_.nstep_save;}
    int                 nstep()      const noexcept {return header_.nstep;}
    int&                nstep()            noexcept {return header_.nstep;}
    int                 nunit()      const noexcept {return header_.nunit;}
    int&                nunit()            noexcept {return header_.nunit;}
    int                 verCHARMM()  const noexcept {return header_.verCHARMM;}
    int&                verCHARMM()        noexcept {return header_.verCHARMM;}
    int                 nparticle()  const noexcept {return header_.nparticle;}
    int&                nparticle()        noexcept {return header_.nparticle;}
    time_type           delta_t()    const noexcept {return header_.delta_t;}
    time_type&          delta_t()          noexcept {return header_.delta_t;}
    std::string const&  signeture()  const noexcept {return header_.signeture;}
    std::string&        signeture()        noexcept {return header_.signeture;}
    comment_type const& comment()    const noexcept {return header_.comment;}
    comment_type&       comment()          noexcept {return header_.comment;}
    header_type const&  header()     const noexcept {return header_;}
    header_type&        header()           noexcept {return header_;}

    snapshot_type const& front() const noexcept {return trajectory_.front();}
    snapshot_type&       front()       noexcept {return trajectory_.front();}
    snapshot_type const& back()  const noexcept {return trajectory_.back();}
    snapshot_type&       back()        noexcept {return trajectory_.back();}

    snapshot_type const& operator[](index_type i) const noexcept {return trajectory_[i];}
    snapshot_type&       operator[](index_type i)       noexcept {return trajectory_[i];}
    snapshot_type const& at(index_type i) const {return trajectory_.at(i);}
    snapshot_type&       at(index_type i)       {return trajectory_.at(i);}

    trajectory_type const& traj() const noexcept {return trajectory_;}
    trajectory_type&       traj()       noexcept {return trajectory_;}

    bool      empty() const noexcept {return trajectory_.empty();}
    size_type size()  const noexcept {return trajectory_.size();}

    iterator       begin()        noexcept {return trajectory_.begin();}
    iterator       end()          noexcept {return trajectory_.end();}
    const_iterator begin()  const noexcept {return trajectory_.begin();}
    const_iterator end()    const noexcept {return trajectory_.end();}
    const_iterator cbegin() const noexcept {return trajectory_.cbegin();}
    const_iterator cend()   const noexcept {return trajectory_.cend();}

    //! add comment to header. make the comment length 80 with char '='.
    void append_comment(const std::string& message)
    {
        header_.append_comment(message);
    }

    boundary_trajectory_type const& boundary() const noexcept {return boundary_trajectory_;}
    boundary_trajectory_type&       boundary()       noexcept {return boundary_trajectory_;}

  private:

    header_type     header_; //!< header data. realT is deduced as a scalar_type of vectorT.
    trajectory_type trajectory_; //!< trajectory data.
    boundary_trajectory_type boundary_trajectory_;
};

} // dcd
#endif /* COFFEE_MILL_DCD_DATA */
