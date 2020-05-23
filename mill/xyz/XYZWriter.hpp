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
#include <mill/common/WriterBase.hpp>
#include <mill/util/logger.hpp>
#include <iomanip>
#include <fstream>

namespace mill
{

//! write trajectory data to .xyz file.
/*!
 *  @tparam vectorT type of position
 */
class XYZWriter final : public WriterBase
{
  public:
    using base_type                = WriterBase;
    using trajectory_type          = using base_type::trajectory_type;
    using snapshot_type            = using base_type::snapshot_type;
    using particle_type            = using base_type::particle_type;
    using attribute_container_type = using base_type::attribute_container_type;

  public:

    XYZWriter(const std::string& fname)
        : current_(0), file_name_(fname), xyz_(fname)
    {
        if(!xyz_.good())
        {
            throw std::runtime_error("XYZWriter: file open error: " + fname);
        }
    }
    ~XYZWriter() override = default;

    void write_header(const attribute_container_type&) override
    {
        return; // xyz does not have any header info
    }
    void write(const trajectory_type& traj) override
    {
        for(const auto& frame : traj)
        {
            this->write_frame(frame);
        }
        return;
    }
    void write_frame(const snapshot_type& frame) override
    {
        using namespace std::literals::string_literals;
        xyz_ << frame.size() << '\n';
        xyz_ << frame.try_at("comment").value_or(Attribute(""s)).as_string() << '\n';

        std::size_t max_width = 0;
        for(const auto& particle : frame)
        {
            const auto name = particle.try_at("name").value_or(Attribute("X"s));
            max_width = std::max(max_width, name.as_string().size());
        }
        for(const auto& particle : frame)
        {
            const auto name = particle.try_at("name").value_or(Attribute("X"s));
            xyz_ << std::setw(max_width + 1) << std::left << name.as_string();
            xyz_ << std::right;
            xyz_ << std::setw(18) << std::fixed << particle.position()[0] << ' ';
            xyz_ << std::setw(18) << std::fixed << particle.position()[1] << ' ';
            xyz_ << std::setw(18) << std::fixed << particle.position()[2] << '\n';
        }
        xyz_ << std::flush;
        return;
    }

    std::size_t      size()      const noexcept override {return current_;}
    std::string_view file_name() const noexcept override {return file_name_;}

  private:

    std::size_t current_;
    std::string file_name_;
    std::ofstream xyz_;
};

}// mill
#endif //COFFEE_MILL_DCD_READER
