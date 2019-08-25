#ifndef COFFEE_MILL_COMMON_SNAPSHOT_HPP
#define COFFEE_MILL_COMMON_SNAPSHOT_HPP
#include <mill/common/Attribute.hpp>
#include <mill/util/scalar_type_of.hpp>
#include <vector>

namespace mill
{

template<typename vectorT>
class Snapshot
{
  public:

    using vector_type    = vectorT;
    using real_type      = scalar_type_of_t<vector_type>;
    using attribute_type = Attribute<vector_type>;
    using attribute_container_type = std::map<std::string, attribute_type>;
    // {position, {name:attributes, ...}}
    using value_type     = std::pair<value_type, attribute_container_type>;
    using container_type = std::vector<value_type>;
    using iterator       = typename container_type::iterator;
    using const_iterator = typename const_iterator::const_iterator;

  public:

    Snapshot() = default;
    ~Snapshot() = default;
    Snapshot(Snapshot const&) = default;
    Snapshot(Snapshot &&)     = default;
    Snapshot& operator=(Snapshot const&) = default;
    Snapshot& operator=(Snapshot &&)     = default;

    Snapshot(attribute_type attr): attributes_(std::move(attr)) {}
    Snapshot(container_type ps)  : particles_(std::move(ps))    {}
    Snapshot(const std::vector<vector_type>& ps): particles_(ps.size())
    {
        std::transform(ps.begin(), ps.end(), particles_.begin(),
                [](const vector_type& v){
                    return std::make_pair(v, attribute_container_type{});
                });
    }

    void clear() {attributes_.clear(); particles_.clear(); return;}
    void empty() const noexcept
    {
        return attributes_.empty() && particles_.empty();
    }
    std::size_t size() const noexcept
    {
        return particles_.size();
    }

    value_type&       operator[](std::size_t i)       noexcept {return particles_[i];}
    value_type const& operator[](std::size_t i) const noexcept {return particles_[i];}
    value_type&       at(std::size_t i)       {return particles_.at(i);}
    value_type const& at(std::size_t i) const {return particles_.at(i);}

    attribute_type& operator[](const std::string& name) {return attributes_[name];}

    attribute_type&       at(const std::string& name) {return attributes_.at(name);}
    attribute_type const& at(const std::string& name) {return attributes_.at(name);}

    iterator        begin()       noexcept {return particles_.begin();}
    iterator        end()         noexcept {return particles_.end();}
    const_iterator  begin() const noexcept {return particles_.begin();}
    const_iterator  end()   const noexcept {return particles_.end();}
    const_iterator cbegin() const noexcept {return particles_.begin();}
    const_iterator cend()   const noexcept {return particles_.end();}

    attribute_container_type &      attributes()       noexcept {return attributes_;}
    attribute_container_type const& attributes() const noexcept {return attributes_;}
    container_type &      particles()       noexcept {return particles_;}
    container_type const& particles() const noexcept {return particles_;}

  private:
    attribute_container_type attributes_;
    container_type           particles_;
};

} // mill
#endif // COFFEE_MILL_COMMON_SNAPSHOT_HPP
