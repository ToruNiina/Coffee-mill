#ifndef COFFEE_MILL_DCD_READER
#define COFFEE_MILL_DCD_READER
#include <memory>
#include <utility>
#include "DCDDef.hpp"

namespace coffeemill
{
    class DCDReader
    {
        public:
            DCDReader(){}
            DCDReader(const std::string& filename)
            : dcdfile(filename, std::ios::binary)
            {
                if(dcdfile.fail())
                {
                    throw std::invalid_argument("file open error");
                }
            }
            ~DCDReader()
            {
                dcdfile.close();
            }

            void read_file();
            void read_file(const std::string& filename);

            int get_nset() const {return nset;}
            int get_istart() const {return istart;}
            int get_nstep_save() const {return nstep_save;}
            int get_nstep() const {return nstep;}
            int get_nunit() const {return nunit;}
            int get_npart() const {return nparticle;}
            double get_delta_t() const {return delta_t;}

            std::pair<SnapShot, double> get_snapshot(int i)
            {return std::make_pair(data.at(i), delta_t * i);}

            int get_size() const {return data.size();}
            SnapShot& at(int i){return data.at(i);}

            std::vector<SnapShot>& get_all_data(){return data;}

        private:

            void read_header();
            void read_core();
            void read_head_block1();
            void read_head_block2();
            void read_head_block3();
            std::vector<double> read_x();
            std::vector<double> read_y();
            std::vector<double> read_z();

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
            std::ifstream dcdfile;

        private:

            static const int size_int;
            static const int size_float;
            static const int size_char;
    };

    const int DCDReader::size_int = sizeof(int);
    const int DCDReader::size_float = sizeof(float);
    const int DCDReader::size_char = sizeof(char);

    void DCDReader::read_file(const std::string& filename)
    {
        if(dcdfile.is_open())
        {
            std::cout << "Warning: DCDReader already open a dcd file. "
                      << std::endl;
        }
        else
        {
            dcdfile.open(filename, std::ios::binary);
            if(dcdfile.fail())
            {
                throw std::invalid_argument("file open error");
            }
        }
        read_file();
        return;
    }

