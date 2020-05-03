/*!
  @file XYZWriter.hpp
  @brief definition of a class that reads xyz file.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2017-01-05 17:30
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_XYZ_WRITER_HPP
#define COFFEE_MILL_XYZ_WRITER_HPP
#include <mill/common/Trajectory.hpp>
#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>

namespace mill
{

//! write trajectory data to .xyz file.
/*!
 *  @tparam vectorT type of position
 */
class XYZWriter
{
  public:
    using trajectory_type          = Trajectory;
    using snapshot_type            = Snapshot;
    using particle_type            = Particle;
    using attribute_container_type = Trajectory::attribute_container_type;

  public:

    XYZWriter(const std::string& fname)
        : current_(0), file_name_(fname), xyz_(fname)
    {
        if(!ofs.good())
        {
            throw std::runtime_error("XYZWriter: file open error: " + fname);
        }
    }
    ~XYZWriter() = default;

    void write_header(const attribute_container_type&)
    {
        return; // xyz does not have any header info
    }
    void write(const trajectory_type& traj) const
    {
        for(const auto& frame : traj)
        {
            this->write_frame(frame);
        }
        return;
    }
    void write_frame(const snapshot_type& frame) const
    {
        xyz_ << frame.size()                         << '\n';
        xyz_ << frame.try_at("comment").value_or("") << '\n';

        std::size_t max_width = 0;
        for(const auto& particle : frame)
        {
            using namespace std::literals::string_literals;
            const auto name = particle.try_at("name").value_or(Attribute("X"s));
            max_width = std::max(max_width, name.as_string().size());
        }
        for(const auto& particle : frame)
        {
            const auto name = particle.try_at("name").value_or(Attribute("X"s));
            xyz_ << std::setw(max_width + 1) << std::left << name.as_string();
            xyz_ << std::setw(20) << std::fixed << std::setprecision(17)
                 << particle.position()[0];
            xyz_ << std::setw(20) << std::fixed << std::setprecision(17)
                 << particle.position()[1];
            xyz_ << std::setw(20) << std::fixed << std::setprecision(17)
                 << particle.position()[2];
            xyz_ << '\n';
        }
        xyz_ << std::flush;
        return;
    }

    std::size_t      size()      const noexcept {return current_;}
    std::string_view file_name() const noexcept {return file_name_;}

  private:

    std::size_t current_;
    std::string file_name_;
    std::ifstream xyz_;
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
void XYZWriter<vectorT>::write(const snapshot_type& frame) const
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
void XYZWriter<vectorT>::write(std::ostream& os, const snapshot_type& frame) const
{
    using namespace std::literals::string_literals;
    os << frame.size() << '\n';
    os << frame.try_at("comment").value_or(attribute_type(""s))
               .try_string().value_or(""s) << '\n';
    for(const auto& particle : frame.particles())
    {
        os << particle.try_at("name"s).value_or(attribute_type("X"s))
                      .try_string().value_or("X"s) << ' ';
        os << std::fixed << std::setprecision(17) << std::setw(20) << particle.position()[0] << ' '
           << std::fixed << std::setprecision(17) << std::setw(20) << particle.position()[1] << ' '
           << std::fixed << std::setprecision(17) << std::setw(20) << particle.position()[2] << '\n';
    }
    return;
}

}// mill
#endif //COFFEE_MILL_DCD_READER
