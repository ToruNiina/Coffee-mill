/*!
  @file DCDWriter.hpp
  @brief definition of a class that writes dcd file.

  DCDWriter writes DCDData into DCD file. this can write only one snapshot
  step by step. But, when you write trajectory step by step, the consistency
  between header data and trajectory size will not be guaranteed.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_WRITER_HPP
#define COFFEE_MILL_DCD_WRITER_HPP
#include <mill/common/Trajectory.hpp>
#include <mill/common/WriterBase.hpp>
#include <mill/util/write_as_binary.hpp>
#include <mill/util/logger.hpp>
#include <fstream>

namespace mill
{

//! writes DCD into a file.
class DCDWriter final : public WriterBase
{
  public:
    using base_type                = WriterBase;
    using trajectory_type          = base_type::trajectory_type         ;
    using attribute_container_type = trajectory_type::attribute_container_type;
    using snapshot_type            = base_type::snapshot_type           ;
    using boundary_type            = base_type::boundary_type           ;
    using particle_type            = base_type::particle_type           ;
    using vector_type              = base_type::vector_type             ;

  public:

    DCDWriter(const std::string_view fname)
        : current_(0), file_name_(fname), dcd_(file_name_)
    {
        if(!dcd_.good())
        {
            log::fatal("DCDWriter: file open error: ", fname);
        }
    }
    ~DCDWriter() override = default;

    void write_header(const trajectory_type& traj) override
    {
        this->write_head_block1(traj.attributes());
        this->write_head_block2(traj.attributes());
        this->write_head_block3(traj.attributes());
        return;
    }
    void write_footer(const trajectory_type&) override
    {
        return;
    }
    void write(const trajectory_type& traj) override
    {
        this->write_header(traj);
        for(const auto& frame : traj)
        {
            this->write_frame(frame);
        }
        this->write_footer(traj);
        return;
    }
    void write_frame(const snapshot_type& frame) override
    {
        if(frame.boundary().kind() == BoundaryConditionKind::CuboidalPeriodic)
        {
            const auto& b = frame.boundary().as_periodic();

            const std::int32_t block_size = 6 * sizeof(double);
            write_as_binary<std::int32_t>(dcd_, block_size);
            write_as_binary<double      >(dcd_, b.width()[0]);
            write_as_binary<double      >(dcd_, 90.0);
            write_as_binary<double      >(dcd_, b.width()[1]);
            write_as_binary<double      >(dcd_, 90.0);
            write_as_binary<double      >(dcd_, 90.0);
            write_as_binary<double      >(dcd_, b.width()[2]);
            write_as_binary<std::int32_t>(dcd_, block_size);
        }

        std::vector<float> x(frame.size());
        std::vector<float> y(frame.size());
        std::vector<float> z(frame.size());
        for(std::size_t i=0; i<frame.size(); ++i)
        {
            x.at(i) = static_cast<float>(frame.at(i).position()[0]);
            y.at(i) = static_cast<float>(frame.at(i).position()[1]);
            z.at(i) = static_cast<float>(frame.at(i).position()[2]);
        }
        this->write_coord(x);
        this->write_coord(y);
        this->write_coord(z);
        return;
    }

    std::size_t      size()      const noexcept override {return current_;}
    std::string_view file_name() const noexcept override {return file_name_;}

  private:

    void write_head_block1(const attribute_container_type& header)
    {
        write_as_binary<std::int32_t>(dcd_, 84);

        const auto sign = header.at("signature").as_string();
        dcd_.write(sign.c_str(), 4);

        write_as_binary<std::int32_t>(dcd_, static_cast<std::int32_t>(header.at("nset")      .as_integer()));
        write_as_binary<std::int32_t>(dcd_, static_cast<std::int32_t>(header.at("istart")    .as_integer()));
        write_as_binary<std::int32_t>(dcd_, static_cast<std::int32_t>(header.at("nstep_save").as_integer()));
        write_as_binary<std::int32_t>(dcd_, static_cast<std::int32_t>(header.at("nstep")     .as_integer()));
        write_as_binary<std::int32_t>(dcd_, static_cast<std::int32_t>(header.at("nunit")     .as_integer()));

        for(std::size_t i=0; i<4; ++i){write_as_binary<std::int32_t>(dcd_, 0);}

        write_as_binary<float>(dcd_,
            static_cast<float>(header.at("delta_t").as_floating()));
        write_as_binary<std::int32_t>(dcd_, 0); // TODO boundary info flag.

        for(std::size_t i=0; i<8; ++i){write_as_binary<std::int32_t>(dcd_, 0);}

        write_as_binary<std::int32_t>(dcd_,
            static_cast<std::int32_t>(header.at("verCHARMM").as_integer()));
        write_as_binary<std::int32_t>(dcd_, 84);
        return;
    }
    void write_head_block2(const attribute_container_type&)
    {
        const char comment[81] = "====================== Generated by Coff"
                                 "ee-mill DCDWriter ======================";
        write_as_binary<std::int32_t>(dcd_, 84);
        write_as_binary<std::int32_t>(dcd_,  1);
        dcd_.write(comment, 80);
        write_as_binary<std::int32_t>(dcd_, 84);
        return;
    }
    void write_head_block3(const attribute_container_type& header)
    {
        write_as_binary<std::int32_t>(dcd_, 4);
        write_as_binary<std::int32_t>(dcd_,
                static_cast<std::int32_t>(header.at("nparticle").as_integer()));
        write_as_binary<std::int32_t>(dcd_, 4);
        return;
    }

    void write_coord(const std::vector<float>& coord)
    {
        const std::int32_t size = 4 * coord.size();
        write_as_binary(dcd_, size);
        for(const float v : coord)
        {
            write_as_binary(dcd_, v);
        }
        write_as_binary(dcd_, size);
        return;
    }

  private:

    std::size_t current_;
    std::string file_name_;
    std::ofstream dcd_;
};

}//mill
#endif //COFFEE_MILL_DCD_WRITER