    void DCDReader::read_file()
    {
        if(dcdfile.fail())
        {
            throw std::invalid_argument("file open error");
        }
        read_header();
        read_core();
        std::cout << "Info   : read_file completed" << std::endl;
        return;
    }

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
            std::cout << "this file may not be cafemol output." << std::endl;
            std::cout << "header block 1 : " << byte << "bytes" << std::endl;
        }
        delete [] cbytes;

        char *ccord = new char [4];
        dcdfile.read(ccord, 4);
        std::string cord(ccord);
        if(cord == "CORD")
        {
            std::cout << "Message: start to read dcd file." << std::endl;
        }
        else if(cord == "VELD")
        {
            std::cout << "Message: start to read vdcd file." << std::endl;
        }
        else
        {
            std::cout << "Warning: unknown file : " << cord << std::endl;
        }
        delete [] ccord;

        char *cnset = new char[size_int];
        dcdfile.read(cnset, size_int);
        nset = *reinterpret_cast<int*>(cnset);
        delete [] cnset;
        
        char *cistart = new char[size_int];
        dcdfile.read(cistart, size_int);
        istart = *reinterpret_cast<int*>(cistart);
        delete [] cistart;

        char *cnstep_save = new char[size_int];
        dcdfile.read(cnstep_save, size_int);
        nstep_save = *reinterpret_cast<int*>(cnstep_save);
        delete [] cnstep_save;

        std::cout << "nstep_save: " << nstep_save << std::endl;

        char *cnstep = new char[size_int];
        dcdfile.read(cnstep, size_int);
        nstep = *reinterpret_cast<int*>(cnstep);
        delete [] cnstep;

        std::cout << "Info   : nstep: " << nstep << std::endl;
        std::cout << "Info   : number of saved snapshot: "
                  << nstep / nstep_save << std::endl;

        char *cnunit = new char[size_int];
        dcdfile.read(cnunit, size_int);
        nunit = *reinterpret_cast<int*>(cnunit);
        delete [] cnunit;

        char *null_1 = new char[16];
        dcdfile.read(null_1, 16);
        delete [] null_1;
        
        char *cdelta = new char[size_int];
        dcdfile.read(cdelta, size_int);
        float fdelta = *(float*)(cnunit);
        delta_t = static_cast<double>(fdelta);
        delete [] cdelta;

        std::cout << "Info   : delta t: " << delta_t << std::endl;

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
        delete cbytes_f;
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
            std::cout << "Warning: header block2 size = "
                      << bytes << "bytes." << std::endl;
            std::cout << "         but block2 has " << lines
                      << "lines" << std::endl;
            throw std::invalid_argument("header block2 has invalid size");
        }

        std::cout << "header: " << std::endl;
        for(int i(0); i<lines; ++i)
        {
            char *line = new char[80];
            dcdfile.read(line, 80);
            std::cout << line << std::endl;
            delete [] line;
        }
        std::cout << "header end" << std::endl;

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

    void DCDReader::read_core()
    {
        int saved(nstep / nstep_save);
        data.reserve(saved);

        for(int i(0); i<=saved; ++i)
        {
            std::vector<double> x(read_x());
            std::vector<double> y(read_y());
            std::vector<double> z(read_z());

            SnapShot temp_snapshot(nparticle);
            for(int c(0); c != x.size(); ++c)
            {
                Eigen::Vector3d coord(x.at(c), y.at(c), z.at(c));
                temp_snapshot.at(c) = coord;
            }

            data.push_back(temp_snapshot);
        }

        return;
    }

    std::vector<double> DCDReader::read_x()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes(*reinterpret_cast<int*>(cbytes));
        if(bytes / size_float != nparticle)
        {
            std::cout << "Warning: "
                      << "dcd file x coordinate has "
                      << bytes << " bytes but nparticle is"
                      << nparticle << ", and sizeof float is "
                      << size_float
                      << std::endl;
            throw std::invalid_argument(
                    "x coordinate block has invalid byte-information");
        }
        delete [] cbytes;

        std::vector<double> retval(nparticle);
        for(int i(0); i<nparticle; ++i)
        {
            char *cfloat = new char[size_float];
            dcdfile.read(cfloat, size_float);
            double x(static_cast<double>(*reinterpret_cast<float*>(cfloat)));
            retval.at(i) = x;
            delete [] cfloat;
        }

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            std::cout << "Warning: "
                      << "dcd file x coordinate header-byte-information "
                      << "is not same number as footer-byte-information"
                      <<std::endl;
            throw std::invalid_argument("x block has invalid byte-information");
        }
        delete [] cbytes_f;

        return retval;
    }

    std::vector<double> DCDReader::read_y()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes(*reinterpret_cast<int*>(cbytes));
        if(bytes / size_float != nparticle)
        {
            std::cout << "Warning: "
                      << "dcd file y coordinate has "
                      << bytes << " bytes but nparticle is "
                      << nparticle << ", and sizeof(float) is"
                      << size_float
                      << std::endl;
            throw std::invalid_argument(
                    "y coordinate block has invalid byte-information");
        }
        delete [] cbytes;

        std::vector<double> retval(nparticle);
        for(int i(0); i<nparticle; ++i)
        {
            char *cfloat = new char[size_float];
            dcdfile.read(cfloat, size_float);
            double y(static_cast<double>(*reinterpret_cast<float*>(cfloat)));
            retval.at(i) = y;
            delete [] cfloat;
        }

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            std::cout << "Warning: "
                      << "dcd file y coordinate header-byte-information "
                      << "is not same number as footer-byte-information"
                      <<std::endl;
            throw std::invalid_argument("y block has invalid byte-information");
        }
        delete [] cbytes_f;

        return retval;
    }

    std::vector<double> DCDReader::read_z()
    {
        char *cbytes = new char[size_int];
        dcdfile.read(cbytes, size_int);
        int bytes(*reinterpret_cast<int*>(cbytes));
        if(bytes / size_float != nparticle)
        {
            std::cout << "Warning: "
                      << "dcd file z coordinate has "
                      << bytes << "bytes but nparticle is"
                      << nparticle << ", and sizeof(float) is"
                      << size_float
                      << std::endl;
            throw std::invalid_argument(
                    "z coordinate block has invalid byte-information");
        }
        delete [] cbytes;

        std::vector<double> retval(nparticle);
        for(int i(0); i < nparticle; ++i)
        {
            char *cfloat = new char[size_float];
            dcdfile.read(cfloat, size_float);
            double z(static_cast<double>(*reinterpret_cast<float*>(cfloat)));
            retval.at(i) = z;
            delete [] cfloat;
        }

        char *cbytes_f = new char[size_int];
        dcdfile.read(cbytes_f, size_int);
        if(bytes != *reinterpret_cast<int*>(cbytes_f))
        {
            std::cout << "Warning: "
                      << "dcd file z coordinate header-byte-information "
                      << "is not same number as footer-byte-information"
                      <<std::endl;
            throw std::invalid_argument("z block has invalid byte-information");
        }
        delete [] cbytes_f;

        return retval;
    }

    typedef std::shared_ptr<DCDReader> DCDRderSptr;

}//end namespace coffeemill

#endif //COFFEE_MILL_DCD_READER
