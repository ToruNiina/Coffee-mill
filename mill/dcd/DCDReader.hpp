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
    using boundary_type = typename data_type::boundary_type;

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

    boundary_type read_unit_cell(std::istream& dcdfile) const;

  private:

    // for filesize validation
    std::size_t header1_size;
    std::size_t header2_size;
    std::size_t header3_size;
    std::size_t snapshot_size;
    std::int32_t has_unitcell_;
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
    log::debug("DCD file reading start\n");

    data_type data = read_header(is);
    read_trajectory(is, data);

    log::debug("DCD file reading completed\n");
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
        log::warn("invalid filesize!\n");
        log::warn("actual size is ", file_size, " bytes\n");
        log::warn("but it contains ", dat.nset(), " snapshots.\n");
        log::warn("number of particles is ", dat.nparticle(), ".\n");
        log::warn("1st header block has ", header1_size, " bytes\n");
        log::warn("2nd header block has ", header2_size, " bytes\n");
        log::warn("3rd header block has ", header3_size, " bytes\n");
        log::warn("The file must have ",  expected_size, " bytes\n");

        if((file_size - header1_size - header2_size - header3_size) % snapshot_size != 0)
        {
            log::warn("The last snapshot seems to be incomplete.\n");
        }

        dat.nset() = (file_size - header1_size - header2_size - header3_size) /
                     snapshot_size;
        log::warn("The actual number of snapshots seems to be ",
                             dat.nset(), ".\n");
    }
    return dat;
}

template <typename T>
bool DCDReader<T>::validate_filesize(
        const data_type& data, std::size_t file_size)
{
    this->snapshot_size = 12 * (data.nparticle() + 2);
    if(this->has_unitcell_)
    {
        this->snapshot_size += sizeof(std::int32_t) * 2 + sizeof(double) * 6;
    }
    return file_size == (header1_size + header2_size + header3_size +
                        snapshot_size * data.nset());
}

template <typename T>
void DCDReader<T>::read_header_block1(std::istream& dcdfile, data_type& data)
{
    const int byte = read_binary_as<int>(dcdfile);
    if(byte != 84)
    {
        log::warn("this file may not be cafemol output\n");
    }

    char csigneture[5];
    dcdfile.read(csigneture, 4);
    csigneture[4] = '\0';
    std::string signeture(csigneture);
    log::debug("file signeture is \"", signeture, "\"\n");

    if(signeture != "CORD" && signeture != "VELD")
    {
        throw std::runtime_error("Unknown File Signeture: " + signeture);
    }

    data.signeture() = signeture;
    data.nset() = read_binary_as<int>(dcdfile);
    log::debug("nset       = ", data.nset(), "\n");

    data.istart() = read_binary_as<int>(dcdfile);
    log::debug("istart     = ", data.istart(), "\n");

    data.nstep_save() = read_binary_as<int>(dcdfile);
    log::debug("nstep_save = ", data.nstep_save(), "\n");

    data.nstep() = read_binary_as<int>(dcdfile);
    log::debug("nstep      = ", data.nstep(), "\n");

    data.nunit() = read_binary_as<int>(dcdfile);
    log::debug("nunit      = ", data.nunit(), "\n");

    dcdfile.ignore(16);

    data.delta_t() = read_binary_as<float>(dcdfile);
    log::debug("delta_t    = ", data.delta_t(), "\n");

    this->has_unitcell_ = read_binary_as<std::int32_t>(dcdfile);

    dcdfile.ignore(32);

    data.verCHARMM() = read_binary_as<int>(dcdfile);

    const int bytes_f = read_binary_as<int>(dcdfile);
    if(byte != bytes_f)
    {
        throw std::runtime_error("header block1 has invalid size information");
    }
    this->header1_size = byte + sizeof(int) * 2;
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
        log::debug(i, "-th comment: ", line, "\n");
        data.comment().push_back(std::string(line));
    }
    const int bytes_f = read_binary_as<int>(dcdfile);
    if(bytes != bytes_f)
    {
        throw std::runtime_error("header block2 has invalid size information");
    }
    header2_size = bytes + sizeof(int) * 2;
    return;
}

