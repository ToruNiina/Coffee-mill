#ifndef COFFEE_MILL_UTIL_RANGE_HPP
#define COFFEE_MILL_UTIL_RANGE_HPP
#include <iterator>

namespace mill
{

// For the simplicity sake, it is defined as an LegacyInputIterator.
template<typename integerT, typename iteratorT>
struct enumerate_iterator
{
    using integer_type      = integerT;
    using iterator          = iteratorT;
    using traits_type       = std::iterator_traits<iterator>;
    using value_type        = std::pair<integer_type, typename traits_type::value_type>;
    using reference         = std::pair<integer_type, typename traits_type::value_type>;
    using pointer           = std::pair<integer_type, typename traits_type::value_type>;
    using difference_type   = typename traits_type::difference_type;
    using iterator_category = std::input_iterator_tag;

    explicit enumerate_iterator(iterator iter)
        : idx_(0), iter_(iter)
    {}
    enumerate_iterator(integer_type idx, iterator iter)
        : idx_(idx), iter_(iter)
    {}
    ~enumerate_iterator() = default;

    enumerate_iterator(const enumerate_iterator&) = default;
    enumerate_iterator(enumerate_iterator&&)      = default;
    enumerate_iterator& operator=(const enumerate_iterator&) = default;
    enumerate_iterator& operator=(enumerate_iterator&&)      = default;

    reference operator*() const noexcept
    {
        return reference(idx_, *iter_);
    }
    pointer operator->() const noexcept
    {
        return pointer(idx_, *iter_);
    }

    enumerate_iterator& operator++()
    {
        this->idx_++;
        this->iter_++;
        return *this;
    }
    enumerate_iterator  operator++(int)
    {
        enumerate_iterator tmp(*this);
        this->operator++();
        return tmp;
    }

    bool operator==(const enumerate_iterator& other)
    {
        return this->idx_ == other.idx_ && this->iter_ == other.iter_;
    }
    bool operator!=(const enumerate_iterator& other)
    {
        return !(*this == other);
    }

  private:

    integer_type idx_;
    iterator     iter_;
};

template<typename integerT, typename iteratorT>
struct enumerable
{
    using iterator = enumerate_iterator<integerT, iteratorT>;

    enumerable(iterator first, iterator last)
        : first_(first), last_(last)
    {}
    ~enumerable() = default;

    enumerable(const enumerable&) = default;
    enumerable(enumerable&&)      = default;
    enumerable& operator=(const enumerable&) = default;
    enumerable& operator=(enumerable&&)      = default;

    iterator begin() const noexcept {return first_;}
    iterator end()   const noexcept {return last_;}

  private:

    iterator first_, last_;
};

template<typename integerT = std::size_t, typename containerT>
enumerable<integerT, typename containerT::const_iterator>
enumerate(const containerT& container)
{
    using base_iterator = typename containerT::const_iterator;
    using iterator = enumerate_iterator<integerT, base_iterator>;

    return enumerable(iterator(0,                    std::begin(container)),
                      iterator(std::size(container), std::end  (container)));
}

} // mill
#endif// COFFEE_MILL_UTIL_RANGE_HPP
