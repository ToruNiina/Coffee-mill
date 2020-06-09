#ifndef COFFEE_MILL_TRAJ_HPP
#define COFFEE_MILL_TRAJ_HPP

#include "dcd/DCDReader.hpp"
#include "dcd/DCDWriter.hpp"

#include "trr/TRRReader.hpp"
#include "trr/TRRWriter.hpp"

#include "xyz/XYZReader.hpp"
#include "xyz/XYZWriter.hpp"

namespace mill
{

class DeferedReader
{
  public:
    using trajectory_type = Trajectory;
    using snapshot_type   = Snapshot;
    using particle_type   = Particle;
    using vector_type     = Particle::vector_type;
    using attribute_container_type = trajectory_type::attribute_container_type;

  public:

    explicit DeferedReader(std::unique_ptr<DeferedReaderBase> reader)
        : reader_(std::move(reader))
    {}
    ~DeferedReader() override = default;
    DeferedReader(const DeferedReader&)            = delete;
    DeferedReader& operator=(const DeferedReader&) = delete;
    DeferedReader(DeferedReader&&)                 = default;
    DeferedReader& operator=(DeferedReader&&)      = default;

    attribute_container_type read_header()    {return reader_->read_header();};
    trajectory_type          read()           {return reader_->read();};
    std::optional<snapshot_type> read_frame() {return reader_->read_frame();};
    std::optional<snapshot_type> read_frame(const std::size_t idx)
    {
        return reader_->read_frame(idx);
    }

    ReaderIterator         begin() {return reader_->begin();}
    ReaderIteratorSentinel end()   {return reader_->begin();}

    void             rewind()                   {return reader_->rewind()   ;}
    bool             is_eof()    const noexcept {return reader_->is_eof()   ;}
    std::size_t      current()   const noexcept {return reader_->current()  ;}
    std::string_view file_name() const noexcept {return reader_->file_name();}

  private:

    std::unique_ptr<DeferedReaderBase> reader_;
};

inline DeferedReader read(std::string_view filename)
{
    const auto dot = filename.rfind('.');
    const auto ext = filename.substr(dot, filename.size() - dot);

    if(ext == ".dcd")
    {
        return DeferedReader(std::make_unique<DCDReader>(filename));
    }
    else if(ext == ".trr")
    {
        return DeferedReader(std::make_unique<TRRReader>(filename));
    }
    else if(ext == ".xyz")
    {
        return DeferedReader(std::make_unique<XYZReader>(filename));
    }
    else
    {
        log::error("mill::read(file): unknown file extension \"", ext, "\".");
        log::error("mill::read(file): supported formats are \".dcd\", "
                   "\".trr\", and \".xyz\".");
        throw std::runtime_error("unknown file.");
    }
}

} // mill
#endif// COFFEE_MILL_TRAJ_HPP
