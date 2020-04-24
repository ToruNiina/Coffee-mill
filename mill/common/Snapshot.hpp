#ifndef COFFEE_MILL_COMMON_SNAPSHOT_HPP
#define COFFEE_MILL_COMMON_SNAPSHOT_HPP
#include <mill/common/Particle.hpp>
#include <mill/common/BoundaryCondition.hpp>
#include <mill/util/scalar_type_of.hpp>
#include <vector>
#include <memory>

namespace mill
{

class Snapshot
{
  public:
    using real_type      = double;
    using vector_type    = Vector<real_type, 3>;
    using attribute_type = Attribute;
    using attribute_container_type = std::map<std::string, attribute_type>;
    // {position, {name:attributes, ...}}
    using particle_type  = Particle;
    using value_type     = particle_type;
    using container_type = std::vector<value_type>;
    using iterator       = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;
    using boundary_type  = BoundaryCondition;

  public:

    Snapshot() = default;
    ~Snapshot() = default;
    Snapshot(Snapshot const&) = default;
    Snapshot(Snapshot &&)     = default;
    Snapshot& operator=(Snapshot const&) = default;
    Snapshot& operator=(Snapshot &&)     = default;

    Snapshot(std::size_t N): particles_(N) {}

    Snapshot(attribute_container_type attr): attributes_(std::move(attr)) {}
    Snapshot(container_type ps)  : particles_(std::move(ps))    {}
    Snapshot(const std::vector<vector_type>& ps): particles_(ps.size())
    {
        std::transform(ps.begin(), ps.end(), particles_.begin(),
                [](const vector_type& pos) -> particle_type {
                    return particle_type(pos, attribute_container_type{});
                });
    }

    void clear() {attributes_.clear(); particles_.clear(); return;}
    bool empty() const noexcept
    {
        return attributes_.empty() && particles_.empty();
    }
    std::size_t size() const noexcept
    {
        return particles_.size();
    }

    particle_type&       operator[](std::size_t i)       noexcept {return particles_[i];}
    particle_type const& operator[](std::size_t i) const noexcept {return particles_[i];}
    particle_type&       at(std::size_t i)       {return particles_.at(i);}
    particle_type const& at(std::size_t i) const {return particles_.at(i);}

    std::optional<particle_type> try_at(const std::size_t i) const noexcept
    {
        if(this->particles_.size() <= i)
        {
            return std::nullopt;
        }
        return this->particles_.at(i);
    }

    attribute_type& operator[](const std::string& name) {return attributes_[name];}

    attribute_type&       at(const std::string& name)       {return attributes_.at(name);}
    attribute_type const& at(const std::string& name) const {return attributes_.at(name);}

    std::optional<attribute_type> try_at(const std::string& name) const noexcept
    {
        if(attributes_.count(name) == 0)
        {
            return std::nullopt;
        }
        return attributes_.at(name);
    }

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
    boundary_type &      boundary()       noexcept {return boundary_;}
    boundary_type const& boundary() const noexcept {return boundary_;}

  private:
    attribute_container_type attributes_;
    container_type           particles_;
    boundary_type            boundary_;
};

} // mill
#endif // COFFEE_MILL_COMMON_SNAPSHOT_HPP
