/*!
  @file DCDWriter.hpp
  @brief definition of a class that writes dcd file.

  DCDWriter writes DCDData into DCD file. this can write only one snapshot
  step by step. But, when you write trajectory step by step, the consistency
  between header data and trajectory size will not be guaranteed.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_WRITER
#define COFFEE_MILL_DCD_WRITER
#include "DCDData.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>

namespace mill
{

//! DCD writer class.
/*!
 *  DCDWriter writes not only entire DCDData but also many Snapshots into dcd file.
 */
template <typename vectorT>
class DCDWriter
{
  public:
    using vector_type     = vectorT;
    using data_type       = DCDData<vector_type>;
    using header_type     = typename data_type::header_type;
    using snapshot_type   = typename data_type::snapshot_type;
    using trajectory_type = typename data_type::trajectory_type;

  public:

    DCDWriter(){}
    ~DCDWriter() = default;

    void write(const std::string& fname, const data_type& dcd);
    void write(std::ostream& os,         const data_type& dcd);
    void write_header    (const std::string& fname, const header_type& dcd);
    void write_header    (std::ostream& os,         const header_type& dcd);
    void write_trajectory(const std::string& fname, const trajectory_type& traj);
    void write_trajectory(std::ostream& dcdfile,    const trajectory_type& traj);
    void write_snapshot  (const std::string& fname, const snapshot_type& snapshot);
    void write_snapshot  (std::ostream& os,         const snapshot_type& snapshot);

  private:

    void write_head_block1(std::ostream& dcdfile, const header_type& header);
    void write_head_block2(std::ostream& dcdfile, const header_type& header);
    void write_head_block3(std::ostream& dcdfile, const header_type& header);
    void write_coord      (std::ostream& dcdfile, const std::vector<float>& coord);

  private:

    static constexpr int size_int   = sizeof(int);
    static constexpr int size_float = sizeof(float);
    static constexpr int size_char  = sizeof(char);
};

