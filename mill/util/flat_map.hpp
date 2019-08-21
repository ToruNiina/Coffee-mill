#ifndef COFFEE_MILL_UTIL_FLAT_MAP_HPP
#define COFFEE_MILL_UTIL_FLAT_MAP_HPP
#include <utility>
#include <initializer_list>
#include <stdexcept>
#include <vector>
#include <algorithm>

namespace mill
{

template<typename Key, typename Value, typename Comp = std::less<Key>>
class flat_map
{
  public:
    using key_type               = Key;
    using mapped_type            = Value;
    using value_type             = std::pair<key_type, mapped_type>;
    using container_type         = std::vector<value_type>;
    using allocator_type         = typename container_type::allocator_type;
    using iterator               = typename container_type::iterator;
    using const_iterator         = typename container_type::const_iterator;
    using reference              = typename container_type::reference;
    using const_reference        = typename container_type::const_reference;
    using pointer                = typename container_type::pointer;
    using const_pointer          = typename container_type::const_pointer;
    using size_type              = typename container_type::size_type;
    using difference_type        = typename container_type::difference_type;
    using reverse_iterator       = typename container_type::reverse_iterator;
    using const_reverse_iterator = typename container_type::const_reverse_iterator;
    using key_compare            = Comp;

    struct value_compare
    {
        value_compare() = default;
        ~value_compare() = default;

        value_compare(key_compare kc): comp_(kc) {}

        bool operator()(const value_type& lhs, const value_type& rhs) const
        {
            return comp_(lhs.first, rhs.first);
        }

        key_compare comp_;
    };

    struct key_value_compare
    {
        key_value_compare() = default;
        ~key_value_compare() = default;

        key_value_compare(key_compare   kc): comp_(kc) {}
        key_value_compare(value_compare vc): comp_(vc.comp_) {}

        bool operator()(const key_type& lhs, const value_type& rhs) const
        {
            return comp_(lhs, rhs.first);
        }
        bool operator()(const value_type& lhs, const key_type& rhs) const
        {
            return comp_(lhs.first, rhs);
        }

        key_compare comp_;
    };

  public:

    flat_map() = default;
    ~flat_map() = default;
    flat_map(flat_map const&) = default;
    flat_map(flat_map &&)     = default;
    flat_map& operator=(flat_map const&) = default;
    flat_map& operator=(flat_map &&)     = default;

    flat_map(const key_compare& kc) : comp_(kc) {}

    template<typename InputIterator>
    flat_map(InputIterator first, InputIterator last,
             const key_compare& kc = key_compare())
        : comp_(kc), container_(first, last)
    {
        std::sort(container_.begin(), container_.end(), comp_);
    }
    flat_map(std::initializer_list<value_type> init,
             const key_compare& kc = key_compare())
        : comp_(kc), container_(std::move(init))
    {
        std::sort(container_.begin(), container_.end(), comp_);
    }
    flat_map& operator=(std::initializer_list<value_type> init)
    {
        this->container_ = std::move(init);
        std::sort(container_.begin(), container_.end(), comp_);
        return *this;
    }

    mapped_type& operator[](const key_type& key)
    {
        const key_value_compare cmp(comp_);
        const auto found = std::lower_bound(this->begin(), this->end(), key, cmp);
        if(found != this->end() && cmp(key, *found))
        {
            return found->second;
        }
        return this->container_.emplace(found, key, mapped_type{})->second;
    }
    mapped_type&       at(const key_type& key)
    {
        const auto found = this->find(key);
        if(found == this->end())
        {
            throw std::out_of_range("mill::flat_map::at: key not found");
        }
        return found->second;
    }
    mapped_type const& at(const key_type& key) const
    {
        const auto found = this->find(key);
        if(found == this->end())
        {
            throw std::out_of_range("mill::flat_map::at: key not found");
        }
        return found->second;
    }

