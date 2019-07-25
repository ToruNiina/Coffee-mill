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

//! read DCDfile and return the data as a DCDData.
/*!
 *  @tparam vectorT type of position
 */
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

    //! read all of the file and return full dcd data.
    data_type read(const std::string& fname);
    data_type read(std::istream& is);

    //! return full header information and empty trajectory.
    data_type read_header(const std::string& fname);
    data_type read_header(std::istream& is);

    //! read only i-th snapshot from file.
    snapshot_type read_snapshot(const std::string& fname, const std::size_t i);
    //! read next snapshot (required having read the header).
    snapshot_type read_snapshot(std::istream& is, const header_type& header);

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
    {
        throw std::runtime_error("DCDReader: file open error" + fname);
    }
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

    data_type data = read_header(is);

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
    {
        throw std::runtime_error("DCDReader file open error " + filename);
    }
    return this->read_header(ifs);
}

template <typename T>
typename DCDReader<T>::data_type
DCDReader<T>::read_header(std::istream& is)
{
    is.seekg(0, is.end);
    const std::size_t file_size = is.tellg();
    is.seekg(0, is.beg);

    data_type dat;
    this->read_header_block1(is, dat);
    this->read_header_block2(is, dat);
    this->read_header_block3(is, dat);

    // if file size is incorrect, warn and redefine the number of snapshot
    if(!validate_filesize(dat, file_size))
    {
        std::cerr << "Warning: filesize is not correct! total filesize is "
                  << file_size << " [byte]" << '\n';
        std::cerr << "       : but file says there are " << dat.nset()
                  << " snapshots. number of containing particles is "
                  << dat.nparticle() << '\n';
        std::cerr << "       : header block 1 size : " << header1_size << '\n';
        std::cerr << "       : header block 2 size : " << header2_size << '\n';
        std::cerr << "       : header block 3 size : " << header3_size << '\n';
        std::cerr << "       : so this file must have "
                  << (header1_size + header2_size + header3_size +
                     snapshot_size * dat.nset()) << " bytes." << '\n';

        if((file_size - header1_size - header2_size - header3_size)
                % snapshot_size != 0)
        {
            std::cerr << "       : probably the last snapshot is imcomplete.\n";
        }

        dat.nset() = (file_size - header1_size - header2_size - header3_size) /
                     snapshot_size;
        std::cerr << "       : guess snapshot size as " << dat.nset() << std::endl;
    }
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
    int byte;
    dcdfile.read(reinterpret_cast<char*>(byte), size_int);
    if(byte != 84)
    {
        std::cerr << "Warning: this file may not be cafemol output."
                  << std::endl;
        std::cerr << "       : header block 1 : " << byte << "bytes"
                  << std::endl;
    }

    char csigneture[5];
    dcdfile.read(csigneture, 4);
    csigneture[4] = '\0';
    std::string signeture(csigneture);

    if(signeture != "CORD" && signeture != "VELD")
    {
        throw std::invalid_argument("Unknown File Signeture: " + signeture);
    }

#ifdef COFFEE_MILL_DEBUG
    if(signeture == "CORD")
    {
        std::cerr << "Info   : file signature is CORD. normal dcd file.\n";
    }
    else if(signeture == "VELD")
    {
        std::cerr << "Info   : file signature is VELD. normal vdcd file.\n";
    }
#endif

    data.signeture() = signeture;

    dcdfile.read(reinterpret_cast<char*>(std::addressof(data.nset())), size_int);
#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nset = " << data.nset() << std::endl;
#endif
    
    dcdfile.read(reinterpret_cast<char*>(std::addressof(data.istart())), size_int);
#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : istart = " << data.istart() << std::endl;
#endif

    dcdfile.read(reinterpret_cast<char*>(std::addressof(data.nstep_save())), size_int);
#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nstep_save = " << data.nstep_save() << std::endl;
#endif

    dcdfile.read(reinterpret_cast<char*>(std::addressof(data.nstep())), size_int);
#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nstep = " << data.nstep() << std::endl;
#endif

    dcdfile.read(reinterpret_cast<char*>(std::addressof(data.nunit())), size_int);
#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : nunit = " << data.nunit() << std::endl;
#endif

    dcdfile.ignore(16);
    
    dcdfile.read(reinterpret_cast<char*>(std::addressof(data.delta_t())), size_float);
#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : delta t = " << data.delta_t() << std::endl;
#endif

    dcdfile.ignore(36);

    dcdfile.read(reinterpret_cast<char*>(std::addressof(data.verCHARMM())), size_int);

    int bytes_f;
    dcdfile.read(reinterpret_cast<char*>(std::addressof(bytes_f)), size_int);
    if(byte != bytes_f)
    {
        throw std::invalid_argument(
                "header block1 has invalid size information");
    }
    this->header1_size = byte + size_int * 2;
    return;
}

