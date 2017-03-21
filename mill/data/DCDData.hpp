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
#include "detail/scalar_type_extractor.hpp"
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
    using real_type       = typename scalar_type_extractor<vector_type>::type;
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

  public:

    DCDData(){}
    explicit DCDData(size_type size) : trajectory_(size){}

    ~DCDData() = default;

    int  nset() const {return header_.nset;}
    int& nset()       {return header_.nset;}
    int  istart() const {return header_.istart;}
    int& istart()       {return header_.istart;}
    int  nstep_save() const {return header_.nstep_save;}
    int& nstep_save()       {return header_.nstep_save;}
    int  nstep() const {return header_.nstep;}
    int& nstep()       {return header_.nstep;}
    int  nunit() const {return header_.nunit;}
    int& nunit()       {return header_.nunit;}
    int  verCHARMM() const {return header_.verCHARMM;}
    int& verCHARMM()       {return header_.verCHARMM;}
    int  nparticle() const {return header_.nparticle;}
    int& nparticle()       {return header_.nparticle;}
    time_type  delta_t() const {return header_.delta_t;}
    time_type& delta_t()       {return header_.delta_t;}
    std::string const& signeture() const {return header_.signeture;}
    std::string      & signeture()       {return header_.signeture;}
    comment_type const& comment() const {return header_.comment;}
    comment_type      & comment()       {return header_.comment;}
    header_type const& header() const {return header_;}
    header_type      & header()       {return header_;}

    snapshot_type const& front() const {return trajectory_.front();}
    snapshot_type      & front()       {return trajectory_.front();}
    snapshot_type const& back() const {return trajectory_.back();}
    snapshot_type      & back()       {return trajectory_.back();}

    snapshot_type const& operator[](index_type i) const {return trajectory_[i];}
    snapshot_type      & operator[](index_type i)       {return trajectory_[i];}
    snapshot_type const& at(index_type i) const {return trajectory_.at(i);}
    snapshot_type      & at(index_type i)       {return trajectory_.at(i);}

    trajectory_type const& traj() const {return trajectory_;}
    trajectory_type      & traj()       {return trajectory_;}

    bool      empty() const {return trajectory_.empty();}
    size_type size() const {return trajectory_.size();}

    iterator begin() {return trajectory_.begin();}
    iterator end()   {return trajectory_.end();}
    const_iterator cbegin() const {return trajectory_.cbegin();}
    const_iterator cend()   const {return trajectory_.cend();}

    //! add comment to header. make the comment length 80 with char '='.
    void append_comment(const std::string& message)
    {
        header_.append_comment(message);
    }

  private:

    header_type     header_; //!< header data. realT is deduced as a scalar_type of vectorT.
    trajectory_type trajectory_; //!< trajectory data.
};

}

#endif /* COFFEE_MILL_DCD_DATA */
