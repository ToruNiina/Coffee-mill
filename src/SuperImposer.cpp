#include "SuperImposer.hpp"

namespace coffeemill
{

SuperImpose::Trajectory SuperImpose::operator()(const Trajectory& traj) const
{
    Trajectory imposed;
    imposed.resize(traj.size());

    Structure prev = this->zeroing(this->extract(traj.front()));
    imposed.push_back(prev); // initial

    BestFit bestfit;
    bestfit.reference() = prev;

    for(auto iter = traj.cbegin() + 1; iter != traj.cend(); ++iter)
    {
        const Structure snap = bestfit.fit(this->extract(*iter));
        imposed.push_back(snap);

        bestfit.reference() = snap;// the reference is previous imposed snapshot
    }

    return imposed;
}

SuperImpose::Structure_pair
SuperImpose::operator()(const Structure_pair& strs) const
{
    Structure ref = this->zeroing(this->extract(strs.first));

    BestFit bestfit;
    bestfit.reference() = ref;
    Structure sub = bestfit.fit(this->extract(strs.second));
    return std::make_pair(ref, sub);
}

SuperImpose::Structure SuperImpose::zeroing(const Structure& str) const
{
    Vector3d sum(0e0);
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        sum += *iter;
    const Vector3d CoM = sum / static_cast<double>(str.size());

    Structure zeroed;
    zeroed.resize(str.size());
    for(auto iter = str.cbegin(); iter != str.cend(); ++iter)
        zeroed.push_back(*iter - CoM);
    return zeroed;
}

SuperImpose::Structure SuperImpose::extract(const Structure& str) const
{
    if(this->except_list_.empty()) return str;

    Structure retval;
    retval.resize(str.size() - except_list_.size());

    for(std::size_t i=0; i < str.size(); ++i)
        if(std::find(this->except_list_.cbegin(),
                     this->except_list_.cend(), i) == except_list_.cend())
            retval.push_back(str[i]);

    return retval;
}

}//coffeemill
