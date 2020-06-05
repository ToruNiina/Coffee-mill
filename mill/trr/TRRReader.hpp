#ifndef COFFEE_MILL_TRR_READER_HPP
#define COFFEE_MILL_TRR_READER_HPP
#include <mill/common/Trajectory.hpp>
#include <mill/common/DeferedReader.hpp>
#include <mill/util/read_binary_as.hpp>
#include <mill/util/logger.hpp>
#include <fstream>

namespace mill
{

class TRRReader final : public DeferedReaderBase
{
  public:
    using base_type = DeferedReaderBase;
    using trajectory_type          = base_type::trajectory_type         ;
    using snapshot_type            = base_type::snapshot_type           ;
    using boundary_type            = snapshot_type::boundary_type       ;
    using particle_type            = base_type::particle_type           ;
    using vector_type              = particle_type::vector_type         ;
    using attribute_container_type = base_type::attribute_container_type;

  public:

    explicit TRRReader(const std::string& fname)
        : current_(0), file_name_(fname),
          trr_(fname, std::ios::in | std::ios::binary)
    {
        if(!trr_.good())
        {
            throw std::runtime_error("TRRReader: file open error: " + fname);
        }
        trr_.seekg(0, trr_.end);
        this->file_size_ = trr_.tellg();
        trr_.seekg(0, trr_.beg);
    }
    ~TRRReader() override = default;

    attribute_container_type read_header() override
    {
        // In TRR file, each snapshot have its own header. So there is no header
        // region for the whole file.
        // Still we need to calculate the size of each snapshot, we read the
        // header of the first snapshot here. So the header of the trajectory
        // will be the same as the header of the first snapshot.
        log::debug("mill::TRRReader: reading header ...");

        attribute_container_type attr;
        this->read_header_block(attr);

        if(this->file_size_ % this->snapshot_size_ != 0)
        {
            log::warn("mill::TRRReader: file_size (", file_size_, ") is not a "
                    "multiple of snapshot_size (", snapshot_size_, "). This "
                    "might cause a problem later.");
        }
        this->rewind(); // all the snapshot has its own header.

        log::debug("mill::TRRReader: done.");
        return attr;
    }
    trajectory_type read() override
    {
        log::debug("mill::TRRReader: reading the whole trajectory...");
        this->rewind();

        trajectory_type traj(this->read_header());

        while(!this->trr_.eof())
        {
            traj.snapshots().push_back(*read_frame());
            this->trr_.peek();
        }
        log::debug("mill::TRRReader: done.");
        return traj;
    }
    std::optional<snapshot_type> read_frame() override
    {
        log::debug("mill::TRRReader: reading a frame...");
        if(this->num_particles_ == 0)
        {
            this->read_header();
        }

        if(this->is_eof())
        {
            return std::nullopt;
        }
        snapshot_type frame;
        this->read_header_block(frame.attributes());
        frame.particles().resize(frame.at("number_of_particles").as_integer());

        log::debug("this snapshot contains ", frame.particles().size(), " particles");

        if(frame.at("ir_size").as_integer() != 0)
        {
            trr_.ignore(frame.at("ir_size").as_integer());
        }
        if(frame.at("e_size").as_integer() != 0)
        {
            trr_.ignore(frame.at("e_size").as_integer());
        }
        if(frame.at("box_size").as_integer() != 0)
        {
            frame.boundary() = this->read_boundary(frame);
        }
        if(frame.at("vir_size").as_integer() != 0)
        {
            trr_.ignore(frame.at("vir_size").as_integer());
        }
        if(frame.at("pres_size").as_integer() != 0)
        {
            trr_.ignore(frame.at("pres_size").as_integer());
        }
        if(frame.at("top_size").as_integer() != 0)
        {
            trr_.ignore(frame.at("top_size").as_integer());
        }
        if(frame.at("sym_size").as_integer() != 0)
        {
            trr_.ignore(frame.at("sym_size").as_integer());
        }
        if(frame.at("position_size").as_integer() != 0)
        {
            for(std::int64_t i=0; i<frame.at("number_of_particles").as_integer(); ++i)
            {
                if(frame.at("precision").as_string() == "float")
                {
                    frame.at(i).position()[0] = read_binary_as<float>(trr_);
                    frame.at(i).position()[1] = read_binary_as<float>(trr_);
                    frame.at(i).position()[2] = read_binary_as<float>(trr_);
                }
                else if(frame.at("precision").as_string() == "double")
                {
                    frame.at(i).position()[0] = read_binary_as<double>(trr_);
                    frame.at(i).position()[1] = read_binary_as<double>(trr_);
                    frame.at(i).position()[2] = read_binary_as<double>(trr_);
                }
            }
        }
        if(frame.at("velocity_size").as_integer() != 0)
        {
            for(std::int64_t i=0; i<frame.at("number_of_particles").as_integer(); ++i)
            {
                vector_type vel;
                if(frame.at("precision").as_string() == "float")
                {
                    vel[0] = read_binary_as<float>(trr_);
                    vel[1] = read_binary_as<float>(trr_);
                    vel[2] = read_binary_as<float>(trr_);
                }
                else if(frame.at("precision").as_string() == "double")
                {
                    vel[0] = read_binary_as<double>(trr_);
                    vel[1] = read_binary_as<double>(trr_);
                    vel[2] = read_binary_as<double>(trr_);
                }
                frame.at(i)["velocity"] = vel;
            }
        }
        if(frame.at("force_size").as_integer() != 0)
        {
            for(std::int64_t i=0; i<frame.at("number_of_particles").as_integer(); ++i)
            {
                vector_type frc;
                if(frame.at("precision").as_string() == "float")
                {
                    frc[0] = read_binary_as<float>(trr_);
                    frc[1] = read_binary_as<float>(trr_);
                    frc[2] = read_binary_as<float>(trr_);
                }
                else if(frame.at("precision").as_string() == "double")
                {
                    frc[0] = read_binary_as<double>(trr_);
                    frc[1] = read_binary_as<double>(trr_);
                    frc[2] = read_binary_as<double>(trr_);
                }
                frame.at(i)["force"] = frc;
            }
        }

        this->current_ += 1;
        this->trr_.peek();
        log::debug("mill::TRRReader: done.");
        return frame;
    }
    std::optional<snapshot_type> read_frame(const std::size_t idx) override
    {
        log::debug("mill::TRRReader: reading ", idx, "-th frame...");
        this->rewind();

        const auto pos = snapshot_size_ * idx;

        if(file_size_ <= pos)
        {
            return std::nullopt;
        }
        trr_.seekg(pos, trr_.beg);
        this->current_ = idx;

        const auto frame = this->read_frame();
        log::debug("mill::TRRReader: done.");
        return frame;
    }

