#include "SuperImpose.hpp"

namespace coffeemill
{

SuperImpose::Trajectory SuperImpose::operator()(const Trajectory& traj) const
{
    Trajectory imposed;
    imposed.reserve(traj.size());
    imposed.push_back(this->zeroing(traj.front())); // initial

    BestFit bestfit;
    bestfit.reference() = this->zeroing(this->extract(traj.front()));

    for(auto iter = traj.cbegin() + 1; iter != traj.cend(); ++iter)
    {
        const Structure extracted = this->zeroing(this->extract(*iter));
        const auto      R         = bestfit.RotationMatrix(extracted);
        const Structure snap      = this->rotate(R, this->zeroing(*iter));
        imposed.push_back(snap);

        bestfit.reference() = this->rotate(R, extracted);
        // the reference is previous imposed snapshot
    }

    return imposed;
}

SuperImpose::Structure_pair
SuperImpose::operator()(const Structure_pair& strs) const
{
    const Structure ref = this->zeroing(this->extract(strs.first));

    BestFit bestfit;
    bestfit.reference() = ref;
    const Structure sub = bestfit.fit(this->extract(strs.second));
    return std::make_pair(ref, sub);
}

SuperImpose::Structure SuperImpose::zeroing(const Structure& str) const
{
    Vector3d sum(0e0);
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        sum += *iter;
    const Vector3d CoM = sum / static_cast<double>(str.size());

    Structure zeroed;
    zeroed.reserve(str.size());
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        zeroed.push_back(*iter - CoM);
    return zeroed;
}

SuperImpose::Structure SuperImpose::extract(const Structure& str) const
{
    if(this->except_list_.empty()) return str;

    Structure retval;
    retval.reserve(str.size() - except_list_.size());

    for(std::size_t i=0; i < str.size(); ++i)
        if(std::find(this->except_list_.cbegin(),
                     this->except_list_.cend(), i) == except_list_.cend())
            retval.push_back(str[i]);

    return retval;
}

SuperImpose::Structure SuperImpose::rotate(const Matrix3d& R, const Structure& str) const
{
    Structure retval;
    retval.reserve(str.size());
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        retval.push_back(R * (*iter));

    return retval;
}

}//coffeemill
