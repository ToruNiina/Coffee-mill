/*!
  @file XYZReader.hpp
  @brief definition of a class that reads xyz file.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2017-01-05 17:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_XYZ_READER_HPP
#define COFFEE_MILL_XYZ_READER_HPP
#include <mill/util/logger.hpp>
#include <mill/common/Trajectory.hpp>
#include <mill/common/DeferedReader.hpp>
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
class XYZReader final : public DeferedReaderBase
{
  public:
    using base_type = DeferedReaderBase;
    using trajectory_type          = base_type::trajectory_type         ;
    using snapshot_type            = base_type::snapshot_type           ;
    using particle_type            = base_type::particle_type           ;
    using attribute_container_type = base_type::attribute_container_type;

  public:

    explicit XYZReader(const std::string& fname)
        : current_(0), file_name_(fname), xyz_(fname)
    {
        if(!xyz_.good())
        {
            throw std::runtime_error("XYZReader: file open error: " + fname);
        }
    }
    ~XYZReader() override = default;

    attribute_container_type read_header() override
    {
        mill::log::debug("mill::XYZReader: reading header ...\n");
        mill::log::debug("mill::XYZReader: done.\n");
        return attribute_container_type{};
    }
    trajectory_type read() override
    {
        mill::log::debug("mill::XYZReader: reading the whole trajectory...\n");
        this->rewind();

        trajectory_type traj;
        while(!this->xyz_.eof())
        {
            traj.snapshots().push_back(this->read_frame());
            this->xyz_.peek();
        }
        mill::log::debug("mill::XYZReader: done.\n");
        return traj;
    }
    snapshot_type   read_frame() override
    {
        mill::log::debug("mill::XYZReader: reading the next snapshot...\n");

        std::string line;
        std::getline(this->xyz_, line);

        const std::size_t N = convert_to_ull(line);
        snapshot_type frame(N);

        std::getline(this->xyz_, line);
        frame["comment"] = line;
        mill::log::debug("mill::XYZReader: comment of the next snapshot is ",
                         line, "\n");

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
        current_ += 1;
        xyz_.peek();
        mill::log::debug("mill::XYZReader: done.\n");
        return frame;
    }

    snapshot_type   read_frame(const std::size_t idx) override
    {
        mill::log::debug("mill::XYZReader: reading the ", idx, "-th snapshot\n");
        this->rewind();

        // skip n snapshots
        std::string line;
        for(std::size_t i=0; i<idx; ++i)
        {
            if(this->is_eof())
            {
                mill::log::error("mill::XYZReader: ", idx, "-th snapshot"
                                 " does not exist\n");
                throw std::out_of_range("mill::XYZReader: frame index out of range");
            }
            std::getline(this->xyz_, line);
            const std::size_t N = convert_to_ull(line);

            // skip comment line and N particles
            for(std::size_t j=0; j<N+1; ++j)
            {
                std::getline(this->xyz_, line);
            }
            xyz_.peek();
        }
        mill::log::debug("mill::XYZReader: done.\n");
        return this->read_frame();
    }

    void rewind() override
    {
        mill::log::debug("mill::XYZReader: rewinding the file\n");
        current_ = 0;
        xyz_.seekg(0, std::ios_base::beg);
        mill::log::debug("mill::XYZReader: done.\n");
    }

    bool             is_eof()    const noexcept override {return xyz_.eof();}
    std::size_t      current()   const noexcept override {return current_;}
    std::string_view file_name() const noexcept override {return file_name_;}

  private:

    std::size_t convert_to_ull(const std::string& str)
    {
        std::size_t N;
        try
        {
            N = std::stoull(line);
        }
        catch(...)
        {
            throw std::runtime_error("XYZReader::read_frame: "
                    "expected number, but got " + line);
        }
        mill::log::debug("mill::XYZReader: the next snapshot has ", N, "particles.\n");
        return N;
    }

  private:
    std::size_t   current_;
    std::string   file_name_;
    std::ifstream xyz_;
};

}// mill
#endif //COFFEE_MILL_XYZ_READER
