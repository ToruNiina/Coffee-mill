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

#ifndef COFFEE_MILL_DCD_DATA
#define COFFEE_MILL_DCD_DATA
#include "DefaultTraits.hpp"
#include <vector>

namespace coffeemill
{

template<typename T = DefaultTraits>
class DCDHeader
{
  public:

    using traits_type     = T;
    using size_type       = typename traits_type::size_type;
    using index_type      = size_type;
    using time_type       = typename traits_type::real_type;
    using position_type   = typename traits_type::position_type;
    using snapshot_type   = std::vector<position_type>;
    using trajectory_type = std::vector<snapshot_type>;
    using iterator        = trajectory_type::iterator;
    using const_iterator  = trajectory_type::const_iterator;
    using string_type     = typename traits_type::string_type;
    using comment_type    = std::vector<string_type>;

  public:
    DCDHeader() = default;
    ~DCDHeader() = default;

    //! number of frames.
    size_type  nset() const {return nset_;}
    size_type& nset()       {return nset_;}

    //! initial step. normally 0.
    size_type  istart() const {return istart_;}
    size_type& istart()       {return istart_;}

    //! output interval.
    size_type  nstep_save() const {return nstep_save_;}
    size_type& nstep_save()       {return nstep_save_;}

    //! total step.
    size_type  nstep() const {return nstep_;}
    size_type& nstep()       {return nstep_;}

    //! number of units.
    size_type  nunit() const {return nunit_;}
    size_type& nunit()       {return nunit_;}

    //! version of CHARMM. in cafemol, normally, 24.
    size_type  verCHARMM() const {return verCHARMM_;}
    size_type& verCHARMM()       {return verCHARMM_;}

    //! number of particles in snapshot.
    size_type  nparticle() const {return nparticle_;}
    size_type& nparticle()       {return nparticle_;}

    //! delta t.
    time_type  delta_t() const {return delta_t_;}
    time_type& delta_t()       {return delta_t_;}

    //! file signeture. normally, CORD or VELO.
    string_type const& signeture() const {return signeture_;}
    string_type      & signeture()       {return signeture_;}

    const comment_type& comment() const {return comment_;}
          comment_type& comment()       {return comment_;}

    //! add comment to header. make the comment length 80 with char '='.
    void push_comment(const std::string& message)
    {
        if(message.size() == 80)
            header_.push_back(message);
        else if(message.size() > 80)
            throw std::invalid_argument("too long message");
        else if(message.size() < 80)
            header_.push_back(message + std::string(80 - message.size(), '='));
    }

  private:

    size_type       nset_;               //!< number of frames.
    size_type       istart_;             //!< initial step of traj. nomally zero
    size_type       nstep_save_;         //!< interval of saving snapshot
    size_type       nstep_;              //!< total step number of the traj
    size_type       nunit_;              //!< total unit(chains) number
    size_type       verCHARMM_;          //!< version of CHARMM. in cafemol, 24
    size_type       nparticle_;          //!< total number of particle
    time_type       delta_t_;            //!< delta t.
    string_type     signeture_ = "CORD"; //!< CORD or VELO
    comment_type    comment_;             //!< header comment. vector of string.
};

template<typename T = DefaultTraits>
class DCDData
{
  public:

    using traits_type     = T;
    using size_type       = typename traits_type::size_type;
    using index_type      = size_type;
    using time_type       = typename traits_type::real_type;
    using position_type   = typename traits_type::position_type;
    using snapshot_type   = std::vector<position_type>;
    using trajectory_type = std::vector<snapshot_type>;
    using iterator        = trajectory_type::iterator;
    using const_iterator  = trajectory_type::const_iterator;
    using string_type     = typename traits_type::string_type;
    using header_type     = DCDHeader<traits_type>;
    using comment_type    = typename header_type::

  public:

    DCDData(){}
    explicit DCDData(size_type size) : trajectory_(size){}

    ~DCDData() = default;

    //! number of frames.
    size_type  nset() const {return header_.nset();}
    size_type& nset()       {return header_.nset();}

    //! initial step. normally 0.
    size_type  istart() const {return header_.istart();}
    size_type& istart()       {return header_.istart();}

    //! output interval.
    size_type  nstep_save() const {return header_.nstep_save();}
    size_type& nstep_save()       {return header_.nstep_save();}

    //! total step.
    size_type  nstep() const {return header_.nstep();}
    size_type& nstep()       {return header_.nstep();}

    //! number of units.
    size_type  nunit() const {return header_.nunit();}
    size_type& nunit()       {return header_.nunit();}

    //! version of CHARMM. in cafemol, normally, 24.
    size_type  verCHARMM() const {return header_.verCHARMM();}
    size_type& verCHARMM()       {return header_.verCHARMM();}

    //! number of particles in snapshot.
    size_type  nparticle() const {return header_.nparticle();}
    size_type& nparticle()       {return header_.nparticle();}

    //! delta t.
    time_type  delta_t() const {return header_.delta_t();}
    time_type& delta_t()       {return header_.delta_t();}

    //! file signeture. normally, CORD or VELO.
    string_type const& signeture() const {return header_.signeture();}
    string_type      & signeture()       {return header_.signeture();}

    //! front snapshot of trajectory.
    snapshot_type const& front() const {return trajectory_.front();}
    snapshot_type      & front()       {return trajectory_.front();}

    //! back snapshot of trajectory.
    snapshot_type const& back() const {return trajectory_.back();}
    snapshot_type      & back()       {return trajectory_.back();}

    //! access to i-th snapshot.
    snapshot_type const& operator[](index_type i) const {return trajectory_[i];}
    snapshot_type      & operator[](index_type i)       {return trajectory_[i];}
    snapshot_type const& at(index_type i) const {return trajectory_.at(i);}
    snapshot_type      & at(index_type i)       {return trajectory_.at(i);}

    //! access to entire trajectory.
    trajectory_type const& traj() const {return trajectory_;}
    trajectory_type      & traj()       {return trajectory_;}

    //! return whether trajectory is empty or not.
    bool      empty() const {return trajectory_.empty();}
    size_type size() const {return trajectory_.size();}

    //! iterator of the trajectory.
    iterator begin() {return trajectory_.begin();}
    iterator end()   {return trajectory_.end();}
    const_iterator cbegin() const {return trajectory_.cbegin();}
    const_iterator cend()   const {return trajectory_.cend();}

    //! header comment.
    const comment_type& comment() const {return header_.comment();}
          comment_type& comment()       {return header_.comment();}

    //! add comment to header. make the comment length 80 with char '='.
    void push_comment(const std::string& message)
    {
        header_.push_comment(message);
    }

  private:
    header_type     header_;
    trajectory_type trajectory_; //!< trajectory data.
};

}

#endif /* COFFEE_MILL_DCD_DATA */
