#ifndef COFFEE_MILL_DCD_MANAGER
#define COFFEE_MILL_DCD_MANAGER
#include "DCDReader.hpp"
#include "DCDWriter.hpp"

namespace coffeemill
{
    class DCDManager
    {
        public:
            DCDManager(){}

            void set_ifname(const std::string& ifname){readfilename = ifname;}
            void set_ofname(const std::string& ofname){writefilename = ofname;}

            void read_file(const std::string& filename);
            void write_file(const std::string& filename);

        private:

            std::string readfilename;
            std::string writefilename;
            std::unique_ptr<DCDReader> reader;
            std::unique_ptr<DCDWriter> writer;
    };

    void DCDManager::read_file(const std::string& filename)
    {
        if(reader)
        {
            std::cerr << "Warning: DCDManager has read a dcd file\""
                      << reader->get_filename() << "\"" << std::endl;
            std::cerr << "       : but now discard the file "
                      << " and starting to read " << filename
                      << std::endl;
        }
        reader.reset(new DCDReader(filename));
        reader->read_file();
        return;
    }

    void DCDManager::write_file(const std::string& filename)
    {
        reader.reset(new DCDReader(filename));
        reader->read_file();
        return;
    }
 
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
