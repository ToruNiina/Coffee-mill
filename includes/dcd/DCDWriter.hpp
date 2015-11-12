#ifndef COFFEE_MILL_DCD_WRITER
#define COFFEE_MILL_DCD_WRITER
#include "DCDDef.hpp"

namespace coffeemill
{
    class DCDWriter
    {
        public:

            DCDWriter(){}
            DCDWriter(const std::string& filename_)
                :dcdfile(filename_, std::ios::out|std::ios::binary),
                 filename(filename_)
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

            void set_nset(int nset_){nset = nset_;}
            void set_istart(int istart_){istart = istart_;}
            void set_nstep_save(int nsave_){nstep_save = nsave_;}
            void set_nstep(int nstep_){nstep = nstep_;}
            void set_nunit(int nunit_){nunit = nunit_;}
            void set_npart(int npart_){nparticle = npart_;}
            void set_delta_t(double dt){delta_t = dt;}

            SnapShot& at(int i){return data.at(i);}

            void set_all_data(const std::vector<SnapShot>& data_)
            {data = data_;}

        private:

            void write_header();
            void write_core();
            void write_head_block1();
            void write_head_block2();
            void write_head_block3();
            void write_coord(std::vector<double>& coord);
    
        private:

            int nset;// number of frame
            int istart;// initial value of isteps
            int nstep_save;// saving frequency
            int nstep;// total step number
            int nunit;// total unit number
            int verCHARMM;
            int nparticle;// total number of particle
            double delta_t;
            std::vector<SnapShot> data;
            std::ofstream dcdfile;
            std::string filename;

        private:

            static const int size_int;
            static const int size_float;
            static const int size_char;
    };

    void DCDWriter::write_file(const std::string& filename_)
    {
        if(dcdfile.is_open())
        {
            std::cout << "Warning: DCDWriter already open a dcd file."
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
        write_header();
        write_core();
        std::cout << "Info   : write_file completed" << std::endl;
        return;
    }

    void DCDWriter::write_header()
    {
        write_head_block1();
        write_head_block2();
        write_head_block3();
        return;
    }

    void DCDWriter::write_head_block1()
    {
        int wrote(0);
        int bytes(84);
        dcdfile.write(reinterpret_cast<char*>(&bytes), sizeof(int));

        char cord[5] = "CORD";
        dcdfile.write(reinterpret_cast<char*>(&cord), sizeof(char)*4);
        wrote += sizeof(char)*4;

        dcdfile.write(reinterpret_cast<char*>(&nset), sizeof(int));
        wrote += sizeof(int);

        dcdfile.write(reinterpret_cast<char*>(&istart), sizeof(int));
        wrote += sizeof(int);

        dcdfile.write(reinterpret_cast<char*>(&nstep_save), sizeof(int));
        wrote += sizeof(int);

        dcdfile.write(reinterpret_cast<char*>(&nstep), sizeof(int));
        wrote += sizeof(int);

        dcdfile.write(reinterpret_cast<char*>(&nunit), sizeof(int));
        wrote += sizeof(int);

        // null data
        int fill_zero(0);
        for(int i(0); i<4; ++i)
        {
            dcdfile.write(reinterpret_cast<char*>(&fill_zero), sizeof(int));
            wrote += sizeof(int);
        }

        float dt(static_cast<float>(delta_t));
        dcdfile.write(reinterpret_cast<char*>(&dt), sizeof(float));
        wrote += sizeof(float);

        // null data
        for(int i(0); i<9; ++i)
        {
            dcdfile.write(reinterpret_cast<char*>(&fill_zero), sizeof(int));
            wrote += sizeof(int);
        }

        dcdfile.write(reinterpret_cast<char*>(&verCHARMM), sizeof(int));
        wrote += sizeof(int);

        if(wrote != bytes)
        {
            throw std::invalid_argument("byte information error");
        }

        dcdfile.write(reinterpret_cast<char*>(&bytes), sizeof(int));
        return;
    }

    void DCDWriter::write_head_block2()
    {
        int lines(4);
        int bytes(4 + 80*lines);
        int wrote(0);
        dcdfile.write(reinterpret_cast<char*>(&bytes), sizeof(int));

        dcdfile.write(reinterpret_cast<char*>(&lines), sizeof(int));
        wrote += sizeof(int);

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
        char line4[81];
        sprintf(line4, "%s", fileinfo.c_str());

        dcdfile.write(reinterpret_cast<char*>(&line1), sizeof(char)*80);
        wrote += sizeof(char)*80;
        dcdfile.write(reinterpret_cast<char*>(&line2), sizeof(char)*80);
        wrote += sizeof(char)*80;
        dcdfile.write(reinterpret_cast<char*>(&line3), sizeof(char)*80);
        wrote += sizeof(char)*80;
        dcdfile.write(reinterpret_cast<char*>(&line4), sizeof(char)*80);
        wrote += sizeof(char)*80;

        if(wrote != bytes)
            throw std::invalid_argument("byte information error");

        dcdfile.write(reinterpret_cast<char*>(&bytes), sizeof(int));
        return;
    }

    void DCDWriter::write_head_block3()
    {
        int bytes(sizeof(int));
        dcdfile.write(reinterpret_cast<char*>(&bytes), sizeof(int));
        dcdfile.write(reinterpret_cast<char*>(&nparticle), sizeof(int));
        dcdfile.write(reinterpret_cast<char*>(&bytes), sizeof(int));
        return;
    }

    void DCDWriter::write_core()
    {
        for(std::vector<SnapShot>::iterator iter = data.begin();
            iter != data.end(); ++iter)
        {
            std::vector<double> x((*iter).size());
            std::vector<double> y((*iter).size());
            std::vector<double> z((*iter).size());
            int counter(0);
            for(SnapShot::iterator ssiter = (*iter).begin();
                ssiter != (*iter).end(); ++ssiter)
            {
                x.at(counter) = (*ssiter)[0];
                y.at(counter) = (*ssiter)[1];
                z.at(counter) = (*ssiter)[2];
                ++counter;
            }
            write_coord(x);
            write_coord(y);
            write_coord(z);
        }
        return;
    }

    void DCDWriter::write_coord(std::vector<double>& coord)
    {
        int size(coord.size());
        int bytes(size * sizeof(float));
        dcdfile.write(reinterpret_cast<char*>(&bytes), sizeof(int));

        for(std::vector<double>::iterator iter = coord.begin();
            iter != coord.end(); ++iter)
        {
            float temp(static_cast<float>(*iter));
            dcdfile.write(reinterpret_cast<char*>(&temp), sizeof(float));
        }

        dcdfile.write(reinterpret_cast<char*>(&bytes), sizeof(int));
        return;
    }

}//end namespace coffeemill

#endif //COFFEE_MILL_DCD_WRITER
