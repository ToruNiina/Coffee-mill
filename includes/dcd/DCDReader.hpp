#ifndef COFFEE_MILL_DCD_READER
#define COFFEE_MILL_DCD_READER
#include <memory>
#include <utility>
#include "DCDDef.hpp"

namespace coffeemill
{
    /*@brief > this class reads dcd file and contain information and       *
     *         array of snapshots of the trajectory.                       *
     *         You can get the vector and information separately and also  *
     *             can use this as wrapped std::vector<SnapShot>.          */
    class DCDReader
    {
        public:
            using iterator = std::vector<SnapShot>::iterator;
            using const_iterator = std::vector<SnapShot>::const_iterator;

        public:
            DCDReader(){}
            DCDReader(const std::string& fname)
            : filename(fname)
            {}

            DCDReader(const DCDReader& reader)            = delete;
            DCDReader(DCDReader&& reader)                 = delete;
            DCDReader& operator=(const DCDReader& reader) = delete;

            ~DCDReader() = default;

            void read_file();
            void read_file(const std::string& fname);

            int get_nset()       const {return nset;}
            int get_istart()     const {return istart;}
            int get_nstep_save() const {return nstep_save;}
            int get_nstep()      const {return nstep;}
            int get_nunit()      const {return nunit;}
            int get_npart()      const {return nparticle;}
            double get_delta_t() const {return delta_t;}

            std::pair<SnapShot, double> get_snapshot(const std::size_t i) const
            {
                return std::make_pair(data.at(i), delta_t * i);
            }

            bool is_file_specified()          const {return filename.empty();}
            bool is_data_empty()              const {return data.empty();}
            std::size_t get_size()            const {return data.size();}
            SnapShot  at(const std::size_t i) const {return data.at(i);}
            SnapShot& at(const std::size_t i)       {return data.at(i);}
            SnapShot  operator[](const std::size_t i) const {return data[i];}
            SnapShot& operator[](const std::size_t i)       {return data[i];}
            iterator begin(){return data.begin();}
            iterator end(){return data.end();}
            const_iterator cbegin() const {return data.cbegin();}
            const_iterator cend() const {return data.cend();}

            std::vector<SnapShot>& get_all_data(){return data;}

        private:

            void read_header();
            void read_core();
            void read_head_block1();
            void read_head_block2();
            void read_head_block3();
            std::vector<double> read_coord();

        private:

            // These values are coded in files, not calculated value
            int nset;       // number of frame(a.k.a. SnapShot)
            int istart;     // initial value of isteps (nomally zero)
            int nstep_save; // nstep_save timesteps per one snapshot
            int nstep;      // total step number of the simulation
            int nunit;      // total unit number (chains??)
            int verCHARMM;  // version of CHARMM (usually unused)
            int nparticle;  // total number of particle
            double delta_t;

            std::string filename;
            std::ifstream dcdfile;
            std::vector<std::string> header; // comments in header block 2
            std::vector<SnapShot> data;      // trajectory data

        private:

            //XXX: size_value depends on the environment that the simulation
            //     runs, not on the environment where you analysis the dcd file.
            //     so these size value perhaps differ from the true value.
            static constexpr std::size_t size_int = sizeof(int);
            static constexpr std::size_t size_float = sizeof(float);
            static constexpr std::size_t size_char = sizeof(char);
    };
    typedef std::shared_ptr<DCDReader> DCDReaderSptr;

    void DCDReader::read_file(const std::string& fname)
    {
        if(filename.empty())
        {
            filename = fname;
        }
        else
        {
            std::cout << "Warning: DCDReader: you already specify filename. "
                      << filename << " but now you are trying to open "
                      << fname << ", so now open " << fname
                      << std::endl;
            filename = fname;
        }
        read_file();
        return;
    }

    void DCDReader::read_file()
    {
        if(filename.empty())
        {
            throw std::invalid_argument("filename is not specified!");
        }
        else if(!dcdfile.is_open())
        {
            dcdfile.open(filename, std::ios::binary);
            if(dcdfile.fail())
            {
                throw std::invalid_argument("file open error: " + filename);
            }
        }
        else
        {
            throw std::logic_error(
                    "filename is not specified but file is open!?");
        }
        read_header();
        read_core();
        dcdfile.close();
        std::cout << "Info   : dcd file(" << filename
                  <<  ") reading completed" << std::endl;
        return;
    }

    // necessary?
    void DCDReader::read_header()
    {
        read_head_block1();
        read_head_block2();
        read_head_block3();
        return;
    }

    void DCDReader::read_head_block1()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int byte = *reinterpret_cast<int*>(cbytes);
        if(byte != 84)
        {
            std::cout << "Warning: this file may not be cafemol output."
                      << std::endl;
            std::cout << "       : header block 1 : " << byte << "bytes"
                      << std::endl;
        }
        delete [] cbytes;

        char *ccord = new char [5];
        dcdfile.read(ccord, 4);
        ccord[4] = '\0';
        std::string cord(ccord);
        delete [] ccord;

        if(cord == "CORD")
        {
            std::cout << "Info   : file signature is CORD. normal dcd file."
                      << std::endl;
        }
        else if(cord == "VELD")
        {
            std::cout << "Info   : file signature is VELD. normal vdcd file."
                      << std::endl;
        }
        else
        {
            std::cout << "Error  : unknown signature : " << cord << std::endl;
            throw std::invalid_argument("Unknown File");
        }

        char *cnset = new char[size_int];
        dcdfile.read(cnset, size_int);
        nset = *reinterpret_cast<int*>(cnset);
        delete [] cnset;

