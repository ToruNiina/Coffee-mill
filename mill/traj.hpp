#ifndef COFFEE_MILL_TRAJ_HPP
#define COFFEE_MILL_TRAJ_HPP

#include "dcd/DCDReader.hpp"
#include "dcd/DCDWriter.hpp"

#include "trr/TRRReader.hpp"
#include "trr/TRRWriter.hpp"

#include "xyz/XYZReader.hpp"
#include "xyz/XYZWriter.hpp"

#include "util/file_extension.hpp"

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
    ~DeferedReader() = default;
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
    ReaderIteratorSentinel end()   {return reader_->end();}

    void             rewind()                   {return reader_->rewind()   ;}
    bool             is_eof()    const noexcept {return reader_->is_eof()   ;}
    std::size_t      current()   const noexcept {return reader_->current()  ;}
    std::string_view file_name() const noexcept {return reader_->file_name();}

  private:

    std::unique_ptr<DeferedReaderBase> reader_;
};

inline DeferedReader read(std::string_view filename)
{
    const auto ext = extension_of(filename);
    if(ext == ".dcd")
    {
        return DeferedReader(std::make_unique<DCDReader>(std::string(filename)));
    }
    else if(ext == ".trr")
    {
        return DeferedReader(std::make_unique<TRRReader>(std::string(filename)));
    }
    else if(ext == ".xyz")
    {
        return DeferedReader(std::make_unique<XYZReader>(std::string(filename)));
    }
    else
    {
        log::error("mill::read(file): unknown file extension \"", ext, "\".");
        log::error("mill::read(file): supported formats are \".dcd\", "
                   "\".trr\", and \".xyz\".");
        throw std::runtime_error("unknown file.");
    }
}

class TrajWriter
{
  public:
    using trajectory_type          = Trajectory;
    using snapshot_type            = Snapshot;
    using particle_type            = Particle;
    using attribute_container_type = Trajectory::attribute_container_type;
    using boundary_type            = Snapshot::boundary_type;
    using vector_type              = Particle::vector_type;

  public:

    explicit TrajWriter(std::unique_ptr<WriterBase> writer)
        : writer_(std::move(writer))
    {}
    ~TrajWriter() = default;
    TrajWriter(const TrajWriter&)            = delete;
    TrajWriter& operator=(const TrajWriter&) = delete;
    TrajWriter(TrajWriter&&)                 = default;
    TrajWriter& operator=(TrajWriter&&)      = default;

    void write_header(const attribute_container_type& header)
    {
        return writer_->write_header(header);
    }
    void write(const trajectory_type& traj)
    {
        return writer_->write(traj);
    }
    void write_frame(const snapshot_type& frame)
    {
        return writer_->write_frame(frame);
    }

    std::size_t      size()      const noexcept {return writer_->size();}
    std::string_view file_name() const noexcept {return writer_->file_name();}

  private:

    std::unique_ptr<WriterBase> writer_;
};

inline TrajWriter writer(std::string_view filename)
{
    const auto ext = extension_of(filename);

    if(ext == ".dcd")
    {
        return TrajWriter(std::make_unique<DCDWriter>(std::string(filename)));
    }
    else if(ext == ".trr")
    {
        return TrajWriter(std::make_unique<TRRWriter>(std::string(filename)));
    }
    else if(ext == ".xyz")
    {
        return TrajWriter(std::make_unique<XYZWriter>(std::string(filename)));
    }
    else
    {
        log::error("mill::writer(file): unknown file extension \"", ext, "\".");
        log::error("mill::writer(file): supported formats are \".dcd\", "
                   "\".trr\", and \".xyz\".");
        throw std::runtime_error("unknown file.");
    }
}


} // mill
#endif// COFFEE_MILL_TRAJ_HPP
