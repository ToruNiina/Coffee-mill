/*!
  @file XYZWriter.hpp
  @brief definition of a class that reads xyz file.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2017-01-05 17:30
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_XYZ_WRITER
#define COFFEE_MILL_XYZ_WRITER
#include "XYZData.hpp"
#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace mill
{

//! write trajectory data to .xyz file.
/*!
 *  @tparam vectorT type of position
 */
template<typename vectorT>
class XYZWriter
{
  public:
    using vector_type     = vectorT;
    using position_type   = vector_type;
    using frame_type      = XYZFrame<position_type>;
    using trajectory_type = std::vector<frame_type>;

  public:

    XYZWriter(const std::string& fname): filename_(fname)
    {
        // clear the content
        std::ofstream ofs(fname);
        if(!ofs.good())
        {
            throw std::runtime_error("XYZWriter: file open error: " + fname);
        }
    }
    ~XYZWriter() = default;

    void write(const trajectory_type& traj) const;
    void write(const frame_type&     frame) const;

  private:

    void write(std::ostream& os, const frame_type& frame) const;

  private:

    std::string filename_;
};

template<typename vectorT>
void XYZWriter<vectorT>::write(const trajectory_type& traj) const
{
    std::ofstream ofs(filename_, std::ios::app | std::ios::out);
    if(!ofs.good())
    {
        throw std::runtime_error("XYZWriter: file open error: " + filename_);
    }
    for(const auto& frame : traj)
    {
        this->write(ofs, frame);
    }
    return;
}

template<typename vectorT>
void XYZWriter<vectorT>::write(const frame_type& frame) const
{
    std::ofstream ofs(filename_, std::ios::app | std::ios::out);
    if(!ofs.good())
    {
        throw std::runtime_error("XYZWriter: file open error: " + filename_);
    }
    this->write(ofs, frame);
    return;
}

template<typename vectorT>
void XYZWriter<vectorT>::write(std::ostream& os, const frame_type& frame) const
{
    os << ss.size()     << '\n';
    os << frame.comment << '\n';
    for(const auto& particle : frame.particles)
    {
        os << particle.first     << ' ' << particle.second[0] << ' '
              particle.second[1] << ' ' << particle.second[2] << '\n';
    }
    return;   
}

}// mill
#endif //COFFEE_MILL_DCD_READER