    std::pair<iterator, bool> insert(const value_type& v)
    {
        const key_value_compare cmp(comp_);
        const auto found = std::lower_bound(this->begin(), this->end(), v.first, cmp);
        if(found != this->end() && cmp(v.first, *found)) // already exists.
        {
            return std::make_pair(found, false);
        }
        const auto inserted = this->container_.emplace(found, v);
        return std::make_pair(inserted, true);
    }
    std::pair<iterator, bool> insert(value_type&& v)
    {
        const key_value_compare cmp(comp_);
        const auto found = std::lower_bound(this->begin(), this->end(), v.first, cmp);
        if(found != this->end() && cmp(v.first, *found)) // already exists.
        {
            return std::make_pair(found, false);
        }
        const auto inserted = this->container_.emplace(found, std::move(v));
        return std::make_pair(inserted, true);
    }
    template<typename InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        for(; first != last; ++first)
        {
            this->insert(*first);
        }
        return;
    }
    void insert(std::initializer_list<value_type> init)
    {
        for(const auto& x : init)
        {
            this->insert(x);
        }
        return;
    }
    template<typename ... Ts>
    void emplace(Ts&& ... args)
    {
        this->insert(value_type(std::forward<Ts>(args)...));
        return;
    }

    size_type erase(const key_type& k)
    {
        const auto found = this->find(k);
        if(found == this->end())
        {
            return 0;
        }
        this->erase(found);
        return 1;
    }
    void erase(const_iterator pos)
    {
        this->container_.erase(pos);
    }
    void erase(const_iterator first, const_iterator last)
    {
        this->container_.erase(first, last);
    }

    void swap(flat_map& other)
    {
        using std::swap;
        swap(this->comp_,      other.comp_);
        swap(this->container_, other.container_);
        return;
    }

    void clear() {container_.clear();}
    bool empty()         const noexcept {return container_.empty();}
    size_type size()     const noexcept {return container_.size();}
    size_type max_size() const noexcept {return container_.max_size();}

    iterator       begin()        noexcept {return container_.begin();}
    iterator       end()          noexcept {return container_.end();}
    const_iterator begin()  const noexcept {return container_.begin();}
    const_iterator end()    const noexcept {return container_.end();}
    const_iterator cbegin() const noexcept {return container_.cbegin();}
    const_iterator cend()   const noexcept {return container_.cend();}

    reverse_iterator       rbegin()        noexcept {return container_.rbegin();}
    reverse_iterator       rend()          noexcept {return container_.rend();}
    const_reverse_iterator rbegin()  const noexcept {return container_.rbegin();}
    const_reverse_iterator rend()    const noexcept {return container_.rend();}
    const_reverse_iterator crbegin() const noexcept {return container_.crbegin();}
    const_reverse_iterator crend()   const noexcept {return container_.crend();}

    size_type count(const key_type& k) const
    {
        return (this->find(k) == this->end()) ? 0 : 1;
    }
    bool contains(const key_type& k) const
    {
        return (this->find(k) != this->end());
    }

    iterator find(const key_type& key)
    {
        const key_value_compare cmp(comp_);
        const auto found = std::lower_bound(this->begin(), this->end(), key, cmp);
        // assuming `not (a < b) and not (b < a)` is equivalent to `a == b`.
        // lower_bound returns the first element that is not `*found < k`.
        // if `not k < *found`, k == found. otherwise, there are no element.
        if(found != this->end() && cmp(key, *found))
        {
            return this->end();
        }
        return found;
    }
    const_iterator find(const key_type& key) const
    {
        const key_value_compare cmp(comp_);
        const auto found = std::lower_bound(this->begin(), this->end(), key, cmp);
        if(found != this->end() && cmp(key, *found))
        {
            return this->end();
        }
        return found;
    }

    iterator lower_bound(const key_type& k)
    {
        return std::lower_bound(this->begin(), this->end(), k,
                                key_value_compare(comp_));
    }
    const_iterator lower_bound(const key_type& k) const
    {
        return std::lower_bound(this->begin(), this->end(), k,
                                key_value_compare(comp_));
    }

    iterator upper_bound(const key_type& k)
    {
        return std::upper_bound(this->begin(), this->end(), k,
                                key_value_compare(comp_));
    }
    const_iterator upper_bound(const key_type& k) const
    {
        return std::upper_bound(this->begin(), this->end(), k,
                                key_value_compare(comp_));
    }

  private:
    value_compare  comp_;
    container_type container_;
};

} // mill
#endif// COFFEE_MILL_UTIL_FLAT_MAP_HPP
