#ifndef COFFEE_MILL_MASS_CONST
#define COFFEE_MILL_MASS_CONST
#include <string>
#include <stdexcept>

//maybe map<char,double> is better

namespace coffeemill
{
    class AtomMass
    {
        public:

            static AtomMass& getInstance();

            AtomMass(const AtomMass& am) = delete;
            AtomMass(AtomMass&&) = delete;
            AtomMass& operator=(const AtomMass& am) = delete;
            AtomMass& operator=(AtomMass&& am) = delete;

        private:

            AtomMass() = default;
            ~AtomMass() = default;

        public:

            static double get_mass_atom(const char atom);
//             static double get_mass_amino(const std::string& amino);

            constexpr static double mass_P = 30.973761e0;
            constexpr static double mass_O = 15.9994e0;
            constexpr static double mass_C = 12.0107e0;
            constexpr static double mass_N = 14.0067e0;
            constexpr static double mass_H = 1.008e0;
            constexpr static double mass_F = 18.9984032e0;
            constexpr static double mass_S = 32.065e0;
            constexpr static double mass_BR= 79.904e0;

            constexpr static double mass_PO4 = mass_P + mass_O * 4;
            constexpr static double mass_ALA = mass_C * 3 + mass_H * 7 +
                                               mass_N     + mass_O * 2;
            constexpr static double mass_CYS = mass_C * 3 + mass_H * 7 +
                                               mass_N     + mass_O * 2 + mass_S;
            constexpr static double mass_ASP = mass_C * 4 + mass_H * 7 +
                                               mass_N     + mass_O * 4;
            constexpr static double mass_GLU = mass_C * 5 + mass_H * 9 +
                                               mass_N     + mass_O * 4;
            constexpr static double mass_PHE = mass_C * 9 + mass_H * 11+
                                               mass_N     + mass_O * 2;
            constexpr static double mass_GLY = mass_C * 2 + mass_H * 5 +
                                               mass_N     + mass_O * 2;
            constexpr static double mass_HIS = mass_C * 6 + mass_H * 9 +
                                               mass_N * 3 + mass_O * 2;
            constexpr static double mass_ILE = mass_C * 6 + mass_H * 13+
                                               mass_N     + mass_O * 2;
            constexpr static double mass_LYS = mass_C * 6 + mass_H * 14+
                                               mass_N * 2 + mass_O * 2;
            constexpr static double mass_LEU = mass_C * 6 + mass_H * 13+
                                               mass_N     + mass_O * 2;
            constexpr static double mass_MET = mass_C * 5 + mass_H * 11+
                                               mass_N     + mass_O * 2 + mass_S;
            constexpr static double mass_ASN = mass_C * 4 + mass_H * 8 +
                                               mass_N * 2 + mass_O * 3;
            constexpr static double mass_PRO = mass_C * 5 + mass_H * 9 +
                                               mass_N     + mass_O * 2;
            constexpr static double mass_GLN = mass_C * 5 + mass_H * 10+
                                               mass_N * 2 + mass_O * 3;
            constexpr static double mass_ARG = mass_C * 6 + mass_H * 14+
                                               mass_N * 4 + mass_O * 2;
            constexpr static double mass_SER = mass_C * 3 + mass_H * 7 +
                                               mass_N     + mass_O * 3;
            constexpr static double mass_THR = mass_C * 4 + mass_H * 9 +
                                               mass_N     + mass_O * 3;
            constexpr static double mass_VAL = mass_C * 5 + mass_H * 11+
                                               mass_N     + mass_O * 2;
            constexpr static double mass_TRP = mass_C * 11+ mass_H * 12+
                                               mass_N * 2 + mass_O * 2;
            constexpr static double mass_TYR = mass_C * 9 + mass_H * 11+
                                               mass_N     + mass_O * 3;
    };

    AtomMass& AtomMass::getInstance()
    {
        static AtomMass instance;
        return instance;
    }

    double AtomMass::get_mass_atom(const char atom)
    {
        switch(atom)
        {
            case 'H':
                return mass_H;
            case 'C':
                return mass_C;
            case 'N':
                return mass_N;
            case 'O':
                return mass_O;
            case 'P':
                return mass_P;
            case 'S':
                return mass_S;
            case 'F':
                return mass_F;
            default:
                throw std::invalid_argument("not defined atom" + std::string(&atom));
        }
    }

}
#endif //COFFEE_MILL_MASS_CONST
