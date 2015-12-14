#ifndef COFFEE_MILL_DCD_WRITER
#define COFFEE_MILL_DCD_WRITER
#include <cstring>
#include "DCDDef.hpp"

namespace coffeemill
{
    class DCDWriter
    {
        public:

            DCDWriter():header_written(false), header_set(false){}

            DCDWriter(const std::string& filename_)
                :header_written(false), header_set(false),
                 filename(filename_),
                 dcdfile(filename_, std::ios::out|std::ios::binary)
            {
                if(dcdfile.fail())
                    throw std::invalid_argument("file open error");
            }

            ~DCDWriter()
            {
                dcdfile.close();
            }

            void write_file();
            void write_file(const std::string& filename);

            void write_header();
            void write_SnapShot(SnapShot& snapshot);

            void set_nset(int nset_){nset = nset_;}
            void set_istart(int istart_){istart = istart_;}
            void set_nstep_save(int nsave_){nstep_save = nsave_;}
            void set_nstep(int nstep_){nstep = nstep_;}
            void set_nunit(int nunit_){nunit = nunit_;}
            void set_npart(int npart_){nparticle = npart_;}
            void set_delta_t(double dt){delta_t = dt;}
            void set_header(const std::vector<std::string>& h);

            SnapShot& at(int i){return data.at(i);}

            void set_all_data(const std::vector<SnapShot>& data_)
            {data = data_;}

        private:

            void write_core();
            void write_head_block1();
            void write_head_block2();
            void write_head_block3();
            void write_user_set_header();
            void write_coord(std::vector<double>& coord);
    
        private:

            bool header_written;
            bool header_set;
            int nset;// number of frame
            int istart;// initial value of isteps
            int nstep_save;// saving frequency
            int nstep;// total step number
            int nunit;// total unit number
            int verCHARMM;
            int nparticle;// total number of particle
            double delta_t;
            std::string filename;
            std::ofstream dcdfile;
            std::vector<std::string> header;
            std::vector<SnapShot> data;

        private:

            static constexpr int size_int = sizeof(int);
            static constexpr int size_float = sizeof(float);
            static constexpr int size_char = sizeof(char);
    };

    void DCDWriter::write_file(const std::string& filename_)
    {
        if(dcdfile.is_open())
        {
            std::cout << "Warning: DCDWriter already open a dcd file: "
                      << filename
                      << std::endl;
        }
        else
        {
            dcdfile.open(filename_, std::ios::out|std::ios::binary);
            if(dcdfile.fail())
                throw std::invalid_argument("file open error");
        }
        write_file();
        return;
    }

    void DCDWriter::write_file()
    {
        if(dcdfile.fail())
            throw std::invalid_argument("file open error");
        if(header_written)
            throw std::invalid_argument("file is partially written");
        write_header();
        write_core();
        std::cout << "Info   : write_file completed" << std::endl;
        return;
    }

    void DCDWriter::write_SnapShot(SnapShot& snapshot)
    {
        if(!header_written)
            throw std::invalid_argument("write DCD file without header");

        std::vector<double> x((snapshot).size());
        std::vector<double> y((snapshot).size());
        std::vector<double> z((snapshot).size());
        int counter(0);
        for(auto iter = (snapshot).begin(); iter != (snapshot).end(); ++iter)
        {
            x.at(counter) = (*iter)[0];
            y.at(counter) = (*iter)[1];
            z.at(counter) = (*iter)[2];
            ++counter;
        }
        write_coord(x);
        write_coord(y);
        write_coord(z);

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
                header.push_back((*iter) + filler);
            }
            else
            {
                header.push_back(*iter);
            }
        }
        header_set = true;
        return;
    }

    void DCDWriter::write_header()
    {
        write_head_block1();
        if(header_set)
        {
            write_user_set_header();
        }
        else
        {
            write_head_block2();
        }
        write_head_block3();
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
        int lines(4);
        int bytes(4 + 80*lines);
        int wrote(0);
        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);

        dcdfile.write(reinterpret_cast<char*>(&lines), size_int);
        wrote += size_int;

        size_t filename_length(filename.size());
        std::string fileinfo;
        if(filename_length > 41)
        {
            fileinfo = "==================== original file is " + filename.substr(0,63);
        }
        else
        {
            fileinfo = "==================== original file is " + filename + ' ';
//             std::cout << "fileinfo size: " << fileinfo.size() << std::endl;
            std::string filler((80 - fileinfo.size()), '=');
            fileinfo = fileinfo + filler;
            if(fileinfo.size() != 80)
            {
                std::cout << "filename: " << filename.size() << std::endl;
                std::cout << "filler size: " << filler.size() << std::endl;
                std::cout << "fileinfo size: " << fileinfo.size() << std::endl;
                throw std::invalid_argument("line size error");
            }
        }

        char line1[81] = "==================== Molecular Dynamics Code : CafeMol ver.  2.01.1485 =========";
        char line2[81] = "==================== Developed by Kyoto University =============================";
        char line3[81] = "==================== This file is modified using coffee-mill ===================";
        char line4[81] = "==================================== test ======================================";
//         sprintf(line4, "%s", fileinfo.c_str());

        dcdfile.write(reinterpret_cast<char*>(&line1), size_char*80);
        wrote += size_char*80;
        dcdfile.write(reinterpret_cast<char*>(&line2), size_char*80);
        wrote += size_char*80;
        dcdfile.write(reinterpret_cast<char*>(&line3), size_char*80);
        wrote += size_char*80;
        dcdfile.write(reinterpret_cast<char*>(&line4), size_char*80);
        wrote += size_char*80;

        if(wrote != bytes)
            throw std::invalid_argument("byte information error");

        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
        return;
    }

    void DCDWriter::write_user_set_header()
    {
        int lines(header.size());
        int bytes(4 + 80*lines);
        int wrote(0);
        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);

        dcdfile.write(reinterpret_cast<char*>(&lines), size_int);
        wrote += size_int;

        for(auto iter = header.begin(); iter != header.end(); ++iter)
        {
            char* line = new char[81];
            memcpy(line, (*iter).c_str(), 81);
            dcdfile.write(reinterpret_cast<char*>(&line), size_char*80);
            wrote += size_char*80;
        }

        if(wrote != bytes)
            throw std::invalid_argument("byte information error");

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
        for(auto iter = data.begin(); iter != data.end(); ++iter)
        {
            write_SnapShot(*iter);
        }
        return;
    }

    void DCDWriter::write_coord(std::vector<double>& coord)
    {
        int size(coord.size());
        int bytes(size * size_float);
        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);

        for(auto iter = coord.begin(); iter != coord.end(); ++iter)
        {
            float temp(static_cast<float>(*iter));
            dcdfile.write(reinterpret_cast<char*>(&temp), size_float);
        }

        dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
        return;
    }

}//end namespace coffeemill

#endif //COFFEE_MILL_DCD_WRITER