        std::cout << "Info   : nset = " << nset << std::endl;
        
        char *cistart = new char[size_int];
        dcdfile.read(cistart, size_int);
        istart = *reinterpret_cast<int*>(cistart);
        delete [] cistart;

        std::cout << "Info   : istart = " << istart << std::endl;

        char *cnstep_save = new char[size_int];
        dcdfile.read(cnstep_save, size_int);
        nstep_save = *reinterpret_cast<int*>(cnstep_save);
        delete [] cnstep_save;

        std::cout << "Info   : nstep_save = " << nstep_save << std::endl;

        char *cnstep = new char[size_int];
        dcdfile.read(cnstep, size_int);
        nstep = *reinterpret_cast<int*>(cnstep);
        delete [] cnstep;

        std::cout << "Info   : nstep = " << nstep << std::endl;

        char *cnunit = new char[size_int];
        dcdfile.read(cnunit, size_int);
        nunit = *reinterpret_cast<int*>(cnunit);
        delete [] cnunit;

        std::cout << "Info   : nunit = " << nunit << std::endl;

        char *null_1 = new char[16];
        dcdfile.read(null_1, 16);
        delete [] null_1;
        
        char *cdelta = new char[size_float];
        dcdfile.read(cdelta, size_float);
        delta_t = static_cast<double>(*reinterpret_cast<float*>(cdelta));
        delete [] cdelta;

        std::cout << "Info   : delta t = " << delta_t << std::endl;

        char *null_2 = new char[36];
        dcdfile.read(null_2, 36);
        delete [] null_2;

        char *cverCHARMM = new char[size_int];
        dcdfile.read(cverCHARMM, size_int);
        verCHARMM = *reinterpret_cast<int*>(cverCHARMM);
        delete [] cverCHARMM;

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(byte != *reinterpret_cast<int*>(cbytes_f))
        {
            throw std::invalid_argument(
                    "header block1 has invalid size information");
        }
        delete [] cbytes_f;

        return;
    }

    void DCDReader::read_head_block2()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes = *reinterpret_cast<int*>(cbytes);
        delete [] cbytes;
        
        char *clines = new char[size_int];
        dcdfile.read(clines, size_int);
        int lines = *reinterpret_cast<int*>(clines);
        delete [] clines;

        if((80 * lines + 4) != bytes)
        {
            std::cout << "Error  : header block2 size = "
                      << bytes << " bytes."
                      << " But block2 has " << lines
                      << "lines" << std::endl;
            throw std::invalid_argument("header block2 has invalid size");
        }

        std::cout << "Info   : dcd file header begin" << std::endl;
        for(int i(0); i<lines; ++i)
        {
            char *line = new char[81];
            dcdfile.read(line, 80);
            line[80] = '\0';
            std::cout << line << std::endl;
            header.push_back(std::string(line));
            delete [] line;
        }
        std::cout << "Info   : header end" << std::endl;

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            throw std::invalid_argument(
                    "header block2 has invalid size information");
        }
        delete [] cbytes_f;
    }

    void DCDReader::read_head_block3()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes = *reinterpret_cast<int*>(cbytes);
        delete [] cbytes;
        
        char *cnpart = new char[size_int];
        dcdfile.read(cnpart, size_int);
        nparticle = *reinterpret_cast<int*>(cnpart);
        delete [] cnpart;

        std::cout << "Info   : there are " << nparticle
                  << " particles in this file" << std::endl;

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            throw std::invalid_argument(
                    "header block3 has invalid size information");
        }
        delete [] cbytes_f;
        return;
    }

    //read all of the coordinate blocks of the trajectory coded in the file
    void DCDReader::read_core()
    {
        data.reserve(nset);

        for(int i(0); i<=nset; ++i)
        {
            std::vector<double> x(read_coord());
            std::vector<double> y(read_coord());
            std::vector<double> z(read_coord());
            if(static_cast<std::size_t>(nparticle) != x.size() ||
               static_cast<std::size_t>(nparticle) != y.size() ||
               static_cast<std::size_t>(nparticle) != z.size())
            {
                std::cout << "Warning: number of values of coordinate differs"
                          << " from nparticle" << std::endl;
                throw std::invalid_argument("error in reading property");
            }

            SnapShot temp_snapshot(nparticle);
            for(int c(0); c < nparticle; ++c)
            {
                temp_snapshot[c] = Realvec(x[c], y[c], z[c]);
            }
            data.push_back(temp_snapshot);
        }

        return;
    }

    std::vector<double> DCDReader::read_coord()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes(*reinterpret_cast<int*>(cbytes));
        if(bytes / size_float != nparticle)
        {
            std::cout << "Error  : "
                      << "dcd file coordinate has "
                      << bytes << " bytes but nparticle is"
                      << nparticle << ", and sizeof float is "
                      << size_float << std::endl;
            throw std::invalid_argument(
                    "dcd: coordinate block has invalid byte-information");
        }
        delete [] cbytes;

        std::vector<double> coordinate(nparticle);
        for(int i(0); i<nparticle; ++i)
        {
            char *cfloat = new char[size_float];
            dcdfile.read(cfloat, size_float);
            coordinate[i] =
                static_cast<double>(*reinterpret_cast<float*>(cfloat));
            delete [] cfloat;
        }

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            std::cout << "Error  : "
                      << "dcd file coordinate header-byte-information "
                      << "is not same as footer-byte-information" <<std::endl;
            throw std::invalid_argument(
                    "dcd coordinate block has invalid byte-information");
        }
        delete [] cbytes_f;

        return coordinate;
    }

}//end namespace coffeemill

#endif //COFFEE_MILL_DCD_READER
