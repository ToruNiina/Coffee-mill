#ifndef COFFEE_MILL_NUC_TRACKER
#define COFFEE_MILL_NUC_TRACKER
#include "dcd/DCDManager.hpp"
#include "SuperImposer.hpp"

// this code is very hardly coded!
// may do not work correctly in general.

namespace coffeemill
{
    SnapShot pickupHistoneCore(const SnapShot& ss, const int DNA_length)
    {
        SnapShot retval(ss);
        SnapShot::iterator iter = retval.begin();
        retval.erase(iter, iter + (2 * DNA_length));

        for(int i(0); i < 2; ++i)
        {
            retval.erase(iter, iter + 32);//size of each tail.
            iter = iter + 103;            //size of histone. 135 - tail.
            retval.erase(iter, iter + 23);
            iter = iter + 79;             //size of histone. 102 - tail.
            retval.erase(iter, iter + 14);
            iter = iter + 106;            //size of histone. 128 - tail.
            retval.erase(iter, iter + 8);
            retval.erase(iter, iter + 21);
            iter = iter + 104;            //size of histone. 125 - tail.
        }
        if(retval.size() != 2 * (490 - 32 - 23 - 14 - 8 - 21))
        {
            std::cout << "truncated size: " << retval.size() << std::endl;
            throw std::logic_error("internal error");
        }
        return retval;
    }

    SnapShot pickupHistoneTail(const SnapShot& ss, const int DNA_length)
    {
        SnapShot retval(ss);
        SnapShot::iterator iter = retval.begin();
        retval.erase(iter, iter + (2 * DNA_length));

        for(int i(0); i < 2; ++i)
        {
            iter = iter + 32;
            retval.erase(iter, iter + 103);
            iter = iter + 23;
            retval.erase(iter, iter + 79);
            iter = iter + 14;
            retval.erase(iter, iter + 106);
            iter = iter + 8;
            iter = iter + 21;
            retval.erase(iter, iter + 104);
        }
        return retval;
    }

    std::vector<Realvec> extractDNAcenter(const SnapShot& ss, const int DNA_length)
    {
        size_t dna_base((DNA_length + 1)/3);
        std::vector<Realvec> retval(dna_base);

        SnapShot dna_only(ss);
        dna_only.erase(dna_only.begin()+(DNA_length*2), dna_only.end());

        size_t dnaonlysize(dna_only.size()-1);
        for(size_t i(0); i<dna_base; ++i)
        {
            Realvec fwbase_pos = dna_only.at(1 + 3*i);
            Realvec rvbase_pos = dna_only.at(dnaonlysize - 3*i);

            retval.at(i) = (fwbase_pos + rvbase_pos) / 2e0;
        }

        return retval;
    }
}
#endif //COFFEE_MILL_NUC_TRACKER
