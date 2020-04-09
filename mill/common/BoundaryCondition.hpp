#ifndef MJOLNIR_CORE_BOUNDARY_CONDITION_HPP
#define MJOLNIR_CORE_BOUNDARY_CONDITION_HPP
#include <mill/math/Vector.hpp>
#include <mill/util/scalar_type_of.hpp>
#include <limits>

namespace mjolnir
{

template<typename vectorT>
struct BoundaryCondition
{
  public:
    using vector_type = coordT;
    using real_type   = typename scalar_type_of<vector_type>::type;

  public:

    virtual ~BoundaryCondition() = default;

    vector_type adjust_direction(vector_type dr) const noexcept = 0;
    vector_type adjust_position (vector_type r ) const noexcept = 0;

    real_type volume() const noexcept = 0;
};

template<typename vectorT>
struct UnlimitedBoundary final : BoundaryCondition<vectorT>
{
  public:
    using base_type   = BoundaryCondition<vectorT>;
    using vector_type = typename base_type::vector_type;
    using real_type   = typename base_type::real_type;

  public:
    UnlimitedBoundary() = default;
    ~UnlimitedBoundary() override = default;

    vector_type adjust_direction(vector_type dr) const noexcept override {return dr;}
    vector_type adjust_position (vector_type r ) const noexcept override {return r;}

    real_type volume() const noexcept override {return std::numeric_limits<real_type>::infinity();}
};

template<vectorT>
struct CuboidalPeriodicBoundary final : BoundaryCondition<vectorT>
{
  public:
    using base_type   = BoundaryCondition<vectorT>;
    using vector_type = typename base_type::vector_type;
    using real_type   = typename base_type::real_type;

  public:

    CuboidalPeriodicBoundary() noexcept
        : CuboidalPeriodicBoundary(vector_type(0, 0, 0), vector_type(0, 0, 0))
    {}

    CuboidalPeriodicBoundary(const vector_type& lw, const vector_type& up) noexcept
        : lower_(lw), upper_(up), width_(up - lw), halfw_((up - lw) / 2)
    {}
    ~CuboidalPeriodicBoundary() override = default;

    vector_type adjust_direction(vector_type dr) const noexcept
    {
        if     (dr[0] < -halfw_[0]) {dr[0] += width_[0];}
        else if(dr[0] >= halfw_[0]) {dr[0] -= width_[0];}
        if     (dr[1] < -halfw_[1]) {dr[1] += width_[1];}
        else if(dr[1] >= halfw_[1]) {dr[1] -= width_[1];}
        if     (dr[2] < -halfw_[2]) {dr[2] += width_[2];}
        else if(dr[2] >= halfw_[2]) {dr[2] -= width_[2];}
        return dr;
    }

    vector_type adjust_position(vector_type pos) const noexcept
    {
        if     (pos[0] <  lower_[0]) {pos[0] += width_[0];}
        else if(pos[0] >= upper_[0]) {pos[0] -= width_[0];}
        if     (pos[1] <  lower_[1]) {pos[1] += width_[1];}
        else if(pos[1] >= upper_[1]) {pos[1] -= width_[1];}
        if     (pos[2] <  lower_[2]) {pos[2] += width_[2];}
        else if(pos[2] >= upper_[2]) {pos[2] -= width_[2];}
        return pos;
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

}//mjolnir
#endif /* MJOLNIR_BOUNDARY_CONDITION */
