#ifndef COFFEE_MILL_COMMON_DEFERED_READER_HPP
#define COFFEE_MILL_COMMON_DEFERED_READER_HPP
#include <mill/common/Trajectory.hpp>
#include <string_view>
#include <optional>
#include <iterator>
#include <cstddef>

namespace mill
{

class DeferedReaderBase;

class ReaderIterator
{
  public:
    using iterator_category = std::input_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Snapshot;
    using reference         = value_type&;
    using pointer           = value_type*;

  public:
    explicit ReaderIterator(DeferedReaderBase& reader)
        : current_(std::nullopt), reader_(std::addressof(reader))
    {}

    const value_type& operator* () const
    {
        if(!current_) {current_ = reader_->read_frame();}
        return *current_;
    }
    const value_type* operator->() const
    {
        if(!current_) {current_ = reader_->read_frame();}
        return std::addressof(*current_);
    }

    ReaderIterator& operator++()
    {
        current_ = reader_->read_frame();
        return *this;
    }
    ReaderIterator  operator++(int)
    {
        current_ = reader_->read_frame();
        return *this;
    }

    bool             is_eof()    const noexcept {return reader_->is_eof();}
    std::size_t      current()   const noexcept {return reader_->current();}
    std::string_view file_name() const noexcept {return reader_->file_name();}

  private:
    std::optional<value_type> current_;
    DeferedReaderBase*        reader_;
};

inline bool operator==(const ReaderIterator& lhs, const ReaderIterator& rhs)
{
    return std::make_tuple(lhs->is_eof(), lhs->current(), lhs->file_name()) ==
           std::make_tuple(lhs->is_eof(), lhs->current(), lhs->file_name());
}
inline bool operator!=(const ReaderIterator& lhs, const ReaderIterator& rhs)
{
    return !(lhs == rhs);
}

class DeferedReaderBase
{
  public:
    using trajectory_type = Trajectory;
    using snapshot_type   = Snapshot;
    using attribute_container_type = trajectory_type::attribute_container_type;

  public:

    attribute_container_type read_header()                     = 0;
    trajectory_type          read()                            = 0;
    snapshot_type            read_frame()                      = 0;
    snapshot_type            read_frame(const std::size_t idx) = 0;

    bool             is_eof()    const noexcept = 0;
    std::size_t      current()   const noexcept = 0;
    std::string_view file_name() const noexcept = 0;
};

} // mill
#endif// COFFEE_MILL_COMMON_DEFERED_READER_HPP
