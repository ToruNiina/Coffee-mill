#ifndef COFFEE_MILL_SUPER_IMPOSE
#define COFFEE_MILL_SUPER_IMPOSE
#include "BestFitStructure.hpp"
#include <list>

namespace coffeemill
{

class SuperImpose
{
  public:

    using Vector3d       = ax::Vector3d;
    using Matrix3d       = ax::Matrix3;
    using Structure      = std::vector<Vector3d>;
    using Structure_pair = std::pair<Structure, Structure>;
    using Trajectory     = std::vector<Structure>;

  public:

    SuperImpose()  = default;
    explicit SuperImpose(const std::list<std::size_t> except_list)
        :except_list_(except_list)
    {}
    ~SuperImpose() = default;

    Trajectory     operator()(const Trajectory& traj) const;
    Structure_pair operator()(const Structure_pair& strs) const;

          std::list<std::size_t>& except_list()       {return except_list_;}
    const std::list<std::size_t>& except_list() const {return except_list_;}

  private:

    Structure zeroing(const Structure& str) const;
    Structure extract(const Structure& str) const;

  private:
    std::list<std::size_t> except_list_;
};



}// coffeemill

#endif//COFFEE_MILL_SUPER_IMPOSE
