#ifndef COFFEE_MILL_COMMON_DEFERED_READER_HPP
#define COFFEE_MILL_COMMON_DEFERED_READER_HPP
#include <mill/common/Trajectory.hpp>
#include <string_view>
#include <optional>
#include <iterator>
#include <cstddef>

namespace mill
{

class ReaderIterator;
class ReaderIteratorSentinel;

class DeferedReaderBase
{
  public:
    using trajectory_type = Trajectory;
    using snapshot_type   = Snapshot;
    using particle_type   = Particle;
    using vector_type     = Particle::vector_type;
    using attribute_container_type = trajectory_type::attribute_container_type;

  public:

    virtual ~DeferedReaderBase() = default;

    virtual attribute_container_type read_header()                         = 0;
    virtual trajectory_type          read()                                = 0;
    virtual std::optional<snapshot_type> read_frame()                      = 0;
    virtual std::optional<snapshot_type> read_frame(const std::size_t idx) = 0;

    ReaderIterator         begin(); // defined later
    ReaderIteratorSentinel end();   // ditto

    virtual void             rewind()    = 0;
    virtual bool             is_eof()    const noexcept = 0;
    virtual std::size_t      current()   const noexcept = 0;
    virtual std::string_view file_name() const noexcept = 0;
};

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
        : current_(reader.read_frame()), reader_(std::addressof(reader))
    {}
    ~ReaderIterator() = default;

    ReaderIterator(const ReaderIterator&) = default;
    ReaderIterator(ReaderIterator&&)      = default;
    ReaderIterator& operator=(const ReaderIterator&) = default;
    ReaderIterator& operator=(ReaderIterator&&)      = default;

    value_type& operator*()
    {
        if(!current_)
        {
            using namespace std::literals::string_literals;
            throw std::out_of_range("ReaderIterator: "s +
                    std::string(reader_->file_name()) +
                    " does not have any more snapshot"s);
        }
        return *current_;
    }
    value_type* operator->()
    {
        if(!current_)
        {
            using namespace std::literals::string_literals;
            throw std::out_of_range("ReaderIterator: "s +
                    std::string(reader_->file_name()) +
                    " does not have any more snapshot"s);
        }
        return std::addressof(*current_);
    }

    ReaderIterator& operator++()
    {
        current_ = reader_->read_frame();
        return *this;
    }
    ReaderIterator  operator++(int)
    {
        const auto self(*this);
        current_ = reader_->read_frame();
        return self;
    }

    bool             has_value() const noexcept {return current_.has_value();}
    bool             is_eof()    const noexcept {return reader_->is_eof();}
    std::size_t      current()   const noexcept {return reader_->current();}
    std::string_view file_name() const noexcept {return reader_->file_name();}

  private:
    std::optional<value_type> current_;
    DeferedReaderBase*        reader_;
};

inline bool operator==(const ReaderIterator& lhs, const ReaderIterator& rhs)
{
    return std::make_tuple(lhs.is_eof(), lhs.current(), lhs.file_name()) ==
           std::make_tuple(rhs.is_eof(), rhs.current(), rhs.file_name());
}
inline bool operator!=(const ReaderIterator& lhs, const ReaderIterator& rhs)
{
    return !(lhs == rhs);
}

class ReaderIteratorSentinel
{
  public:
    using iterator_category = std::input_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = Snapshot;
    using reference         = value_type&;
    using pointer           = value_type*;

  public:
    explicit ReaderIteratorSentinel(DeferedReaderBase& reader)
        : file_name_(reader.file_name())
    {}

    bool             is_eof()    const noexcept {return true;}
    std::string_view file_name() const noexcept {return file_name_;}

  private:
    std::string_view file_name_;
};

inline bool operator==(
        const ReaderIteratorSentinel& lhs, const ReaderIteratorSentinel& rhs)
{
    return lhs.file_name() == rhs.file_name();
}
inline bool operator!=(
        const ReaderIteratorSentinel& lhs, const ReaderIteratorSentinel& rhs)
{
    return !(lhs == rhs);
}

inline bool operator==(const ReaderIterator& lhs, const ReaderIteratorSentinel& rhs)
{
    return (not lhs.has_value()) && (lhs.file_name() == rhs.file_name());
}
inline bool operator==(const ReaderIteratorSentinel& lhs, const ReaderIterator& rhs)
{
    return (not rhs.has_value()) && (lhs.file_name() == rhs.file_name());
}
inline bool operator!=(const ReaderIterator& lhs, const ReaderIteratorSentinel& rhs)
{
    return !(lhs == rhs);
}
inline bool operator!=(const ReaderIteratorSentinel& lhs, const ReaderIterator& rhs)
{
    return !(lhs == rhs);
}

inline ReaderIterator DeferedReaderBase::begin()
{
    return ReaderIterator(*this);
}
inline ReaderIteratorSentinel DeferedReaderBase::end()
{
    return ReaderIteratorSentinel(*this);
}

} // mill
#endif// COFFEE_MILL_COMMON_DEFERED_READER_HPP
