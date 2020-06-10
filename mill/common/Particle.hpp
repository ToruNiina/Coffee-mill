#ifndef COFFEE_MILL_COMMON_PARTICLE_HPP
#define COFFEE_MILL_COMMON_PARTICLE_HPP
#include <mill/common/Attribute.hpp>
#include <vector>
#include <map>
#include <memory>

namespace mill
{

class Particle
{
  public:
    using real_type                = double;
    using vector_type              = Vector<real_type, 3>;
    using attribute_type           = Attribute;
    using attribute_container_type = std::map<std::string, attribute_type>;

  public:

    Particle() = default;
    ~Particle() = default;
    Particle(Particle const&) = default;
    Particle(Particle &&)     = default;
    Particle& operator=(Particle const&) = default;
    Particle& operator=(Particle &&)     = default;

    explicit Particle(vector_type pos) noexcept: position_(pos) {}
    explicit Particle(attribute_container_type attr) noexcept
        : attributes_(std::move(attr))
    {}
    Particle(vector_type pos, attribute_container_type attr) noexcept
        : position_(pos), attributes_(std::move(attr))
    {}

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

    attribute_container_type &      attributes()       noexcept {return attributes_;}
    attribute_container_type const& attributes() const noexcept {return attributes_;}
    vector_type &      position()       noexcept {return position_;}
    vector_type const& position() const noexcept {return position_;}

  private:
    vector_type              position_;
    attribute_container_type attributes_;
};

} // mill
#endif // COFFEE_MILL_COMMON_SNAPSHOT_HPP
