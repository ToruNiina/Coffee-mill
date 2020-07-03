/*!
  @file DCDReader.hpp
  @brief definition of a class that reads dcd file.

  DCDReader reads dcd file and store the information included in the file.
  the data is stored as DCDData. Reader can read only a header region.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_READER_HPP
#define COFFEE_MILL_DCD_READER_HPP
#include <mill/common/Trajectory.hpp>
#include <mill/common/DeferedReader.hpp>
#include <mill/util/read_binary_as.hpp>
#include <mill/util/logger.hpp>
#include <fstream>

namespace mill
{

//! read DCDfile and return the data as a DCDData.
class DCDReader final : public DeferedReaderBase
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

    explicit DCDReader(const std::string& fname)
        : current_(0), file_name_(fname),
          dcd_(fname, std::ios::in | std::ios::binary)
    {
        if(!dcd_.good())
        {
            log::fatal("DCDReader: file open error: ", fname);
        }

        dcd_.seekg(0, dcd_.end);
        this->file_size_ = dcd_.tellg();
        dcd_.seekg(0, dcd_.beg);
    }
    ~DCDReader() override = default;

    attribute_container_type read_header() override
    {
        log::debug("mill::DCDReader: reading header ...");
        attribute_container_type attr;
        this->read_header_block1(attr);
        this->read_header_block2(attr);
        this->read_header_block3(attr);
        this->validate_filesize(attr);
        log::debug("mill::DCDReader: done.");
        return attr;
    }
    trajectory_type read() override
    {
        log::debug("mill::DCDReader: reading the whole trajectory...");
        this->rewind();

        trajectory_type traj(this->read_header());
        traj.snapshots().reserve(traj["nset"].as_integer());

        while(!this->dcd_.eof())
        {
            traj.snapshots().push_back(*read_frame());
            this->dcd_.peek();
        }
        log::debug("mill::DCDReader: done.");
        return traj;
    }
    std::optional<snapshot_type> read_frame() override
    {
        log::debug("mill::DCDReader: reading a frame...");
        if(this->num_particles_ == 0)
        {
            log::debug("mill::DCDReader: number of particles is 0. reading header.");
            this->read_header();
        }

        if(this->is_eof())
        {
            log::debug("mill::DCDReader: we are already at the EOF. done.");
            return std::nullopt;
        }

        snapshot_type frame(this->num_particles_);
        frame.boundary() = read_unit_cell();

        const auto x = read_coord();
        const auto y = read_coord();
        const auto z = read_coord();
        for(std::size_t i=0; i<num_particles_; ++i)
        {
            frame.at(i).position()[0] = x.at(i);
            frame.at(i).position()[1] = y.at(i);
            frame.at(i).position()[2] = z.at(i);
        }
        this->current_ += 1;
        log::debug("mill::DCDReader: done.");

        this->dcd_.peek(); // to check currently we are in EOF.
        return frame;
    }
    std::optional<snapshot_type> read_frame(const std::size_t idx) override
    {
        log::debug("mill::DCDReader: reading ", idx, "-th frame...");
        this->rewind();

        const auto header_size = header1_size_ + header2_size_ + header3_size_;
        const auto pos = header_size + snapshot_size_ * idx;

        if(file_size_ <= pos)
        {
            return std::nullopt;
        }
        dcd_.seekg(pos, dcd_.beg);
        this->current_ = idx;

        const auto frame = this->read_frame();
        log::debug("mill::DCDReader: done.");
        this->dcd_.peek(); // to check currently we are in EOF.
        return frame;
    }

    void rewind() override
    {
        log::debug("mill::DCDReader: rewinding the file");
        current_ = 0;
        dcd_.clear(); // clear failbit flags like the EOF flag
        dcd_.seekg(0, std::ios_base::beg);
        dcd_.peek(); // check whether we are at the EOF
        log::debug("mill::DCDReader: done.");
        return;
    }
    bool             is_eof()    const noexcept override {return dcd_.eof();}
    std::size_t      current()   const noexcept override {return current_;}
    std::string_view file_name() const noexcept override {return file_name_;}

  private:

    std::vector<float> read_coord()
    {
        const int size_of_block = read_binary_as<int>(dcd_);
        if(size_of_block / sizeof(float) != num_particles_)
        {
            log::fatal("invalid coordinate block!\n",
                       "size of coordinate block is ", size_of_block, '\n',
                       "but the number of particle is  ", num_particles_);
        }

        std::vector<float> coordinate(num_particles_, 0.0f);
        for(auto& v : coordinate)
        {
            v = read_binary_as<float>(dcd_);
        }

        const int size_of_block_f = read_binary_as<int>(dcd_);
        if(size_of_block != size_of_block_f)
        {
            log::fatal("invalid coordinate block delimiter\n",
                       "the size of the block is not ", size_of_block_f,
                       " but ", size_of_block);
        }
        return coordinate;
    }

    void read_header_block1(attribute_container_type& header)
    {
        const auto block_size = read_binary_as<std::int32_t>(dcd_);
        if(block_size != 84)
        {
            log::warn("Unrecognized header block size, ", block_size);
        }

        char signature[5];
        dcd_.read(signature, 4);
        signature[4] = '\0';
        header["signature"] = std::string(signature);

        if(header["signature"].as_string() != "CORD" &&
           header["signature"].as_string() != "VELO")
        {
            log::warn("Unknown file signature -> ", signature);
        }

        header["nset"      ] = read_binary_as<std::int32_t>(dcd_);
        header["istart"    ] = read_binary_as<std::int32_t>(dcd_);
        header["nstep_save"] = read_binary_as<std::int32_t>(dcd_);
        header["nstep"     ] = read_binary_as<std::int32_t>(dcd_);
        header["nunit"     ] = read_binary_as<std::int32_t>(dcd_);
        dcd_.ignore(16);
        header["delta_t"   ] = read_binary_as<float>(dcd_);
        this->has_unitcell_     = read_binary_as<std::int32_t>(dcd_);
        dcd_.ignore(32);
        header["verCHARMM" ] = read_binary_as<std::int32_t>(dcd_);

        log::debug("signature  = ", header.at("signature") .as_string()  );
        log::debug("nset       = ", header.at("nset")      .as_integer() );
        log::debug("istart     = ", header.at("istart")    .as_integer() );
        log::debug("nstep_save = ", header.at("nstep_save").as_integer() );
        log::debug("nstep      = ", header.at("nstep")     .as_integer() );
        log::debug("nunit      = ", header.at("nunit")     .as_integer() );
        log::debug("delta_t    = ", header.at("delta_t")   .as_floating());

        const auto block_size_check = read_binary_as<std::int32_t>(dcd_);
        if(block_size != block_size_check)
        {
            log::fatal("mill::DCDReader: header block 1 is broken.\n",
                       "first block size (", block_size, ") != last (",
                       block_size_check, ").");
        }
        this->header1_size_ = block_size + sizeof(std::int32_t) * 2;
        return;
    }
    void read_header_block2(attribute_container_type& header)
    {
        const auto block_size = read_binary_as<std::int32_t>(dcd_);

        const auto lines = read_binary_as<std::int32_t>(dcd_);
        if((80 * lines + sizeof(std::int32_t)) != static_cast<std::size_t>(block_size))
        {
            log::fatal("mill::DCDReader: header block 2 is broken.\n",
                       "block size (", block_size, ") is not consistent with ",
                       "the number of lines (", lines, ").");
        }

        std::string comment;
        for(std::int32_t i(0); i<lines; ++i)
        {
            char line[81];
            dcd_.read(line, 80);
            line[80] = '\0';
            comment += std::string(line);
        }
        header["comment"] = comment;
        log::debug("comment = ", header.at("comment").as_string());

        const auto block_size_check = read_binary_as<std::int32_t>(dcd_);
        if(block_size != block_size_check)
        {
            log::fatal("mill::DCDReader: header block 2 is broken.\n",
                       "first block size (", block_size, ") != last (",
                       block_size_check, ").");
        }
        header2_size_ = block_size + sizeof(std::int32_t) * 2;
        return;
    }
    void read_header_block3(attribute_container_type& header)
    {
        const auto block_size = read_binary_as<std::int32_t>(dcd_);

        this->num_particles_   = read_binary_as<std::int32_t>(dcd_);
        header["nparticle"] = num_particles_;
        log::debug("nparticle = ", header.at("nparticle").as_integer());

        const auto block_size_check = read_binary_as<std::int32_t>(dcd_);
        if(block_size != block_size_check)
        {
            log::fatal("mill::DCDReader: header block 3 is broken.\n",
                       "first block size (", block_size, ") != last (",
                       block_size_check, ").");
        }
        header3_size_ = block_size + sizeof(std::int32_t) * 2;
        return;
    }

    void validate_filesize (attribute_container_type& header)
    {
        this->snapshot_size_ = 12 * (header.at("nparticle").as_integer() + 2);
        if(this->has_unitcell_)
        {
            this->snapshot_size_ += sizeof(std::int32_t) * 2 + sizeof(double) * 6;
        }
        const auto header_size = header1_size_ + header2_size_ + header3_size_;
        const auto expected_size = header_size +
            snapshot_size_ * header.at("nset").as_integer();

        if(file_size_ != expected_size)
        {
            log::warn("invalid filesize!");
            log::warn("actual file size is ", file_size_, " bytes.");
            log::warn("header says there are ", header.at("nset").as_integer(),
                      " snapshots.");
            log::warn("and there are ", header.at("nparticle").as_integer(),
                      " particles.");
            log::warn("The file must have ",  expected_size, " bytes");

            const auto traj_size = file_size_ - header_size;
            if(traj_size % snapshot_size_ != 0)
            {
                log::warn("The last snapshot seems to be incomplete.");
            }
            header.at("nset") = traj_size / snapshot_size_;
            log::warn("The actual number of snapshots seems to be ",
                      header.at("nset").as_integer(), ".");
        }
        return;
    }

    boundary_type read_unit_cell()
    {
        if(not this->has_unitcell_)
        {
            return boundary_type(UnlimitedBoundary{});
        }
        const auto block_begin = read_binary_as<std::int32_t>(dcd_);
        const auto A           = read_binary_as<double      >(dcd_);
        const auto gamma       = read_binary_as<double      >(dcd_);
        const auto B           = read_binary_as<double      >(dcd_);
        const auto beta        = read_binary_as<double      >(dcd_);
        const auto alpha       = read_binary_as<double      >(dcd_);
        const auto C           = read_binary_as<double      >(dcd_);
        const auto block_end   = read_binary_as<std::int32_t>(dcd_);

        if(block_begin != block_end)
        {
            log::error("DCD file has invalid unit cell information.");
            log::error("block_begin = ", block_begin, ", block_end = ", block_end);
            log::error("A     = ", A,     ", B    = ", B,    ", C     = ", C     );
            log::error("alpha = ", alpha, ", beta = ", beta, ", gamma = ", gamma );
            return boundary_type(UnlimitedBoundary{});
        }

        const auto differs = [](const double lhs, const double rhs) noexcept -> bool {
            constexpr double rel_tol = 1e-8;
            constexpr double abs_tol = 1e-10;
            return abs_tol                     < std::abs(lhs - rhs) ||
                   rel_tol * 0.5 * (lhs + rhs) < std::abs(lhs - rhs);
        };
        constexpr double half_pi = 1.57079632679;

        if((differs(alpha, 90.0) && differs(alpha, std::cos(half_pi))) ||
           (differs(beta,  90.0) && differs(beta,  std::cos(half_pi))) ||
           (differs(gamma, 90.0) && differs(gamma, std::cos(half_pi))))
        {
            log::error("The unit cell is not a rectangle");
            log::error("angle alpha = ", alpha);
            log::error("angle beta  = ", beta );
            log::error("angle gamma = ", gamma);
        }
        return boundary_type(CuboidalPeriodicBoundary(
                vector_type(0, 0, 0), vector_type(A, B, C)));
    }

  private:

    // To check the number of frames contained, store those values.
    std::size_t file_size_     = 0;
    std::size_t header1_size_  = 0;
    std::size_t header2_size_  = 0;
    std::size_t header3_size_  = 0;
    std::size_t snapshot_size_ = 0;
    std::size_t num_particles_ = 0;
    std::size_t current_;
    std::int32_t has_unitcell_ = 0; // a flag. dcd header uses i32 as a flag.
    std::string file_name_;
    std::ifstream dcd_;
};

}// mill
#endif //COFFEE_MILL_DCD_READER
