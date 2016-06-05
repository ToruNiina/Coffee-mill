#ifndef COFFEE_MILL_DCD_DATA
#define COFFEE_MILL_DCD_DATA
#include "math/LinearAlgebra.hpp"

namespace coffeemill
{

class DCDData
{
  public:

    using size_type       = std::size_t;
    using index_type      = size_type;
    using time_type       = double;
    using position_type   = ax::Vector3d;
    using snapshot_type   = std::vector<position_type>;
    using trajectory_type = std::vector<snapshot_type>;
    using iterator        = trajectory_type::iterator;
    using const_iterator  = trajectory_type::const_iterator;
    using string_type     = std::string;
    using header_type     = std::vector<string_type>;

  public:

    DCDData(){}
    DCDData(size_type size) : trajectory_(size){}
    ~DCDData() = default;

    size_type  nset() const {return nset_;}
    size_type& nset()       {return nset_;}
    size_type  istart() const {return istart_;}
    size_type& istart()       {return istart_;}
    size_type  nstep_save() const {return nstep_save_;}
    size_type& nstep_save()       {return nstep_save_;}
    size_type  nstep() const {return nstep_;}
    size_type& nstep()       {return nstep_;}
    size_type  nunit() const {return nunit_;}
    size_type& nunit()       {return nunit_;}
    size_type  verCHARMM() const {return verCHARMM_;}
    size_type& verCHARMM()       {return verCHARMM_;}
    size_type  nparticle() const {return nparticle_;}
    size_type& nparticle()       {return nparticle_;}
    time_type  delta_t() const {return delta_t_;}
    time_type& delta_t()       {return delta_t_;}

    const snapshot_type& front() const {return trajectory_.front();}
          snapshot_type& front()       {return trajectory_.front();}
    const snapshot_type& back() const {return trajectory_.back();}
          snapshot_type& back()       {return trajectory_.back();}
    const snapshot_type& operator[](index_type i) const {return trajectory_[i];}
          snapshot_type& operator[](index_type i)       {return trajectory_[i];}
    const snapshot_type& at(index_type i) const {return trajectory_.at(i);}
          snapshot_type& at(index_type i)       {return trajectory_.at(i);}

    const trajectory_type& traj() const {return trajectory_;}
          trajectory_type& traj()       {return trajectory_;}

    bool      empty() const {return trajectory_.empty();}
    size_type size() const {return trajectory_.size();}

    iterator begin() {return trajectory_.begin();}
    iterator end()   {return trajectory_.end();}
    const_iterator cbegin() const {return trajectory_.cbegin();}
    const_iterator cend()   const {return trajectory_.cend();}

    const header_type&     header() const {return header_;}
          header_type&     header()       {return header_;}

    void push_header(const std::string& message)
    {
        if(message.size() == 80)
            header_.push_back(message);
        else if(message.size() > 80)
            throw std::invalid_argument("too long message");
        else if(message.size() < 80)
            header_.push_back(message + std::string(80 - message.size()), '=');
    }

  private:

    size_type       nset_;
    size_type       istart_;     // initial value of isteps (nomally zero)
    size_type       nstep_save_; // nstep_save timesteps per one snapshot
    size_type       nstep_;      // total step number of the simulation
    size_type       nunit_;      // total unit number (chains??)
    size_type       verCHARMM_;  // version of CHARMM (usually unused)
    size_type       nparticle_;  // total number of particle
    time_type       delta_t_;
    header_type     header_;
    trajectory_type trajectory_;
};

}

#endif /* COFFEE_MILL_DCD_DATA */