template <typename T>
void DCDReader<T>::read_header_block2(std::istream& dcdfile, data_type& data)
{
    int bytes;
    dcdfile.read(reinterpret_cast<char*>(std::addressof(bytes)), size_int);
    
    int lines;
    dcdfile.read(reinterpret_cast<char*>(std::addressof(lines)), size_int);
    if((80 * lines + 4) != bytes)
    {
        std::cerr << "Error  : header block2 size = " << bytes << " bytes."
                  << " But block2 has " << lines << "lines" << std::endl;
        throw std::invalid_argument("header block2 has invalid size");
    }

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : dcd file header begin" << std::endl;
#endif

    for(int i(0); i<lines; ++i)
    {
        char line[81];
        dcdfile.read(line, 80);
        line[80] = '\0';
#ifdef COFFEE_MILL_DEBUG
        std::cerr << line << std::endl;
#endif
        data.comment().push_back(std::string(line));
    }

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : header end" << std::endl;
#endif

    int bytes_f;
    dcdfile.read(reinterpret_cast<char*>(std::addressof(bytes_f)), size_int);
    if(bytes != bytes_f)
    {
        throw std::invalid_argument(
                "header block2 has invalid size information");
    }
    header2_size = bytes + size_int * 2;
    return;
}

template <typename T>
void DCDReader<T>::read_header_block3(std::istream& dcdfile, data_type& data)
{
    int bytes;
    dcdfile.read(reinterpret_cast<char*>(std::addressof(bytes)), size_int);
    dcdfile.read(reinterpret_cast<char*>(std::addressof(data.nparticle())), size_int);

#ifdef COFFEE_MILL_DEBUG
    std::cerr << "Info   : there are " << data.nparticle()
              << " particles in this file" << std::endl;
#endif

    int bytes_f;
    dcdfile.read(reinterpret_cast<char*>(std::addressof(bytes_f)), size_int);
    if(bytes != bytes_f)
    {
        throw std::invalid_argument(
                "header block3 has invalid size information");
    }

    header3_size = bytes + size_int * 2;
    return;
}

template <typename T>
void DCDReader<T>::read_trajectory(std::istream& is, data_type& data)
{
    using position_type = typename data_type::position_type;
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
            temp_snapshot[c] = position_type(x[c], y[c], z[c]);
        }
        data.traj().push_back(temp_snapshot);
    }

    return;
}

template <typename T>
typename DCDReader<T>::snapshot_type
DCDReader<T>::read_snapshot(std::istream& is, const header_type& header)
{
    using position_type = typename data_type::position_type;
    const std::vector<float> x(read_coord(is, header.nparticle));
    const std::vector<float> y(read_coord(is, header.nparticle));
    const std::vector<float> z(read_coord(is, header.nparticle));

    snapshot_type snap(header.nparticle);
    for(std::size_t c(0); c < header.nparticle; ++c)
    {
        snap[c] = position_type(x[c], y[c], z[c]);
    }
    return snap;
}

template <typename T>
typename DCDReader<T>::snapshot_type
DCDReader<T>::read_snapshot(const std::string& fname, const std::size_t i)
{
    std::ifstream ifs(fname, std::ios::in | std::ios::binary);
    if(not ifs.good())
    {
        throw std::runtime_error("DCDReader: file open error" + fname);
    }

    const auto header = this->read_header(ifs);
    const std::size_t npart = header.nparticle();
    const std::size_t snapsize = (npart + 2) * 3 * size_float;
    ifs.ignore(snapsize * (i-1));

    return this->read_snapshot(ifs, header);
}

template <typename T>
std::vector<float>
DCDReader<T>::read_coord(std::istream& is, const std::size_t nparticle)
{
    int size_of_block;
    is.read(reinterpret_cast<char*>(std::addressof(size_of_block)), size_int);
    if(size_of_block / size_float != nparticle)
    {
        std::cerr << "error: mill::DCDReader: size of block = " << size_of_block;
        std::cerr << ", nparticle = " << nparticle << '\n';
        throw std::invalid_argument(
                "dcd coordinate block size differs from nparticle");
    }

    std::vector<float> coordinate(nparticle, 0.0);
    for(auto& v : coordinate)
    {
        is.read(reinterpret_cast<char*>(std::addressof(v)), size_float);
    }

    int size_of_block_f;
    is.read(reinterpret_cast<char*>(std::addressof(size_of_block_f)), size_int);
    if(size_of_block != size_of_block_f)
    {
        std::cerr << "error: mill::DCDReader: size of block = " << size_of_block;
        std::cerr << ", nparticle = " << nparticle << '\n';
        throw std::invalid_argument(
                "dcd coordinate block has invalid byte-information");
    }
    return coordinate;
}

}// mill
#endif //COFFEE_MILL_DCD_READER