template <typename T>
void DCDReader<T>::read_header_block3(std::istream& dcdfile, data_type& data)
{
    const int bytes  = read_binary_as<int>(dcdfile);
    data.nparticle() = read_binary_as<int>(dcdfile);
    log::debug("nparticle = ", data.nparticle(), "\n");

    const int bytes_f = read_binary_as<int>(dcdfile);
    if(bytes != bytes_f)
    {
        throw std::runtime_error("header block3 has invalid size information");
    }
    header3_size = bytes + sizeof(int) * 2;
    return;
}

template <typename T>
typename DCDReader<T>::boundary_type
DCDReader<T>::read_unit_cell(std::istream& is) const
{
    if(not this->has_unitcell_)
    {
        return boundary_type(UnlimitedBoundary<vector_type>());
    }
    using position_type = typename data_type::position_type;

    const auto block_begin = read_binary_as<std::int32_t>(is);
    const auto A           = read_binary_as<double      >(is);
    const auto gamma       = read_binary_as<double      >(is);
    const auto B           = read_binary_as<double      >(is);
    const auto beta        = read_binary_as<double      >(is);
    const auto alpha       = read_binary_as<double      >(is);
    const auto C           = read_binary_as<double      >(is);
    const auto block_end   = read_binary_as<std::int32_t>(is);

    if(block_begin != block_end)
    {
        log::error("DCD file has invalid unit cell information.\n");
        log::error("block_begin = ", block_begin, ", block_end = ", block_end, "\n");
        log::error("A     = ", A,     ", B    = ", B,    ", C     = ", C,      "\n");
        log::error("alpha = ", alpha, ", beta = ", beta, ", gamma = ", gamma,  "\n");
        return boundary_type(UnlimitedBoundary<vector_type>());
    }

    const auto differs = [](const double lhs, const double rhs) noexcept -> bool {
        constexpr double rel_tol = 1e-8;
        constexpr double abs_tol = 1e-10;
        return abs_tol                     < std::abs(lhs - rhs) ||
               rel_tol * 0.5 * (lhs + rhs) < std::abs(lhs - rhs);
    };
    constexpr double half_pi = 1.57079632679;

    if((differs(alpha, 90.0) && differs(alpha, std::cos(half_pi))) ||
       (differs(beta,  90.0) && differs(beta,  std::cos(half_pi))) ||
       (differs(gamma, 90.0) && differs(gamma, std::cos(half_pi))))
    {
        log::error("The unit cell is not a rectangle\n");
        log::error("angle alpha = ", alpha, '\n');
        log::error("angle beta  = ", beta , '\n');
        log::error("angle gamma = ", gamma, '\n');
    }
    return boundary_type(CuboidalPeriodicBoundary<position_type>(
            position_type(0, 0, 0), position_type(A, B, C)));
}

template <typename T>
void DCDReader<T>::read_trajectory(std::istream& is, data_type& data)
{
    using position_type = typename data_type::position_type;
    data.traj().clear();
    data.traj().reserve(data.nset());

    for(int i=0; i<data.nset(); ++i)
    {
        // read unit cell
        data.boundary().push_back(this->read_unit_cell(is));

        // read coordinates
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
    const std::size_t snapsize = (npart + 2) * 3 * sizeof(float);
    ifs.ignore(snapsize * (i-1));

    return this->read_snapshot(ifs, header);
}

template <typename T>
std::vector<float>
DCDReader<T>::read_coord(std::istream& is, const std::size_t nparticle)
{
    const int size_of_block = read_binary_as<int>(is);
    if(size_of_block / sizeof(float) != nparticle)
    {
        log::error("invalid coordinate block!\n");
        log::error("size of coordinate block is ", size_of_block, "\n");
        log::error("but the number of particle is  ", nparticle, "\n");
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
        log::error("invalid coordinate block delimiter\n");
        log::error("the size of the block is not ", size_of_block_f,
                              " but ", size_of_block, "\n");
        throw std::runtime_error("invalid delimiter in a coordinate block");
    }
    return coordinate;
}

}// mill
#endif //COFFEE_MILL_DCD_READER
