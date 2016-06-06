#include "DCDReader.hpp"
#include <iostream>
#include <fstream>

namespace coffeemill
{

void DCDReader::read(const std::string& fname)
{
    filename_ = fname;
    this->read();
    return;
}

void DCDReader::read()
{
    if(filename_.empty())
    {
        throw std::invalid_argument("filename is not specified");
    }

    std::ifstream dcdfile(filename_, std::ios::binary);

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : DCD file reading start" << std::endl;
#endif

    dcdfile.seekg(0, dcdfile.end);
    this->file_size = dcdfile.tellg();
    dcdfile.seekg(0, dcdfile.beg);

    read_header(dcdfile);

    // if file size is incorrect, warn and redefine the number of snapshot
    if(!validate_filesize())
    {
        std::cerr << "Warning: filesize is not correct! total filesize is "
                  << this->file_size << " [byte]" << std::endl;
        std::cerr << "       : but file says there are "
                  << this->data_.nset()
                  << " snapshots. number of containing particles is "
                  << this->data_.nparticle() <<std::endl;
        std::cerr << "       : header block 1 size : " << header1_size << std::endl;
        std::cerr << "       : header block 2 size : " << header2_size << std::endl;
        std::cerr << "       : header block 3 size : " << header3_size << std::endl;
        std::cerr << "       : so this file must have "
                  << (header1_size + header2_size + header3_size +
                     snapshot_size * this->data_.nset()) << " bytes." << std::endl;

        if((this->file_size - header1_size - header2_size - header3_size)
                % snapshot_size == 0)
        {
            this->data_.nset() =
                (file_size - header1_size - header2_size - header3_size)
                / snapshot_size;
            std::cerr << "       : guess snapshot size as "
                      << this->data_.nset() << std::endl;
        }
        else
        {
            throw std::invalid_argument("invalid dcd file size");
        }
    }

    read_core(dcdfile);
    dcdfile.close();

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : dcd file(" << filename_
              <<  ") reading completed" << std::endl;
#endif

    return;
}

void DCDReader::read_header(const std::string& filename)
{
    filename_ = filename;
    return this->read_header();
}

void DCDReader::read_header()
{
    std::ifstream dcdfile(filename_, std::ios::binary);
    read_header(dcdfile);
    dcdfile.close();
    return;
}

void DCDReader::read_header(std::ifstream& dcdfile)
{
    read_head_block1(dcdfile);
    read_head_block2(dcdfile);
    read_head_block3(dcdfile);
    return;
}

bool DCDReader::validate_filesize()
{
    snapshot_size = 12 * (this->data_.nparticle() + 2);

    return file_size == (header1_size + header2_size + header3_size +
                        snapshot_size * this->data_.nset());
}

void DCDReader::read_head_block1(std::ifstream& dcdfile)
{
    char *cbytes = new char[size_int];
    dcdfile.read(cbytes, size_int);
    int byte = *reinterpret_cast<int*>(cbytes);
    if(byte != 84)
    {
        std::cerr << "Warning: this file may not be cafemol output."
                  << std::endl;
        std::cerr << "       : header block 1 : " << byte << "bytes"
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
#ifdef COFFEE_MILL_DEBUG
        std::cerr << "Info   : file signature is CORD. normal dcd file."
                  << std::endl;
#endif
    }
    else if(cord == "VELD")
    {
#ifdef COFFEE_MILL_DEBUG
        std::cerr << "Info   : file signature is VELD. normal vdcd file."
                  << std::endl;
#endif
    }
    else
    {
        std::cout << "Error  : unknown signature : " << cord << std::endl;
        throw std::invalid_argument("Unknown File Signeture");
    }

    char *cnset = new char[size_int];
    dcdfile.read(cnset, size_int);
    this->data_.nset() = *reinterpret_cast<int*>(cnset);
    delete [] cnset;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nset = " << this->data_.nset() << std::endl;
#endif
    
    char *cistart = new char[size_int];
    dcdfile.read(cistart, size_int);
    this->data_.istart() = *reinterpret_cast<int*>(cistart);
    delete [] cistart;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : istart = " << this->data_.istart() << std::endl;
#endif

    char *cnstep_save = new char[size_int];
    dcdfile.read(cnstep_save, size_int);
    this->data_.nstep_save() = *reinterpret_cast<int*>(cnstep_save);
    delete [] cnstep_save;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nstep_save = " << this->data_.nstep_save() << std::endl;
#endif

    char *cnstep = new char[size_int];
    dcdfile.read(cnstep, size_int);
    this->data_.nstep() = *reinterpret_cast<int*>(cnstep);
    delete [] cnstep;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nstep = " << this->data_.nstep() << std::endl;
#endif

    char *cnunit = new char[size_int];
    dcdfile.read(cnunit, size_int);
    this->data_.nunit() = *reinterpret_cast<int*>(cnunit);
    delete [] cnunit;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nunit = " << this->data_.nunit() << std::endl;
#endif

    char *null_1 = new char[16];
    dcdfile.read(null_1, 16);
    delete [] null_1;
    
    char *cdelta = new char[size_float];
    dcdfile.read(cdelta, size_float);
    this->data_.delta_t() = static_cast<double>(*reinterpret_cast<float*>(cdelta));
    delete [] cdelta;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : delta t = " << this->data_.delta_t() << std::endl;
#endif

    char *null_2 = new char[36];
    dcdfile.read(null_2, 36);
    delete [] null_2;

    char *cverCHARMM = new char[size_int];
    dcdfile.read(cverCHARMM, size_int);
    this->data_.verCHARMM() = *reinterpret_cast<int*>(cverCHARMM);
    delete [] cverCHARMM;

    char *cbytes_f = new char[size_int];
    dcdfile.read(cbytes_f, size_int);
    if(byte != *reinterpret_cast<int*>(cbytes_f))
    {
        throw std::invalid_argument(
                "header block1 has invalid size information");
    }
    delete [] cbytes_f;

    this->header1_size = byte + size_int * 2;
    return;
}

void DCDReader::read_head_block2(std::ifstream& dcdfile)
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
        std::cerr << "Error  : header block2 size = "
                  << bytes << " bytes."
                  << " But block2 has " << lines
                  << "lines" << std::endl;
        throw std::invalid_argument("header block2 has invalid size");
    }

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : dcd file header begin" << std::endl;
#endif

