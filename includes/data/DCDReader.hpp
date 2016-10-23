/*!
  @file DCDReader.hpp
  @brief definition of a class that reads dcd file.

  DCDReader reads dcd file and store the information included in the file.
  the data is stored as DCDData. Reader can read only a header region.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_READER
#define COFFEE_MILL_DCD_READER
#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include "DCDData.hpp"

namespace mill
{

template <typename vectorT>
class DCDReader
{
  public:
    using vector_type   = vectorT;
    using data_type     = DCDData<vector_type>;
    using real_type     = typename data_type::real_type;
    using header_type   = typename data_type::header_type;
    using snapshot_type = typename data_type::snapshot_type;

  public:

    DCDReader(){}
    ~DCDReader() = default;

    // return full dcd data.
    data_type read(const std::string& fname);
    data_type read(std::istream& is);

    // return full header information and empty trajectory.
    data_type read_header(const std::string& fname);
    data_type read_header(std::istream& is);

    // return one snapshot from file(required having read the header).
    snapshot_type read_snapshot(const std::string& fname, const std::size_t i);
    snapshot_type read_snapshot(std::istream& is);

  private:

    bool validate_filesize(const data_type& dat, const std::size_t file_size);

    std::vector<float> read_coord(std::istream& dcdfile, const std::size_t i);
    void read_trajectory(std::istream& dcdfile, data_type& data);

    void read_header_block1(std::istream& dcdfile, data_type& data);
    void read_header_block2(std::istream& dcdfile, data_type& data);
    void read_header_block3(std::istream& dcdfile, data_type& data);

  private:

    // for filesize validation
    std::size_t header1_size;
    std::size_t header2_size;
    std::size_t header3_size;
    std::size_t snapshot_size;

    //XXX: size_value depends on the environment that the simulation
    //     runs, not on the environment where you analysis the dcd file.
    //     so these size value perhaps differ from the true value.
    constexpr static std::size_t size_int   = sizeof(int);
    constexpr static std::size_t size_float = sizeof(float);
    constexpr static std::size_t size_char  = sizeof(char);
};

template <typename T>
typename DCDReader<T>::data_type
DCDReader<T>::read(const std::string& fname)
{
    std::ifstream ifs(fname, std::ios::in | std::ios::binary);
    if(not ifs.good())
        throw std::runtime_error("DCDReader: file open error" + fname);
    auto retval = this->read(ifs);
    ifs.close();
    return retval;
}

template <typename T>
typename DCDReader<T>::data_type
DCDReader<T>::read(std::istream& is)
{
#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : DCD file reading start" << std::endl;
#endif

    is.seekg(0, is.end);
    const std::size_t file_size = is.tellg();
    is.seekg(0, is.beg);

    data_type data = read_header(is);

    // if file size is incorrect, warn and redefine the number of snapshot
    if(!validate_filesize(data, file_size))
    {
        std::cerr << "Warning: filesize is not correct! total filesize is "
                  << file_size << " [byte]" << std::endl;
        std::cerr << "       : but file says there are " << data.nset()
                  << " snapshots. number of containing particles is "
                  << data.nparticle() <<std::endl;
        std::cerr << "       : header block 1 size : " << header1_size << std::endl;
        std::cerr << "       : header block 2 size : " << header2_size << std::endl;
        std::cerr << "       : header block 3 size : " << header3_size << std::endl;
        std::cerr << "       : so this file must have "
                  << (header1_size + header2_size + header3_size +
                     snapshot_size * data.nset()) << " bytes." << std::endl;

        if((file_size - header1_size - header2_size - header3_size)
                % snapshot_size != 0)
            std::cerr << "       : probably the last snapshot is imcomplete." << std::endl;

        data.nset() =
            (file_size - header1_size - header2_size - header3_size)
            / snapshot_size;
        std::cerr << "       : guess snapshot size as " << data.nset() << std::endl;
    }

    read_trajectory(is, data);

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : reading dcd file  completed" << std::endl;
#endif

    return data;
}

template <typename vectorT>
typename DCDReader<vectorT>::data_type
DCDReader<vectorT>::read_header(const std::string& filename)
{
    std::ifstream ifs(filename, std::ios::binary);
    if(not ifs.good())
        throw std::runtime_error("DCDReader file open error " + filename);
    return this->read_header(ifs);
}

template <typename T>
typename DCDReader<T>::data_type
DCDReader<T>::read_header(std::istream& is)
{
    data_type dat;
    this->read_header_block1(is, dat);
    this->read_header_block2(is, dat);
    this->read_header_block3(is, dat);
    return dat;
}

template <typename T>
bool DCDReader<T>::validate_filesize(
        const data_type& data, std::size_t file_size)
{
    this->snapshot_size = 12 * (data.nparticle() + 2);

    return file_size == (header1_size + header2_size + header3_size +
                        snapshot_size * data.nset());
}

template <typename T>
void DCDReader<T>::read_header_block1(std::istream& dcdfile, data_type& data)
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

    char *csigneture = new char [5];
    dcdfile.read(csigneture, 4);
    csigneture[4] = '\0';
    std::string signeture(csigneture);
    delete [] csigneture;

    if(signeture == "CORD")
    {
#ifdef COFFEE_MILL_DEBUG
        std::cerr << "Info   : file signature is CORD. normal dcd file."
                  << std::endl;
#endif
        ;
    }
    else if(signeture == "VELD")
    {
#ifdef COFFEE_MILL_DEBUG
        std::cerr << "Info   : file signature is VELD. normal vdcd file."
                  << std::endl;
#endif
        ;
    }
    else
    {
        throw std::invalid_argument("Unknown File Signeture: " + signeture);
    }
    data.signeture() = signeture;

    char *cnset = new char[size_int];
    dcdfile.read(cnset, size_int);
    data.nset() = *reinterpret_cast<int*>(cnset);
    delete [] cnset;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nset = " << data.nset() << std::endl;
#endif
    
    char *cistart = new char[size_int];
    dcdfile.read(cistart, size_int);
    data.istart() = *reinterpret_cast<int*>(cistart);
    delete [] cistart;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : istart = " << data.istart() << std::endl;
#endif

    char *cnstep_save = new char[size_int];
    dcdfile.read(cnstep_save, size_int);
    data.nstep_save() = *reinterpret_cast<int*>(cnstep_save);
    delete [] cnstep_save;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nstep_save = " << data.nstep_save() << std::endl;
#endif

    char *cnstep = new char[size_int];
    dcdfile.read(cnstep, size_int);
    data.nstep() = *reinterpret_cast<int*>(cnstep);
    delete [] cnstep;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nstep = " << data.nstep() << std::endl;
#endif

    char *cnunit = new char[size_int];
    dcdfile.read(cnunit, size_int);
    data.nunit() = *reinterpret_cast<int*>(cnunit);
    delete [] cnunit;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nunit = " << data.nunit() << std::endl;
#endif

    char *null_1 = new char[16];
    dcdfile.read(null_1, 16);
    delete [] null_1;
    
    char *cdelta = new char[size_float];
    dcdfile.read(cdelta, size_float);
    data.delta_t() = *reinterpret_cast<float*>(cdelta);
    delete [] cdelta;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : delta t = " << data.delta_t() << std::endl;
#endif

    char *null_2 = new char[36];
    dcdfile.read(null_2, 36);
    delete [] null_2;

    char *cverCHARMM = new char[size_int];
    dcdfile.read(cverCHARMM, size_int);
    data.verCHARMM() = *reinterpret_cast<int*>(cverCHARMM);
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

template <typename T>
void DCDReader<T>::read_header_block2(std::istream& dcdfile, data_type& data)
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
        data.comment().push_back(std::string(line));
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

template <typename T>
void DCDReader<T>::read_header_block3(std::istream& dcdfile, data_type& data)
{
    char *cbytes = new char[size_int];
    dcdfile.read(cbytes, size_int);
    int bytes = *reinterpret_cast<int*>(cbytes);
    delete [] cbytes;
    
    char *cnpart = new char[size_int];
    dcdfile.read(cnpart, size_int);
    data.nparticle() = *reinterpret_cast<int*>(cnpart);
    delete [] cnpart;

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : there are " << data.nparticle()
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

template <typename T>
void DCDReader<T>::read_trajectory(
        std::istream& is, data_type& data)
{
    data.traj().clear();
    data.traj().reserve(data.nset());

    for(std::size_t i(0); i<data.nset(); ++i)
    {
        const std::vector<float> x(read_coord(is, data.nparticle()));
        const std::vector<float> y(read_coord(is, data.nparticle()));
        const std::vector<float> z(read_coord(is, data.nparticle()));

        snapshot_type temp_snapshot(data.nparticle());
        for(std::size_t c(0); c < data.nparticle(); ++c)
        {
            const typename data_type::position_type pos(x[c], y[c], z[c]);
            temp_snapshot[c] = pos;
        }
        data.traj().push_back(temp_snapshot);
    }

    return;
}

template <typename T>
std::vector<float>
DCDReader<T>::read_coord(std::istream& is, const std::size_t nparticle)
{
    char *csize_of_block1 = new char[size_int];
    is.read(csize_of_block1, size_int);
    int size_of_block(*reinterpret_cast<int*>(csize_of_block1));
    delete [] csize_of_block1;

    if(size_of_block / size_float != nparticle)
        throw std::invalid_argument(
                "dcd coordinate block size differs from nparticle");

    std::vector<float> coordinate(nparticle);
    for(std::size_t i(0); i<nparticle; ++i)
    {
        char *cfloat = new char[size_float];
        is.read(cfloat, size_float);
        coordinate[i] = *reinterpret_cast<float*>(cfloat);
        delete [] cfloat;
    }

    char *csize_of_block_f = new char[size_int];
    is.read(csize_of_block_f, size_int);
    if(size_of_block != *reinterpret_cast<int*>(csize_of_block_f))
        throw std::invalid_argument(
                "dcd coordinate block has invalid byte-information");
    delete [] csize_of_block_f;

    return coordinate;
}


}// coffeemill

#endif //COFFEE_MILL_DCD_READER