template <typename vectorT>
void DCDWriter<vectorT>::write(const std::string& filename, const data_type& dcd)
{
    std::ofstream ifs(filename);
    if(not ifs.good()) throw std::runtime_error("file open error: " + filename);
    this->write(ifs, dcd);
    ifs.close();
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write(std::ostream& os, const data_type& dcd)
{
    this->write_header(os, dcd.header());
    this->write_trajectory(os, dcd.traj());

#ifdef COFFEE_MILL_DEBUG
    std::cout << "Info   : write_file completed" << std::endl;
#endif

    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_header(
        const std::string& fname, const header_type& dcd)
{
    std::ofstream dcdfile(fname, std::ios::binary);
    if(!dcdfile.good())
        throw std::runtime_error("file open error: " + fname);

    write_header(dcdfile, dcd);
    dcdfile.close();

    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_header(std::ostream& os, const header_type& dcd)
{
    write_head_block1(os, dcd);
    write_head_block2(os, dcd);
    write_head_block3(os, dcd);
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_head_block1(
        std::ostream& os, const header_type& data)
{
    int wrote(0);
    const int bytes(84);
    os.write(reinterpret_cast<const char*>(&bytes), size_int);

    const char* signeture = data.signeture.c_str();
    os.write(signeture, size_char*4);
    wrote += size_char*4;

    os.write(reinterpret_cast<const char*>(&(data.nset)), size_int);
    wrote += size_int;

    os.write(reinterpret_cast<const char*>(&(data.istart)), size_int);
    wrote += size_int;

    os.write(reinterpret_cast<const char*>(&(data.nstep_save)), size_int);
    wrote += size_int;

    os.write(reinterpret_cast<const char*>(&(data.nstep)), size_int);
    wrote += size_int;

    os.write(reinterpret_cast<const char*>(&(data.nunit)), size_int);
    wrote += size_int;

    // null data
    const int fill_zero(0);
    for(int i(0); i<4; ++i)
    {
        os.write(reinterpret_cast<const char*>(&fill_zero), size_int);
        wrote += size_int;
    }

    const float dt = static_cast<float>(data.delta_t);
    os.write(reinterpret_cast<const char*>(&dt), size_float);
    wrote += size_float;

    // null data
    for(int i(0); i<9; ++i)
    {
        os.write(reinterpret_cast<const char*>(&fill_zero), size_int);
        wrote += size_int;
    }

    os.write(reinterpret_cast<const char*>(&(data.verCHARMM)), size_int);
    wrote += size_int;

    if(wrote != bytes)
        throw std::invalid_argument("byte information error");

    os.write(reinterpret_cast<const char*>(&bytes), size_int);
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_head_block2(std::ostream& os, const header_type& data)
{
    if(data.comment.empty())
    {
        const int lines(1);
        const int bytes(84);
        int wrote(0);
        const char* default_comment =
            "==================== This file is modified using Coffee-mill ===================";

        os.write(reinterpret_cast<const char*>(&bytes), size_int);
        os.write(reinterpret_cast<const char*>(&lines), size_int);
        wrote += size_int;
        os << default_comment;
        wrote += 80;
        if(wrote != bytes)
            throw std::logic_error("internal byte information error");
        os.write(reinterpret_cast<const char*>(&bytes), size_int);
    }
    else
    {
        const int lines(data.comment.size());
        const int bytes(4 + 80*lines);
        int wrote(0);

        os.write(reinterpret_cast<const char*>(&bytes), size_int);

        os.write(reinterpret_cast<const char*>(&lines), size_int);
        wrote += size_int;

        for(auto iter = data.comment.cbegin(); iter != data.comment.cend(); ++iter)
        {
            os << iter->c_str();
            wrote += size_char*80;
        }

        if(wrote != bytes)
            throw std::logic_error("internal byte information error");

        os.write(reinterpret_cast<const char*>(&bytes), size_int);
    }
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_head_block3(
        std::ostream& dcdfile, const header_type& data)
{
    int bytes(size_int);
    dcdfile.write(reinterpret_cast<const char*>(&bytes), size_int);
    dcdfile.write(reinterpret_cast<const char*>(&(data.nparticle)), size_int);
    dcdfile.write(reinterpret_cast<const char*>(&bytes), size_int);
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_trajectory(
        const std::string& fname, const trajectory_type& traj)
{
    std::ifstream ifs(fname);
    if(not ifs.good()) throw std::runtime_error("file open error: " + fname);
    this->write_trajectory(ifs, traj);
    ifs.close();
    return ;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_trajectory(
        std::ostream& os, const trajectory_type& traj)
{
    for(auto iter = traj.cbegin(); iter != traj.cend(); ++iter)
        this->write_snapshot(os, *iter);
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_snapshot(
        const std::string& fname, const snapshot_type& snapshot)
{
    std::ifstream ifs(fname);
    if(not ifs.good()) throw std::runtime_error("file open error: " + fname);
    this->write_snapshot(ifs, snapshot);
    ifs.close();
    return ;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_snapshot(
        std::ostream& os, const snapshot_type& snapshot)
{
    std::vector<float> x(snapshot.size());
    std::vector<float> y(snapshot.size());
    std::vector<float> z(snapshot.size());
    std::size_t counter(0);
    for(auto iter = snapshot.cbegin(); iter != snapshot.cend(); ++iter)
    {
        x[counter] = static_cast<float>((*iter)[0]);
        y[counter] = static_cast<float>((*iter)[1]);
        z[counter] = static_cast<float>((*iter)[2]);
        ++counter;
    }

    this->write_coord(os, x);
    this->write_coord(os, y);
    this->write_coord(os, z);

    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_coord(
        std::ostream& os, const std::vector<float>& coord)
{
    int size(coord.size());
    int bytes(size * size_float);
    os.write(reinterpret_cast<const char*>(&bytes), size_int);

    for(auto iter = coord.cbegin(); iter != coord.cend(); ++iter)
    {
        const float tmp = *iter;
        os.write(reinterpret_cast<const char*>(&(tmp)), size_float);
    }

    os.write(reinterpret_cast<const char*>(&bytes), size_int);
    return;
}


}//mill

#endif //COFFEE_MILL_DCD_WRITER
