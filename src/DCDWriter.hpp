#ifndef COFFEE_MILL_DCD_WRITER
#define COFFEE_MILL_DCD_WRITER
#include <cstring>
#include "DCDDef.hpp"

namespace coffeemill
{
    /*@brief > this class recieves data of trajectory(array of snapshot) and *
     *         output the data to dcd file. you can set all trajectory data  *
     *         and also write each snapshot step by step.                    *
     *         Use set_data     & write_file     for all-trajectory output   *
     *             write_header & write_SnapShot for step by step output     */
    class DCDWriter
    {
        public:
            using iterator = std::vector<SnapShot>::iterator;
            using const_iterator = std::vector<SnapShot>::const_iterator;

        public:

            DCDWriter()
                : header_written(false)
            {
                ;
            }

            DCDWriter(const std::string& filename_)
                : header_written(false), filename(filename_)
            {
                ;
            }

            ~DCDWriter() = default;

            void write_file();
            void write_file(const std::string& filename);

            void write_header();
            void write_SnapShot(const SnapShot& snapshot);

            void set_nset       (const int nset_)   {nset = nset_;}
            void set_istart     (const int istart_) {istart = istart_;}
            void set_nstep_save (const int nsave_)  {nstep_save = nsave_;}
            void set_nstep      (const int nstep_)  {nstep = nstep_;}
            void set_nunit      (const int nunit_)  {nunit = nunit_;}
            void set_npart      (const int npart_)  {nparticle = npart_;}
            void set_delta_t    (const double dt)   {delta_t = dt;}
            void set_header     (const std::vector<std::string>& h);

            SnapShot& at(const std::size_t i)       {return data.at(i);}
            SnapShot  at(const std::size_t i) const {return data.at(i);}

            void set_all_data(const std::vector<SnapShot>& dat);

        private:

            // these private member asserts that file is open at any time.
            void write_core();
            void write_head_block1();
            void write_head_block2();
            void write_head_block3();
            void write_coord(const std::vector<double>& coord);
    
        private:

            bool header_written;// already write header
            int nset;           // number of frame
            int istart;         // initial value of isteps
            int nstep_save;     // saving frequency
            int nstep;          // total step number
            int nunit;          // total unit number
            int verCHARMM = 24; // have no meaning but write something
            int nparticle;      // total number of particle
            double delta_t;
            std::string filename;
            std::ofstream dcdfile;
            std::vector<std::string> header;
            std::vector<SnapShot> data;

        private:

            static constexpr int size_int   = sizeof(int);
            static constexpr int size_float = sizeof(float);
            static constexpr int size_char  = sizeof(char);
    };

    void DCDWriter::set_all_data(const std::vector<SnapShot>& dat)
    {
        data = dat;
        nset = data.size();
    }

    void DCDWriter::write_file(const std::string& filename_)
    {
        if(!filename.empty())
        {
            std::cerr << "Warning: you already specify filename in dcdwriter "
                      << filename << " but now you are trying to open "
                      << filename_ << std::endl;
        }

        filename = filename_;
        write_file();
        return;
    }

    void DCDWriter::write_file()
    {
        if(header_written)
            throw std::invalid_argument("file is partially written");

        write_header();
        write_core();

        std::cout << "Info   : write_file completed" << std::endl;
        return;
    }


    void DCDWriter::set_header(const std::vector<std::string>& h)
    {
        if(!header.empty())
            std::cout << "Warning: trying to set header but header is not empty"
                      << std::endl;

        for(auto iter = h.begin(); iter != h.end(); ++iter)
        {
            if((*iter).size() > 80)
            {
                throw std::invalid_argument("too long header line");
            }
            else if((*iter).size() < 80)
            {
                std::string filler((80 - (*iter).size()), '=');
                std::string temp = *iter + filler;
                if(temp.size() != 80)
                    throw std::length_error("header generate error");

                header.push_back(temp);
            }
            else
            {// just 80 chars
                header.push_back(*iter);
            }
        }
        return;
    }

    void DCDWriter::write_header()
    {
        dcdfile.open(filename, std::ios::binary);
        write_head_block1();
        write_head_block2();
        write_head_block3();
        dcdfile.close();
        header_written = true;
        return;
    }

    void DCDWriter::write_head_block1()
    {
        int wrote(0);
        int bytes(84);
        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);

