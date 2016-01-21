#ifndef COFFEE_MILL_RMSD_CALCULATOR
#define COFFEE_MILL_RMSD_CALCULATOR
#include "SuperImposer.hpp"

namespace coffeemill
{
    /*@brief > this calculates RMSD value. All of the imposing process   *
     * is done by SuperImposer, so this calculates only the distance and *
     * some of square of the distances(this is just RMSD).               */
    class RMSDCalculator
    {
        public:
            RMSDCalculator(){}
            ~RMSDCalculator() = default;

            double get_RMSD(const std::vector<Realvec>& reference,
                            const std::vector<Realvec>& subject);

        private:

            double calc_RMSD(const std::vector<Realvec>& rA, 
                             const std::vector<Realvec>& rB);
    };

    double RMSDCalculator::get_RMSD(const std::vector<Realvec>& reference,
                                    const std::vector<Realvec>& subject)
    {
        SuperImposer imposer(reference, subject);
        imposer.superimpose();

        return calc_RMSD(imposer.get_reference(), imposer.get_subject());
    }

    double RMSDCalculator::calc_RMSD(const std::vector<Realvec>& reference,
                                     const std::vector<Realvec>& subject)
    {
        double sigma(0e0);
        for(std::size_t i(0); i<reference.size(); ++i)
        {
            sigma += len_square(reference[i] - subject[i]);
        }
        return std::sqrt(sigma / static_cast<double>(reference.size()));
    }

}

#endif //COFFEE_MILL_RMSD_CALCLATOR
