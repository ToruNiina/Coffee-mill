#ifndef COFFEE_MILL_DEFINITIONS
#define COFFEE_MILL_DEFINITIONS
#include <cstddef>

#define STR( S ) #S
#define DUMP( V )\
    std::cout << std::string(STR(V)) << ": " << (V) << std::endl;

namespace coffeemill
{
    class type_size
    {
        public:
            static constexpr size_t size_int = sizeof(int);
            static constexpr size_t size_char = sizeof(char);
            static constexpr size_t size_float = sizeof(float);
            static constexpr size_t size_double = sizeof(double);
    };

    enum MOLECULE_TYPE
    {
        PROTEIN,
        DNA,
        RNA,
        LIPID,
        LIGAND,
        UNKNOWN
    };

}

#endif//COFFEE_MILL_DEFINITIONS
