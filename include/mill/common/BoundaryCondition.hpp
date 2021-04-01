#ifndef COFFEE_MILL_COMMON_BOUNDARY_CONDITION_HPP
#define COFFEE_MILL_COMMON_BOUNDARY_CONDITION_HPP
#include <mill/math/Vector.hpp>
#include <limits>
#include <variant>

namespace mill
{

struct UnlimitedBoundary
{
  public:
    using real_type   = double;
    using vector_type = Vector<real_type, 3>;

  public:
    UnlimitedBoundary() = default;
    ~UnlimitedBoundary() = default;

    vector_type adjust_direction(vector_type dr) const noexcept {return dr;}
    vector_type adjust_position (vector_type r ) const noexcept {return r;}

    real_type volume() const noexcept {return std::numeric_limits<real_type>::infinity();}

    vector_type lower() const noexcept
    {
        return vector_type{
            -std::numeric_limits<real_type>::infinity(),
            -std::numeric_limits<real_type>::infinity(),
            -std::numeric_limits<real_type>::infinity()
        };
    }
    vector_type upper() const noexcept
    {
        return vector_type{
            std::numeric_limits<real_type>::infinity(),
            std::numeric_limits<real_type>::infinity(),
            std::numeric_limits<real_type>::infinity()
        };
    }
    vector_type width() const noexcept
    {
        return vector_type{
            std::numeric_limits<real_type>::infinity(),
            std::numeric_limits<real_type>::infinity(),
            std::numeric_limits<real_type>::infinity()
        };
    }
};

struct CuboidalPeriodicBoundary
{
  public:
    using real_type   = double;
    using vector_type = Vector<real_type, 3>;

  public:

    CuboidalPeriodicBoundary() noexcept
        : CuboidalPeriodicBoundary(vector_type(0, 0, 0), vector_type(0, 0, 0))
    {}

    CuboidalPeriodicBoundary(const vector_type& lw, const vector_type& up) noexcept
        : lower_(lw), upper_(up), width_(up - lw), halfw_((up - lw) * 0.5)
    {}
    ~CuboidalPeriodicBoundary() = default;

    vector_type adjust_direction(vector_type dr) const noexcept
    {
        while(dr[0] < -halfw_[0]) {dr[0] += width_[0];}
        while(dr[0] >= halfw_[0]) {dr[0] -= width_[0];}
        while(dr[1] < -halfw_[1]) {dr[1] += width_[1];}
        while(dr[1] >= halfw_[1]) {dr[1] -= width_[1];}
        while(dr[2] < -halfw_[2]) {dr[2] += width_[2];}
        while(dr[2] >= halfw_[2]) {dr[2] -= width_[2];}
        return dr;
    }

    vector_type adjust_position(vector_type pos) const noexcept
    {
        while(pos[0] <  lower_[0]) {pos[0] += width_[0];}
        while(pos[0] >= upper_[0]) {pos[0] -= width_[0];}
        while(pos[1] <  lower_[1]) {pos[1] += width_[1];}
        while(pos[1] >= upper_[1]) {pos[1] -= width_[1];}
        while(pos[2] <  lower_[2]) {pos[2] += width_[2];}
        while(pos[2] >= upper_[2]) {pos[2] -= width_[2];}
        return pos;
    }

    real_type volume() const noexcept
    {
        return width_[0] * width_[1] * width_[2];
    }

    vector_type const& lower() const noexcept {return lower_;}
    vector_type const& upper() const noexcept {return upper_;}
    vector_type const& width() const noexcept {return width_;}

    void set_boundary(const vector_type& lw, const vector_type& up) noexcept
    {
        this->lower_ = lw;
        this->upper_ = up;
        this->width_ = this->upper_ - this->lower_;
        this->halfw_ = this->width_ * 0.5;
        return;
    }

  private:

    vector_type lower_;
    vector_type upper_;
    vector_type width_;
    vector_type halfw_;
};

enum class BoundaryConditionKind : std::size_t
{
    Unlimited = 0,
    CuboidalPeriodic = 1,
};

inline std::ostream& operator<<(std::ostream& os, const BoundaryConditionKind kind)
{
    switch(kind)
    {
        case BoundaryConditionKind::Unlimited:
        {
            os << "Unlimited";
            break;
        }
        case BoundaryConditionKind::CuboidalPeriodic:
        {
            os << "Periodic";
            break;
        }
        default:
        {
            os << "Unknown";
            break;
        }
    }
    return os;
}

struct BoundaryCondition
{
  public:

    using real_type   = double;
    using vector_type = Vector<real_type, 3>;
    using storage_type = std::variant<
            UnlimitedBoundary,
            CuboidalPeriodicBoundary
        >;

  public:

    BoundaryCondition()  = default;
    ~BoundaryCondition() = default;

    explicit BoundaryCondition(UnlimitedBoundary b)
        : storage_(std::move(b))
    {}
    explicit BoundaryCondition(CuboidalPeriodicBoundary b)
        : storage_(std::move(b))
    {}

    vector_type adjust_direction(vector_type dr) const noexcept
    {
        return std::visit([dr](const auto& b) -> vector_type {
                    return b.adjust_direction(dr);
                }, storage_);
    }
    vector_type adjust_position (vector_type r ) const noexcept
    {
        return std::visit([r](const auto& b) -> vector_type {
                    return b.adjust_position(r);
                }, storage_);
    }
    real_type volume() const noexcept
    {
        return std::visit([](const auto& b) -> real_type {
                    return b.volume();
                }, storage_);
    }

    vector_type lower() const noexcept
    {
        return std::visit([](const auto& b) -> vector_type {
                return b.lower();
            }, storage_);
    }
    vector_type upper() const noexcept
    {
        return std::visit([](const auto& b) -> vector_type {
                return b.upper();
            }, storage_);
    }
    vector_type width() const noexcept
    {
        return std::visit([](const auto& b) -> vector_type {
                return b.width();
            }, storage_);
    }

    BoundaryConditionKind kind() const noexcept
    {
        return static_cast<BoundaryConditionKind>(storage_.index());
    }

    UnlimitedBoundary const&        as_unlimited() const {return std::get<UnlimitedBoundary>(storage_);}
    UnlimitedBoundary &             as_unlimited()       {return std::get<UnlimitedBoundary>(storage_);}
    CuboidalPeriodicBoundary const& as_periodic()  const {return std::get<CuboidalPeriodicBoundary>(storage_);}
    CuboidalPeriodicBoundary &      as_periodic()        {return std::get<CuboidalPeriodicBoundary>(storage_);}

  private:
    storage_type storage_;
};

}// mill
#endif /* COFFEE_MILL_COMMON_BOUNDARY_CONDITION_HPP */
