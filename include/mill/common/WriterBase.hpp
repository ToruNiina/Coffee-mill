#ifndef COFFEE_MILL_COMMON_WRITER_BASE_HPP
#define COFFEE_MILL_COMMON_WRITER_BASE_HPP
#include <mill/common/Trajectory.hpp>
#include <mill/common/Snapshot.hpp>
#include <mill/common/Particle.hpp>

namespace mill
{

class WriterBase
{
  public:
    using trajectory_type          = Trajectory;
    using snapshot_type            = Snapshot;
    using particle_type            = Particle;
    using attribute_container_type = Trajectory::attribute_container_type;
    using boundary_type            = Snapshot::boundary_type;
    using vector_type              = Particle::vector_type;

  public:

    virtual ~WriterBase() = default;

    virtual void write_header(const attribute_container_type&) = 0;
    virtual void write(const trajectory_type&) = 0;
    virtual void write_frame(const snapshot_type&) = 0;

    virtual std::size_t      size()      const noexcept = 0;
    virtual std::string_view file_name() const noexcept = 0;
};

} // mill
#endif// COFFEE_MILL_COMMON_WRITER_BASE_HPP
