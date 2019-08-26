#ifndef COFFEE_MILL_COMMON_TRAJECTORY_HPP
#define COFFEE_MILL_COMMON_TRAJECTORY_HPP
#include <mill/common/Snapshot.hpp>

namespace mill
{

template<typename vectorT>
class Trajectory
{
  public:
    using vector_type    = vectorT;
    using real_type      = scalar_type_of_t<vector_type>;
    using attribute_type = Attribute<vector_type>;
    using attribute_container_type = std::map<std::string, attribute_type>;
    using snapshot_type  = Snapshot<vector_type>;
    using value_type     = std::pair<snapshot_type, attribute_container_type>;
    using container_type = std::vector<value_type>;
    using iterator       = typename container_type::iterator;
    using const_iterator = typename const_iterator::const_iterator;

  public:

    Trajectory() = default;
    ~Trajectory() = default;
    Trajectory(Trajectory const&) = default;
    Trajectory(Trajectory &&)     = default;
    Trajectory& operator=(Trajectory const&) = default;
    Trajectory& operator=(Trajectory &&)     = default;

    Trajectory(attribute_type attr): attributes_(std::move(attr)) {}
    Trajectory(container_type ps)  : snapshots_(std::move(ps))    {}
    Trajectory(const std::vector<snapshot_type>& ss): snapshots_(ss.size())
    {
        std::transform(ps.begin(), ps.end(), snapshots_.begin(),
                [](const vector_type& v){
                    return std::make_pair(v, attribute_container_type{});
                });
    }

    void clear() {attributes_.clear(); snapshots_.clear(); return;}
    void empty() const noexcept
    {
        return attributes_.empty() && snapshots_.empty();
    }
    std::size_t size() const noexcept
    {
        return snapshots_.size();
    }

    value_type&       operator[](std::size_t i)       noexcept {return snapshots_[i];}
    value_type const& operator[](std::size_t i) const noexcept {return snapshots_[i];}
    value_type&       at(std::size_t i)       {return snapshots_.at(i);}
    value_type const& at(std::size_t i) const {return snapshots_.at(i);}

    attribute_type& operator[](const std::string& name) {return attributes_[name];}

    attribute_type&       at(const std::string& name) {return attributes_.at(name);}
    attribute_type const& at(const std::string& name) {return attributes_.at(name);}

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

  private:
    attribute_container_type attributes_;
    container_type           snapshots_;
};

} // mill
#endif// COFFEE_MILL_COMMON_TRAJECTORY_HPP
