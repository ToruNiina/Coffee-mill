/*!
  @file DCDReader.hpp
  @brief definition of a class that reads dcd file.

  DCDReader reads dcd file and store the information included in the file.
  the data is stored as DCDData. Reader can read only a header region.

  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_READER_HPP
#define COFFEE_MILL_DCD_READER_HPP
#include <mill/util/read_binary_as.hpp>
#include <mill/util/logger.hpp>
#include "DCDData.hpp"
#include <memory>
#include <utility>
#include <iostream>
#include <fstream>

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
    log(log_level::debug, "DCD file reading start\n");

    data_type data = read_header(is);
    read_trajectory(is, data);

    log(log_level::debug, "DCD file reading completed\n");
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
        const auto expected_size = header1_size + header2_size + header3_size +
                                   snapshot_size * dat.nset();
        log(log_level::warn, "invalid filesize!\n");
        log(log_level::warn, "actual size is ", file_size, " bytes\n");
        log(log_level::warn, "but it contains ", dat.nset(), " snapshots.\n");
        log(log_level::warn, "number of particles is ", dat.nparticle(), ".\n");
        log(log_level::warn, "1st header block has ", header1_size, " bytes\n");
        log(log_level::warn, "2nd header block has ", header2_size, " bytes\n");
        log(log_level::warn, "3rd header block has ", header3_size, " bytes\n");
        log(log_level::warn, "The file must have ",  expected_size, " bytes\n");

        if((file_size - header1_size - header2_size - header3_size) % snapshot_size != 0)
        {
            log(log_level::warn, "The last snapshot seems to be incomplete.\n");
        }

        dat.nset() = (file_size - header1_size - header2_size - header3_size) /
                     snapshot_size;
        log(log_level::warn, "The actual number of snapshots seems to be ",
                             dat.nset(), ".\n");
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
    const int byte = read_binary_as<int>(dcdfile);
    if(byte != 84)
    {
        log(log_level::warn, "this file may not be cafemol output\n");
    }

    char csigneture[5];
    dcdfile.read(csigneture, 4);
    csigneture[4] = '\0';
    std::string signeture(csigneture);
    log(log_level::debug, "file signeture is \"", signeture, "\"\n");

    if(signeture != "CORD" && signeture != "VELD")
    {
        throw std::runtime_error("Unknown File Signeture: " + signeture);
    }

    data.signeture() = signeture;
    data.nset() = read_binary_as<int>(dcdfile);
    log(log_level::debug, "nset       = ", data.nset(), "\n");

    data.istart() = read_binary_as<int>(dcdfile);
    log(log_level::debug, "istart     = ", data.istart(), "\n");

    data.nstep_save() = read_binary_as<int>(dcdfile);
    log(log_level::debug, "nstep_save = ", data.nstep_save(), "\n");

    data.nstep() = read_binary_as<int>(dcdfile);
    log(log_level::debug, "nstep      = ", data.nstep(), "\n");

    data.nunit() = read_binary_as<int>(dcdfile);
    log(log_level::debug, "nunit      = ", data.nunit(), "\n");

    dcdfile.ignore(16);

    data.delta_t() = read_binary_as<float>(dcdfile);
    log(log_level::debug, "delta_t    = ", data.delta_t(), "\n");

    dcdfile.ignore(36);

    data.verCHARMM() = read_binary_as<int>(dcdfile);

    const int bytes_f = read_binary_as<int>(dcdfile);
    if(byte != bytes_f)
    {
        throw std::runtime_error("header block1 has invalid size information");
    }
    this->header1_size = byte + size_int * 2;
    return;
}

template <typename T>
void DCDReader<T>::read_header_block2(std::istream& dcdfile, data_type& data)
{
    const int bytes = read_binary_as<int>(dcdfile);

    const int lines = read_binary_as<int>(dcdfile);
    if((80 * lines + 4) != bytes)
    {
        std::cerr << "Error  : header block2 size = " << bytes << " bytes."
                  << " But block2 has " << lines << "lines" << std::endl;
        throw std::invalid_argument("header block2 has invalid size");
    }

    for(int i(0); i<lines; ++i)
    {
        char line[81];
        dcdfile.read(line, 80);
        line[80] = '\0';
        log(log_level::debug, i, "-th comment: ", line, "\n");
        data.comment().push_back(std::string(line));
    }
    const int bytes_f = read_binary_as<int>(dcdfile);
    if(bytes != bytes_f)
    {
        throw std::runtime_error("header block2 has invalid size information");
    }
    header2_size = bytes + size_int * 2;
    return;
}

template <typename T>
void DCDReader<T>::read_header_block3(std::istream& dcdfile, data_type& data)
{
    const int bytes  = read_binary_as<int>(dcdfile);
    data.nparticle() = read_binary_as<int>(dcdfile);
    log(log_level::debug, "nparticle = ", data.nparticle(), "\n");

    const int bytes_f = read_binary_as<int>(dcdfile);
    if(bytes != bytes_f)
    {
        throw std::runtime_error("header block3 has invalid size information");
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

    for(int i=0; i<data.nset(); ++i)
    {
        const std::vector<float> x(read_coord(is, data.nparticle()));
        const std::vector<float> y(read_coord(is, data.nparticle()));
        const std::vector<float> z(read_coord(is, data.nparticle()));

        snapshot_type temp_snapshot(data.nparticle());
        for(int c=0; c < data.nparticle(); ++c)
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
    for(int c = 0; c < header.nparticle; ++c)
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
    const int size_of_block = read_binary_as<int>(is);
    if(size_of_block / size_float != nparticle)
    {
        log(log_level::error, "invalid coordinate block!\n");
        log(log_level::error, "size of coordinate block is ", size_of_block, "\n");
        log(log_level::error, "but the number of particle is  ", nparticle, "\n");
        throw std::runtime_error("dcd coordinate block size invalid");
    }

    std::vector<float> coordinate(nparticle, 0.0);
    for(auto& v : coordinate)
    {
        v = read_binary_as<float>(is);
    }

    const int size_of_block_f = read_binary_as<int>(is);
    if(size_of_block != size_of_block_f)
    {
        log(log_level::error, "invalid coordinate block delimiter\n");
        log(log_level::error, "the size of the block is not ", size_of_block_f,
                              " but ", size_of_block, "\n");
        throw std::runtime_error("invalid delimiter in a coordinate block");
    }
    return coordinate;
}

}// mill
#endif //COFFEE_MILL_DCD_READER
