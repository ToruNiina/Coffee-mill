#ifndef COFFEE_MILL_DCD_MANAGER
#define COFFEE_MILL_DCD_MANAGER
#include "DCDReader.hpp"
#include "DCDWriter.hpp"

namespace coffeemill
{
//     class DCDManager
//     {
//
//     };
 
    void set_header_from_reader(DCDWriter *writer, DCDReader *reader)
    {
        writer->set_nset(reader->get_nset());
        writer->set_istart(reader->get_istart());
        writer->set_nstep_save(reader->get_nstep_save());
        writer->set_nstep(reader->get_nstep());
        writer->set_nunit(reader->get_nunit());
        writer->set_npart(reader->get_npart());
        writer->set_delta_t(reader->get_delta_t());
        return;
    }  

    void set_from_reader(DCDWriter *writer, DCDReader *reader)
    {
        set_header_from_reader(writer, reader);
        writer->set_all_data(reader->get_all_data());
        return;
    }

}//end namespace coffeemill

#endif// COFFEE_MILL_DCD_MANAGER
