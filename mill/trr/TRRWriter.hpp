#ifndef COFFEE_MILL_TRR_WRITER_HPP
#define COFFEE_MILL_TRR_WRITER_HPP
#include <mill/common/Trajectory.hpp>
#include <mill/common/WriterBase.hpp>
#include <mill/util/write_as_binary.hpp>
#include <mill/util/logger.hpp>
#include <fstream>

namespace mill
{

class TRRWriter final : public WriterBase
{
  public:
    using base_type                = WriterBase;
    using trajectory_type          = base_type::trajectory_type         ;
    using snapshot_type            = base_type::snapshot_type           ;
    using boundary_type            = snapshot_type::boundary_type       ;
    using particle_type            = base_type::particle_type           ;
    using vector_type              = particle_type::vector_type         ;
    using attribute_container_type = base_type::attribute_container_type;

  public:

    explicit TRRWriter(const std::string& fname)
        : current_(0), file_name_(fname),
          trr_(fname, std::ios::in | std::ios::binary)
    {
        if(!trr_.good())
        {
            throw std::runtime_error("TRRWriter: file open error: " + fname);
        }
    }
    ~TRRWriter() override = default;

    void write_header(const attribute_container_type&) override
    {
        return; // In trr, each frame has its own header.
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
        log::debug("mill::TRRWriter: writing a frame header...");

        write_as_binary<std::int32_t>(trr_, 1993);
        write_as_binary<std::int32_t>(trr_,   13);
        write_as_binary<std::int32_t>(trr_,   13);
        const auto title = "generated by Coffee-mill"s;
        trr_.write(title.data(), title.size());

        write_as_binary<std::int32_t>(trr_,    0); // ir
        write_as_binary<std::int32_t>(trr_,    0); // e
        if(frame.boundary().kind() == BoundaryConditionKind::CuboidalPeriodic)
        {
            write_as_binary<std::int32_t>(trr_, 9 * sizeof(double)); // box
        }
        else // Unlimited
        {
            write_as_binary<std::int32_t>(trr_, 0); // box
        }
        write_as_binary<std::int32_t>(trr_,    0); // vir
        write_as_binary<std::int32_t>(trr_,    0); // pres
        write_as_binary<std::int32_t>(trr_,    0); // top
        write_as_binary<std::int32_t>(trr_,    0); // sym
        write_as_binary<std::int32_t>(trr_, 3 * sizeof(double) * frame.size());
        if(!frame.empty() && frame.at(0).attributes().count("velocity") != 0)
        {
            write_as_binary<std::int32_t>(trr_, 3 * sizeof(double) * frame.size());
        }
        else
        {
            write_as_binary<std::int32_t>(trr_, 0);
        }
        if(!frame.empty() && frame.at(0).attributes().count("force") != 0)
        {
            write_as_binary<std::int32_t>(trr_, 3 * sizeof(double) * frame.size());
        }
        else
        {
            write_as_binary<std::int32_t>(trr_, 0);
        }
        write_as_binary<std::int32_t>(trr_, frame.size()); // # of particles
        if(const auto step = frame.try_at("step"))
        {
            write_as_binary<std::int32_t>(trr_, step->as_integer());
        }
        else
        {
            write_as_binary<std::int32_t>(trr_, 0);
        }
        write_as_binary<std::int32_t>(trr_, 0); // nre
        write_as_binary<double>(trr_, 0); // t
        write_as_binary<double>(trr_, 0); // lambda

        log::debug("mill::TRRWriter: done.");
        log::debug("mill::TRRWriter: writing frame...");

        if(frame.boundary().kind() == BoundaryConditionKind::CuboidalPeriodic)
        {
            const auto w = frame.boundary().width();
            write_as_binary<double>(trr_, w[0]);
            write_as_binary<double>(trr_, 0.0);
            write_as_binary<double>(trr_, 0.0);
            write_as_binary<double>(trr_, 0.0);
            write_as_binary<double>(trr_, w[1]);
            write_as_binary<double>(trr_, 0.0);
            write_as_binary<double>(trr_, 0.0);
            write_as_binary<double>(trr_, 0.0);
            write_as_binary<double>(trr_, w[2]);
        }

        for(const auto& p : frame)
        {
            write_as_binary<double>(trr_, p.position()[0]);
            write_as_binary<double>(trr_, p.position()[1]);
            write_as_binary<double>(trr_, p.position()[2]);
        }
        if(!frame.empty() && frame.at(0).attributes().count("velocity") != 0)
        {
            for(const auto& p : frame)
            {
                const auto vel = p.try_at("velocity").value_or(vector_type(0,0,0));
                write_as_binary<double>(trr_, vel[0]);
                write_as_binary<double>(trr_, vel[1]);
                write_as_binary<double>(trr_, vel[2]);
            }
        }
        if(!frame.empty() && frame.at(0).attributes().count("force") != 0)
        {
            for(const auto& p : frame)
            {
                const auto frc = p.try_at("force").value_or(vector_type(0,0,0));
                write_as_binary<double>(trr_, frc[0]);
                write_as_binary<double>(trr_, frc[1]);
                write_as_binary<double>(trr_, frc[2]);
            }
        }

        this->current_ += 1;
        log::debug("mill::TRRWriter: done.");
        return frame;
    }

    std::size_t      size()      const noexcept override {return current_;}
    std::string_view file_name() const noexcept override {return file_name_;}

  private:

    // To check the number of frames contained, store those values.
    std::size_t current_;
    std::string file_name_;
    std::ifstream trr_;
};

} // mill
#endif//COFFEE_MILL_TRR_WRITER_HPP
