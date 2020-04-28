/*!
  @file XYZReader.hpp
  @brief definition of a class that reads xyz file.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2017-01-05 17:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_XYZ_READER_HPP
#define COFFEE_MILL_XYZ_READER_HPP
#include <mill/common/Trajectory.hpp>
#include <memory>
#include <utility>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace mill
{

//! read XYZfile and return the data as std::vector<std::vector<position>>.
/*!
 *  @tparam vectorT type of position
 */
class XYZReader
{
  public:
    using trajectory_type = Trajectory;
    using snapshot_type   = typename trajectory_type::snapshot_type;
    using particle_type   = typename snapshot_type::particle_type;
    using vector_type     = typename particle_type::vector_type;

  public:
    XYZReader(const std::string& fname): xyz_(fname)
    {
        if(!xyz_.good())
        {
            throw std::runtime_error("XYZReader: file open error: " + fname);
        }
    }
    ~XYZReader() = default;

    trajectory_type read()
    {
        trajectory_type traj;
        while(!this->xyz_.eof())
        {
            traj.snapshots().push_back(this->read_frame());
            this->xyz_.peek();
        }
        return traj;
    }
    snapshot_type   read_frame()
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
                    "but got " + line);
        }
        snapshot_type frame(N);

        std::getline(this->xyz_, line);
        frame["comment"] = line;

        for(std::size_t i=0; i<N; ++i)
        {
            std::getline(this->xyz_, line);
            std::istringstream iss(line);
            std::string atom;
            double x, y, z;
            iss >> atom >> x >> y >> z;

            particle_type p(vector_type(x, y, z), {{"name", atom}});
            frame.at(i) = std::move(p);
        }
        return frame;
    }

    bool is_eof() const noexcept {return xyz_.eof();}

  private:
    std::ifstream xyz_;
};

}// mill
#endif //COFFEE_MILL_XYZ_READER
