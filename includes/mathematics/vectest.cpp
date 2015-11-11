#include "RealVec.hpp"

#define STR( S ) #S
#define DUMP( V ) std::cout << STR( (V) ) << ": " << V << std::endl

using namespace coffeemill;
int main()
{
    Realvec v1(1e0, 2e0, 3e0);
    Realvec v2(1e0, 1e0, 1e0);
    Realvec v3;
    Realvec v4(v1+v2);
    Realvec v5(v4);

    DUMP(v1);
    DUMP(v2);
    DUMP(v3);
    DUMP(v4);
    DUMP(v5);

    Realvec temp;
    temp = v1+v2;
    std::cout << "v1 + v2 = " << temp << std::endl;
    temp = v1-v2;
    std::cout << "v1 - v2 = " << temp << std::endl;
 
    temp = 2e0 * v1;
    std::cout << "2 * v1 = " << temp << std::endl;
    temp = v1 * 2e0;
    std::cout << "v1 * 2 = " << temp << std::endl;
    temp = v1 / 2e0;
    std::cout << "v1 / 2 = " << temp << std::endl;

    //below: error
//     temp = 2e0 + v1;
//     std::cout << "2 + v1 = " << temp << std::endl;

    return 0;
}
