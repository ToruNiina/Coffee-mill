#ifndef COFFEE_MILL_NINFO_DEF
#define COFFEE_MILL_NINFO_DEF
#include <utility>

namespace coffeemill
{

    enum BlockType
    {
         N_BOND,
         N_ANGL,
         N_DIHD,
         N_AICG13,
         N_AICG14,
         N_AICGDIH,
         N_CONTACT,
         N_BASEPAIR,
         N_BASESTACK,
         NO_BLOCK,
         N_BASE
    };

    enum LineType
    {
        BLOCK_START,
        NINFO_LINE,
        BLOCK_END,
        COMMENT,
        EMPTY,
        WRONG
    };

    typedef std::pair<int, int> iUnits;

    bool operator==(const iUnits& lhs, const iUnits& rhs)
    {
        bool forward(lhs.first == rhs.first && lhs.second == rhs.second);
        bool backward(lhs.first == rhs.second && lhs.second == rhs.first);
        return forward || backward;                                                               
    }

    bool operator<(const iUnits& lhs, const iUnits& rhs)
    {
        if(lhs.first != rhs.first)
        {
            return (lhs.first < rhs.first);
        }else{
            return (lhs.second < rhs.second);
        }
    }
    
}
#endif //COFFEE_MILL_NINFO_DEF
