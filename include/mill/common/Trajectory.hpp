#ifndef COFFEE_MILL_COMMON_TRAJECTORY_HPP
#define COFFEE_MILL_COMMON_TRAJECTORY_HPP
#include <mill/common/Snapshot.hpp>

namespace mill
{

class Trajectory
{
  public:
    using real_type      = double;
    using vector_type    = Vector<real_type, 3>;
    using attribute_type = Attribute;
    using attribute_container_type = std::map<std::string, attribute_type>;
    using snapshot_type  = Snapshot;
    using value_type     = snapshot_type;
    using container_type = std::vector<value_type>;
    using iterator       = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;

  public:

    Trajectory() = default;
    ~Trajectory() = default;
    Trajectory(Trajectory const&) = default;
    Trajectory(Trajectory &&)     = default;
    Trajectory& operator=(Trajectory const&) = default;
    Trajectory& operator=(Trajectory &&)     = default;

    Trajectory(std::size_t N): snapshots_(N) {}
    Trajectory(attribute_container_type attr): attributes_(std::move(attr)) {}
    Trajectory(container_type ps)  : snapshots_(std::move(ps))    {}
    Trajectory(const std::vector<snapshot_type>& ss): snapshots_(ss.size())
    {
        std::copy(ss.begin(), ss.end(), snapshots_.begin());
    }

    void clear() {attributes_.clear(); snapshots_.clear(); return;}
    bool empty() const noexcept
    {
        return attributes_.empty() && snapshots_.empty();
    }
    std::size_t size() const noexcept
    {
        return snapshots_.size();
    }

    snapshot_type&       operator[](std::size_t i)       noexcept {return snapshots_[i];}
    snapshot_type const& operator[](std::size_t i) const noexcept {return snapshots_[i];}
    snapshot_type&       at(std::size_t i)       {return snapshots_.at(i);}
    snapshot_type const& at(std::size_t i) const {return snapshots_.at(i);}

    snapshot_type&       front()       noexcept {return snapshots_.front();}
    snapshot_type const& front() const noexcept {return snapshots_.front();}
    snapshot_type&       back()        noexcept {return snapshots_.back();}
    snapshot_type const& back()  const noexcept {return snapshots_.back();}

    std::optional<snapshot_type> try_at(const std::size_t i) const noexcept
    {
        if(this->snapshots_.size() <= i)
        {
            return std::nullopt;
        }
        return this->snapshots_.at(i);
    }

    std::optional<attribute_type::boolean_type >
    try_boolean (const std::string& name) const noexcept
    {
        if(auto attr = this->try_at(name))
        {
            return attr->try_boolean();
        }
        return std::nullopt;
    }
    std::optional<attribute_type::integer_type >
    try_integer (const std::string& name) const noexcept
    {
        if(auto attr = this->try_at(name))
        {
            return attr->try_integer();
        }
        return std::nullopt;
    }
    std::optional<attribute_type::floating_type>
    try_floating(const std::string& name) const noexcept
    {
        if(auto attr = this->try_at(name))
        {
            return attr->try_floating();
        }
        return std::nullopt;
    }
    std::optional<attribute_type::string_type>
    try_string(const std::string& name) const noexcept
    {
        if(auto attr = this->try_at(name))
        {
            return attr->try_string();
        }
        return std::nullopt;
    }
    std::optional<attribute_type::vector_type>
    try_vector(const std::string& name) const noexcept
    {
        if(auto attr = this->try_at(name))
        {
            return attr->try_vector();
        }
        return std::nullopt;
    }
    std::optional<attribute_type::array_type>
    try_array(const std::string& name) const noexcept
    {
        if(auto attr = this->try_at(name))
        {
            return attr->try_array();
        }
        return std::nullopt;
    }

    attribute_type& operator[](const std::string& name) {return attributes_[name];}

    attribute_type&       at(const std::string& name)       {return attributes_.at(name);}
    attribute_type const& at(const std::string& name) const {return attributes_.at(name);}

    std::optional<attribute_type> try_at(const std::string& name) const noexcept
    {
        if(this->attributes_.count(name) != 0)
        {
            return this->attributes_.at(name);
        }
        return std::nullopt;
    }

    iterator        begin()       noexcept {return snapshots_.begin();}
    iterator        end()         noexcept {return snapshots_.end();}
    const_iterator  begin() const noexcept {return snapshots_.begin();}
    const_iterator  end()   const noexcept {return snapshots_.end();}
    const_iterator cbegin() const noexcept {return snapshots_.begin();}
    const_iterator cend()   const noexcept {return snapshots_.end();}

    attribute_container_type &      attributes()       noexcept {return attributes_;}
    attribute_container_type const& attributes() const noexcept {return attributes_;}
    container_type &      snapshots()       noexcept {return snapshots_;}
    container_type const& snapshots() const noexcept {return snapshots_;}

    void merge_attributes(Trajectory& other)
    {
        attributes_.merge(other.attributes_);
        if(this->size() == other.size())
        {
            for(std::size_t i=0; i<this->size(); ++i)
            {
                this->at(i).merge_attributes(other.at(i));
            }
        }
        return;
    }

  private:
    attribute_container_type attributes_;
    container_type           snapshots_;
};

} // mill
#endif// COFFEE_MILL_COMMON_TRAJECTORY_HPP
