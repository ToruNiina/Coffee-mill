#ifndef COFFEE_MILL_NBLOCK_FACTORY
#define COFFEE_MILL_NBLOCK_FACTORY
#include "NinfoBond.hpp"
#include "NinfoAngl.hpp"
#include "NinfoDihd.hpp"
#include "NinfoAicg13.hpp"
#include "NinfoAicg14.hpp"
#include "NinfoAicgdih.hpp"
#include "NinfoContact.hpp"
#include "NinfoBasepair.hpp"
#include "NinfoBasestack.hpp"

namespace coffeemill
{
    class NinfoBlockFactory
    {
        public:

            NinfoBlockFactory(){}
            ~NinfoBlockFactory(){}

            BlockBase* create(const BlockType& block_t);
            BlockSptr create_Sptr(const BlockType& block_t);
    };

    BlockBase* NinfoBlockFactory::create(const BlockType& block_t)
    {
        switch(block_t)
        {
            case N_BOND:
                return new BondBlock;
            case N_ANGL:
                return new AnglBlock;
            case N_DIHD:
                return new DihdBlock;
            case N_AICG13:
                return new Aicg13Block;
            case N_AICG14:
                return new Aicg14Block;
            case N_AICGDIH:
                return new AicgdihBlock;
            case N_CONTACT:
                return new ContactBlock;
            case N_BASEPAIR:
                return new BasepairBlock;
            case N_BASESTACK:
                return new BasestackBlock;
            default:
                std::cout << "NinofBlockFactory recieve unknown type." << std::endl;
                return NULL;
        }
    }

    BlockSptr NinfoBlockFactory::create_Sptr(const BlockType& block_t)
    {
        switch(block_t)
        {
            case N_BOND:
                return BlockSptr(new BondBlock);
            case N_ANGL:
                return BlockSptr(new AnglBlock);
            case N_DIHD:
                return BlockSptr(new DihdBlock);
            case N_AICG13:
                return BlockSptr(new Aicg13Block);
            case N_AICG14:
                return BlockSptr(new Aicg14Block);
            case N_AICGDIH:
                return BlockSptr(new AicgdihBlock);
            case N_CONTACT:
                return BlockSptr(new ContactBlock);
            case N_BASEPAIR:
                return BlockSptr(new BasepairBlock);
            case N_BASESTACK:
                return BlockSptr(new BasestackBlock);
            default:
                throw std::invalid_argument("Factory: unknown blocktype");
        }
    }

}//end namespace coffeemill

#endif //COFFEE_MILL_NBLOCK_FACTORY
