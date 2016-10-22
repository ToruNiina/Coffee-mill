#ifndef COFFEE_MILL_SUPER_IMPOSE
#define COFFEE_MILL_SUPER_IMPOSE
#include "BestFitStructure.hpp"
#include "DCDReader.hpp"
#include "DCDWriter.hpp"
#include "PDBReader.hpp"
#include "PDBWriter.hpp"
#include <list>

namespace mill
{

template<typename vectorT>
int superimpose(const std::string& fname)
{
    if(fname.substr(fname.size() - 4, 4) == ".dcd")
    {// no input file. use all particles.
        const std::string outname =
            fname.substr(0, fname.size() - 4) + "_imposed.dcd";

        std::ofstream ofs(outname);
        if(not ofs.good())
            throw std::runtime_error("file open error: " + outname);

        DCDReader<vectorT> reader;
        DCDWriter<vectorT> writer;
        auto dcddata = reader.read(fname);
        writer.write_header(ofs, dcddata.header());

        BestFit<typename scalar_type_extractor<vectorT>::type, Matrix> bestfit;
        bestfit.set_reference(dcddata.traj().front());
        for(auto snap = dcddata.cbegin(); snap != dcddata.cend(); ++snap)
        {
            writer.write_snapshot(ofs, bestfit.fit(*snap));
        }
        ofs.close();
        return 0;
    }
    else
    {
        throw std::invalid_argument("unknown file in argument: " + fname);
    }
}

}// mill

#endif//COFFEE_MILL_SUPER_IMPOSE
