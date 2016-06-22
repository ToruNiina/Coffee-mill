#include "SuperImpose.hpp"

namespace coffeemill
{

SuperImpose::Trajectory SuperImpose::operator()(const Trajectory& traj) const
{
    Trajectory imposed;
    imposed.reserve(traj.size());

    imposed.push_back(this->zeroing(traj.front(),
                                    this->center(this->extract(traj.front()))));

    BestFit bestfit;
    bestfit.reference() = this->zeroing(this->extract(traj.front()));

    for(auto iter = traj.cbegin() + 1; iter != traj.cend(); ++iter)
    {
        const Structure extracted  = this->extract(*iter);
        const Vector3d  ext_center = this->center(extracted);
        const auto      R          = bestfit.RotationMatrix(this->zeroing(extracted, ext_center));
        const Structure snap       = this->rotate(R, this->zeroing(*iter, ext_center));
        imposed.push_back(snap);

        // the reference is previous imposed snapshot
        bestfit.reference() = this->rotate(R, extracted);
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

    return this->zeroing(str, CoM);
}

SuperImpose::Structure SuperImpose::zeroing(const Structure& str,
                                            const Vector3d& center) const
{
    Structure zeroed;
    zeroed.reserve(str.size());
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        zeroed.push_back(*iter - center);
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

SuperImpose::Vector3d SuperImpose::center(const Structure& str) const
{
    Vector3d sum(0e0);
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        sum += *iter;
    return sum / static_cast<double>(str.size());
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