        char cord[5] = "CORD";
        dcdfile.write(reinterpret_cast<char*>(&cord), size_char*4);
        wrote += size_char*4;

        dcdfile.write(reinterpret_cast<char*>(&nset), size_int);
        wrote += size_int;

        dcdfile.write(reinterpret_cast<char*>(&istart), size_int);
        wrote += size_int;

        dcdfile.write(reinterpret_cast<char*>(&nstep_save), size_int);
        wrote += size_int;

        dcdfile.write(reinterpret_cast<char*>(&nstep), size_int);
        wrote += size_int;

        dcdfile.write(reinterpret_cast<char*>(&nunit), size_int);
        wrote += size_int;

        // null data
        int fill_zero(0);
        for(int i(0); i<4; ++i)
        {
            dcdfile.write(reinterpret_cast<char*>(&fill_zero), size_int);
            wrote += size_int;
        }

        float dt(static_cast<float>(delta_t));
        dcdfile.write(reinterpret_cast<char*>(&dt), size_float);
        wrote += size_float;

        // null data
        for(int i(0); i<9; ++i)
        {
            dcdfile.write(reinterpret_cast<char*>(&fill_zero), size_int);
            wrote += size_int;
        }

        dcdfile.write(reinterpret_cast<char*>(&verCHARMM), size_int);
        wrote += size_int;

        if(wrote != bytes)
        {
            throw std::invalid_argument("byte information error");
        }

        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
        return;
    }

    void DCDWriter::write_head_block2()
    {
        if(header.empty())
        {
            header.push_back("==================== Molecular Dynamics Code : CafeMol ver.  2.02 ==============");
            header.push_back("==================== Developed by Kyoto University =============================");
            header.push_back("==================== This file is modified using coffee-mill ===================");
        }

        int lines(header.size());
        int bytes(4 + 80*lines);
        int wrote(0);

        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);

        dcdfile.write(reinterpret_cast<char*>(&lines), size_int);
        wrote += size_int;

        for(auto iter = header.begin(); iter != header.end(); ++iter)
        {
//             char* line = new char[81];
//             memcpy(line, iter->c_str(), 81);
            dcdfile << *iter;
//             dcdfile.write(reinterpret_cast<char*>(&line), size_char*80);
            wrote += size_char*80;
        }

        if(wrote != bytes)
            throw std::invalid_argument("internal byte information error");

        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
        return;
    }

    void DCDWriter::write_head_block3()
    {
        int bytes(size_int);
        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
        dcdfile.write(reinterpret_cast<char*>(&nparticle), size_int);
        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
        return;
    }

    void DCDWriter::write_core()
    {
        for(auto iter = data.cbegin(); iter != data.cend(); ++iter)
        {
            write_SnapShot(*iter);
        }
        return;
    }

    void DCDWriter::write_SnapShot(const SnapShot& snapshot)
    {
        if(filename.empty())
            throw std::invalid_argument("filename is not specified!");

        if(!header_written)
            throw std::invalid_argument("write DCD file without header");

        if(dcdfile.is_open())
        {
            throw std::invalid_argument("DCDWriter: file already open");
        }
        else
        {
            dcdfile.open(filename, std::ios::binary | std::ios::app);
            if(dcdfile.fail())
                throw std::invalid_argument("file open error: " + filename);
        }

        std::vector<double> x(snapshot.size());
        std::vector<double> y(snapshot.size());
        std::vector<double> z(snapshot.size());
        int counter(0);
        for(auto iter = (snapshot).cbegin(); iter != (snapshot).cend(); ++iter)
        {
            x[counter] = (*iter)[0];
            y[counter] = (*iter)[1];
            z[counter] = (*iter)[2];
            ++counter;
        }

        write_coord(x);
        write_coord(y);
        write_coord(z);

        dcdfile.close();

        return;
    }

    void DCDWriter::write_coord(const std::vector<double>& coord)
    {
        int size(coord.size());
        int bytes(size * size_float);
        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);

        for(auto iter = coord.cbegin(); iter != coord.cend(); ++iter)
        {
            float temp(static_cast<float>(*iter));
            dcdfile.write(reinterpret_cast<char*>(&temp), size_float);
        }

        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
        return;
    }

}//end namespace coffeemill

#endif //COFFEE_MILL_DCD_WRITER