    for(int i(0); i<lines; ++i)
    {
        char *line = new char[81];
        dcdfile.read(line, 80);
        line[80] = '\0';
#ifdef COFFEE_MILL_DEBUG
        std::cerr << line << std::endl;
#endif
        this->data_.header().push_back(std::string(line));
        delete [] line;
    }

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : header end" << std::endl;
#endif

    char *cbytes_f = new char[size_int];
    dcdfile.read(cbytes_f, size_int);
    if(bytes != *reinterpret_cast<int*>(cbytes_f))
    {
        throw std::invalid_argument(
                "header block2 has invalid size information");
    }
    delete [] cbytes_f;

    header2_size = bytes + size_int * 2;
    return;
}

void DCDReader::read_head_block3(std::ifstream& dcdfile)
{
    char *cbytes = new char[size_int];
    dcdfile.read(cbytes, size_int);
    int bytes = *reinterpret_cast<int*>(cbytes);
    delete [] cbytes;
    
    char *cnpart = new char[size_int];
    dcdfile.read(cnpart, size_int);
    this->data_.nparticle() = *reinterpret_cast<int*>(cnpart);
    delete [] cnpart;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : there are " << this->data_.nparticle()
              << " particles in this file" << std::endl;
#endif

    char *cbytes_f = new char[size_int];
    dcdfile.read(cbytes_f, size_int);
    if(bytes != *reinterpret_cast<int*>(cbytes_f))
    {
        throw std::invalid_argument(
                "header block3 has invalid size information");
    }
    delete [] cbytes_f;

    header3_size = bytes + size_int * 2;
    return;
}

//read all of the coordinate blocks of the trajectory coded in the file
void DCDReader::read_core(std::ifstream& dcdfile)
{
    this->data_.traj().reserve(this->data_.nset());

    for(std::size_t i(0); i<this->data_.nset(); ++i)
    {
        std::vector<double> x(read_coord(dcdfile));
        std::vector<double> y(read_coord(dcdfile));
        std::vector<double> z(read_coord(dcdfile));
        if(static_cast<std::size_t>(this->data_.nparticle()) != x.size() ||
           static_cast<std::size_t>(this->data_.nparticle()) != y.size() ||
           static_cast<std::size_t>(this->data_.nparticle()) != z.size())
        {
            std::cout << "Warning: number of values of coordinate differs"
                      << " from nparticle" << std::endl;
            throw std::invalid_argument("error in reading property");
        }

        DCDData::snapshot_type temp_snapshot(this->data_.nparticle());
        for(std::size_t c(0); c < this->data_.nparticle(); ++c)
        {
            temp_snapshot[c] = DCDData::position_type(x[c], y[c], z[c]);
        }
        data_.traj().push_back(temp_snapshot);
    }

    return;
}

std::vector<double> DCDReader::read_coord(std::ifstream& dcdfile)
{
    char *cbytes = new char[size_int];
    dcdfile.read(cbytes, size_int);
    int bytes(*reinterpret_cast<int*>(cbytes));
    if(bytes / size_float != this->data_.nparticle())
    {
        std::cout << "Error  : "
                  << "dcd file coordinate has "
                  << bytes << " bytes but nparticle is"
                  << this->data_.nparticle() << ", and sizeof float is "
                  << size_float << std::endl;
        throw std::invalid_argument(
                "dcd: coordinate block has invalid byte-information");
    }
    delete [] cbytes;

    std::vector<double> coordinate(this->data_.nparticle());
    for(std::size_t i(0); i<this->data_.nparticle(); ++i)
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


}// coffeemill