    void rewind() override
    {
        log::debug("mill::TRRReader: rewinding the file");
        current_ = 0;
        trr_.seekg(0, std::ios_base::beg);
        log::debug("mill::TRRReader: done.");
        return;
    }
    bool             is_eof()    const noexcept override {return trr_.eof();}
    std::size_t      current()   const noexcept override {return current_;}
    std::string_view file_name() const noexcept override {return file_name_;}

  private:

    void read_header_block(attribute_container_type& header)
    {
        header["magic_number"] = read_binary_as<std::int32_t>(trr_);
        header["version"]      = read_binary_as<std::int32_t>(trr_);

        const auto len_title = read_binary_as<std::int32_t>(trr_);
        std::vector<char> title(len_title);
        trr_.read(title.data(), len_title);
        {
            std::string t(title.begin(), title.end());
            header["title"] = t;
            log::debug("TRR file title is ", t);
        }

        header["ir_size"]       = read_binary_as<std::int32_t>(trr_);
        header["e_size"]        = read_binary_as<std::int32_t>(trr_);
        header["box_size"]      = read_binary_as<std::int32_t>(trr_);
        header["vir_size"]      = read_binary_as<std::int32_t>(trr_);
        header["pres_size"]     = read_binary_as<std::int32_t>(trr_);
        header["top_size"]      = read_binary_as<std::int32_t>(trr_);
        header["sym_size"]      = read_binary_as<std::int32_t>(trr_);
        header["position_size"] = read_binary_as<std::int32_t>(trr_);
        header["velocity_size"] = read_binary_as<std::int32_t>(trr_);
        header["force_size"]    = read_binary_as<std::int32_t>(trr_);
        header["number_of_particles"] = read_binary_as<std::int32_t>(trr_);
        header["step"]          = read_binary_as<std::int32_t>(trr_); // step
        header["nre"]           = read_binary_as<std::int32_t>(trr_); // nre

        this->num_particles_ = header.at("number_of_particles").as_integer();

        log::debug("there are ", this->num_particles_, " particles");

        const auto sizeof_real = header["position_size"].as_integer() /
                                 header["number_of_particles"].as_integer() / 3;
        if(sizeof_real == 4)
        {
            using real_type  = float;
            header["precision"] = std::string("float");
            header["t"]      = read_binary_as<real_type>(trr_);
            header["lambda"] = read_binary_as<real_type>(trr_);
        }
        else if(sizeof_real == 8)
        {
            using real_type  = double;
            header["precision"] = std::string("double");
            header["t"]      = read_binary_as<real_type>(trr_);
            header["lambda"] = read_binary_as<real_type>(trr_);
        }
        else
        {
            log::error("invalid size of types: there are ",
                header.at("number_of_particles"), " particles, and size of "
                "position block is ", header.at("position_size"), ". the size "
                "of real type becomes ", sizeof_real);
        }
        log::debug("precision is ", header.at("precision"));

        this->snapshot_size_ = 64 + header.at("title").as_string().size() +
            header.at("ir_size")      .as_integer() +
            header.at("e_size")       .as_integer() +
            header.at("box_size")     .as_integer() +
            header.at("vir_size")     .as_integer() +
            header.at("pres_size")    .as_integer() +
            header.at("top_size")     .as_integer() +
            header.at("sym_size")     .as_integer() +
            header.at("position_size").as_integer() +
            header.at("velocity_size").as_integer() +
            header.at("force_size")   .as_integer() +
            2 * sizeof_real;

        log::debug("header loaded.");
        return;
    }

