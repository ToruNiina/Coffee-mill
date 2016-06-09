/*!
  @file DCDData.hpp
  @brief definition of a class that is a set of data dcd file contains.

  definition of class DCDData that is a set of data dcd file contains.
  The data is composed of header information(timestep, delta_t, etc...) 
  and trajectory(array of array of Vector3d). This behaves like
  std::vector<std::vector<Vector3d>> with iterator, front()/back(), at/[].
  
  @author Toru Niina (niina.toru.68@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_DATA
#define COFFEE_MILL_DCD_DATA
#include "math/LinearAlgebra.hpp"

namespace coffeemill
{

//! DCD data class.
/*!
 *  DCDData contains trajectory(vector of vector of 3DVector) and
 *  header information(like total step, dt, comment, etc) of DCD file.
 */
class DCDData
{
  public:

    using size_type       = std::size_t;
    using index_type      = size_type;
    using time_type       = double;
    //! use AX library as vector arithmetics
    using position_type   = ax::Vector3d;
    //! definition of snapshot
    using snapshot_type   = std::vector<position_type>;
    //! definition of trajectory
    using trajectory_type = std::vector<snapshot_type>;
    using iterator        = trajectory_type::iterator;
    using const_iterator  = trajectory_type::const_iterator;
    using string_type     = std::string;
    using header_type     = std::vector<string_type>;

  public:

    //! ctor.
    DCDData(){}
    /*!
     *  ctor for known steps. initialize trajectory with size
     *  @param size is a size of trajectory(number of snapshots).
     *  @return DCDData object.
     */
    explicit DCDData(size_type size) : trajectory_(size){}

    //! dtor.
    ~DCDData() = default;

    //! number of frames.
    size_type  nset() const {return nset_;}
    //! number of frames.
    size_type& nset()       {return nset_;}

    //! initial step. normally 0.
    size_type  istart() const {return istart_;}
    //! initial step. normally 0.
    size_type& istart()       {return istart_;}

    //! output interval.
    size_type  nstep_save() const {return nstep_save_;}
    //! output interval.
    size_type& nstep_save()       {return nstep_save_;}

    //! total step.
    size_type  nstep() const {return nstep_;}
    //! total step.
    size_type& nstep()       {return nstep_;}

    //! number of units.
    size_type  nunit() const {return nunit_;}
    //! number of units.
    size_type& nunit()       {return nunit_;}

    //! version of CHARMM. in cafemol, normally, 24.
    size_type  verCHARMM() const {return verCHARMM_;}
    //! version of CHARMM. in cafemol, normally, 24.
    size_type& verCHARMM()       {return verCHARMM_;}

    //! number of particles in snapshot.
    size_type  nparticle() const {return nparticle_;}
    //! number of particles in snapshot.
    size_type& nparticle()       {return nparticle_;}

    //! delta t.
    time_type  delta_t() const {return delta_t_;}
    //! delta t.
    time_type& delta_t()       {return delta_t_;}

    //! file signeture. normally, CORD or VELO.
    const string_type& signeture() const {return signeture_;}
    //! file signeture. normally, CORD or VELO.
          string_type& signeture()       {return signeture_;}

    //! front snapshot of trajectory.
    const snapshot_type& front() const {return trajectory_.front();}
    //! front snapshot of trajectory.
          snapshot_type& front()       {return trajectory_.front();}

    //! back snapshot of trajectory.
    const snapshot_type& back() const {return trajectory_.back();}
    //! back snapshot of trajectory.
          snapshot_type& back()       {return trajectory_.back();}

    //! access to i-th snapshot.
    const snapshot_type& operator[](index_type i) const {return trajectory_[i];}
    //! access to i-th snapshot.
          snapshot_type& operator[](index_type i)       {return trajectory_[i];}
    //! access to i-th snapshot. safer one.
    const snapshot_type& at(index_type i) const {return trajectory_.at(i);}
    //! access to i-th snapshot. safer one.
          snapshot_type& at(index_type i)       {return trajectory_.at(i);}

    //! access to entire trajectory.
    const trajectory_type& traj() const {return trajectory_;}
    //! access to entire trajectory.
          trajectory_type& traj()       {return trajectory_;}

    //! return whether trajectory is empty or not.
    bool      empty() const {return trajectory_.empty();}
    //! return the size of trajectory. expected to be same as nstep.
    size_type size() const {return trajectory_.size();}

    //! iterator of the trajectory.
    iterator begin() {return trajectory_.begin();}
    //! iterator of the trajectory.
    iterator end()   {return trajectory_.end();}
    //! iterator of the trajectory.
    const_iterator cbegin() const {return trajectory_.cbegin();}
    //! iterator of the trajectory.
    const_iterator cend()   const {return trajectory_.cend();}

    //! header comment.
    const header_type& header() const {return header_;}
    //! header comment.
          header_type& header()       {return header_;}

    //! add comment to header. make the comment length 80 with char '='.
    void push_header(const std::string& message)
    {
        if(message.size() == 80)
            header_.push_back(message);
        else if(message.size() > 80)
            throw std::invalid_argument("too long message");
        else if(message.size() < 80)
            header_.push_back(message + std::string(80 - message.size(), '='));
    }

  private:

    size_type       nset_;       //!< number of frames.
    size_type       istart_;     //!< initial value of isteps (nomally zero)
    size_type       nstep_save_; //!< nstep_save timesteps per one snapshot
    size_type       nstep_;      //!< total step number of the simulation
    size_type       nunit_;      //!< total unit(chains) number
    size_type       verCHARMM_;  //!< version of CHARMM (in the case of cafemol, 24)
    size_type       nparticle_;  //!< total number of particle
    time_type       delta_t_;    //!< delta t.
    string_type     signeture_ = "CORD"; //!< CORD or VELO
    header_type     header_;     //!< header comment. vector of string.
    trajectory_type trajectory_; //!< trajectory data.
};

}

#endif /* COFFEE_MILL_DCD_DATA */
