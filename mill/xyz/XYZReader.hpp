/*!
  @file XYZReader.hpp
  @brief definition of a class that reads xyz file.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2017-01-05 17:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_XYZ_READER_HPP
#define COFFEE_MILL_XYZ_READER_HPP
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

//! read XYZfile and return the data as std::vector<std::vector<position>>.
/*!
 *  @tparam vectorT type of position
 */
template<typename vectorT>
class XYZReader
{
  public:
    using vector_type     = vectorT;
    using position_type   = vector_type;
    using frame_type      = XYZFrame<position_type>;
    using snapshot_type   = frame_type;
    using trajectory_type = std::vector<frame_type>;

  public:

    XYZReader(const std::string& fname): xyz_(fname)
    {
        if(!xyz_.good())
        {
            throw std::runtime_error("XYZReader: file open error: " + fname);
        }
    }
    ~XYZReader() = default;

    trajectory_type read();
    snapshot_type   read_frame();

    bool is_eof() const noexcept {return xyz_.eof();}

  private:
    std::ifstream xyz_;
};

template<typename vecT>
typename XYZReader<vecT>::trajectory_type XYZReader<vecT>::read()
{
    trajectory_type traj;
    while(!this->xyz_.eof())
    {
        traj.push_back(this->read_frame());
        this->xyz_.peek();
    }
    return traj;
}

template<typename vecT>
typename XYZReader<vecT>::frame_type XYZReader<vecT>::read_frame()
{
    std::string line;
    std::getline(this->xyz_, line);
    std::size_t N = 0;
    try
    {
        N = std::stoull(line);
    }
    catch(...)
    {
        throw std::runtime_error("XYZReader::read_frame: expected number, "
                "but get " + line);
    }
    frame_type frame;
    std::getline(this->xyz_, frame.comment);
    frame.particles.reserve(N);

    for(std::size_t i=0; i<N; ++i)
    {
        std::getline(this->xyz_, line);
        std::istringstream iss(line);
        std::string atom;
        double x, y, z;
        iss >> atom >> x >> y >> z;
        frame.particles.emplace_back(atom, vector_type(x, y, z));
    }
    return frame;
}

}// mill
#endif //COFFEE_MILL_XYZ_READER