    BoundaryCondition read_boundary(const snapshot_type& frame)
    {
        vector_type X, Y, Z;
        if(frame.at("precision").as_string() == "double")
        {
            X[0] = read_binary_as<double>(this->trr_);
            X[1] = read_binary_as<double>(this->trr_);
            X[2] = read_binary_as<double>(this->trr_);
            Y[0] = read_binary_as<double>(this->trr_);
            Y[1] = read_binary_as<double>(this->trr_);
            Y[2] = read_binary_as<double>(this->trr_);
            Z[0] = read_binary_as<double>(this->trr_);
            Z[1] = read_binary_as<double>(this->trr_);
            Z[2] = read_binary_as<double>(this->trr_);
        }
        else if(frame.at("precision").as_string() == "float")
        {
            X[0] = read_binary_as<float>(this->trr_);
            X[1] = read_binary_as<float>(this->trr_);
            X[2] = read_binary_as<float>(this->trr_);
            Y[0] = read_binary_as<float>(this->trr_);
            Y[1] = read_binary_as<float>(this->trr_);
            Y[2] = read_binary_as<float>(this->trr_);
            Z[0] = read_binary_as<float>(this->trr_);
            Z[1] = read_binary_as<float>(this->trr_);
            Z[2] = read_binary_as<float>(this->trr_);
        }
        else
        {
            log::error("invalid precision while trr file reading");
        }

        if(X[1] != 0.0 || X[2] != 0.0 || Y[0] != 0.0 || Y[2] != 0.0 ||
           Z[0] != 0.0 || Z[1] != 0.0)
        {
            log::error("The unit cell is not a rectangle");
            log::error("X axis vector = ", X, ' ', Y, ' ', Z);
            log::error("X axis vector = ", X, ' ', Y, ' ', Z);
            log::error("X axis vector = ", X, ' ', Y, ' ', Z);
        }

        return BoundaryCondition(CuboidalPeriodicBoundary(
                vector_type(0.0, 0.0, 0.0), vector_type(X[0], Y[1], Z[2])));
    }

  private:

    // To check the number of frames contained, store those values.
    std::size_t file_size_     = 0;
    std::size_t snapshot_size_ = 0;
    std::size_t num_particles_ = 0;
    std::size_t current_;
    std::string file_name_;
    std::ifstream trr_;
};




} // mill
#endif//COFFEE_MILL_TRR_READER_HPP
