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
#include "mill/util/write_as_binary.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdint>

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
};

template <typename vectorT>
void DCDWriter<vectorT>::write(const std::string& filename, const data_type& dcd)
{
    std::ofstream ifs(filename);
    if(not ifs.good())
    {
        throw std::runtime_error("file open error: " + filename);
    }
    this->write(ifs, dcd);
    ifs.close();
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write(std::ostream& os, const data_type& dcd)
{
    this->write_header(os, dcd.header());
    this->write_trajectory(os, dcd.traj());

    log(log_level::debug, "DCDWriter: file written");
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_header(
        const std::string& fname, const header_type& dcd)
{
    std::ofstream dcdfile(fname, std::ios::binary);
    if(!dcdfile.good())
    {
        throw std::runtime_error("file open error: " + fname);
    }
    write_header(dcdfile, dcd);
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
    write_as_binary<std::int32_t>(os, 84);

    os.write(data.signeture.c_str(), 4);

    write_as_binary<std::int32_t>(os, data.nset);
    write_as_binary<std::int32_t>(os, data.istart);
    write_as_binary<std::int32_t>(os, data.nstep_save);
    write_as_binary<std::int32_t>(os, data.nstep);
    write_as_binary<std::int32_t>(os, data.nunit);

    // null data
    for(std::size_t i=0; i<4; ++i){write_as_binary<std::int32_t>(os, 0);}

    write_as_binary<float>(os, static_cast<float>(data.delta_t));
    write_as_binary<std::int32_t>(os, 0); // boundary info flag. no boundary.

    // null data
    for(std::size_t i=0; i<8; ++i){write_as_binary<std::int32_t>(os, 0);}

    write_as_binary<std::int32_t>(os, data.verCHARMM);
    write_as_binary<std::int32_t>(os, 84);
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_head_block2(std::ostream& os, const header_type& data)
{
    if(data.comment.empty())
    {
        const char comment[81] = "====================== Generated by Coff"
                                 "ee-mill DCDWriter ======================";
        write_as_binary<std::int32_t>(os, 84);
        write_as_binary<std::int32_t>(os,  1);
        os.write(comment, 80);
        write_as_binary<std::int32_t>(os, 84);
    }
    else
    {
        const std::int32_t lines = data.comment.size();
        const std::int32_t bytes = 4 + 80 * lines;

        write_as_binary(os, bytes);
        write_as_binary(os, lines);
        for(auto com : data.comment)
        {
            if(com.size() < 80) {com.resize(80, ' ');}
            os.write(com.c_str(), 80);
        }
        write_as_binary(os, bytes);
    }
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_head_block3(
        std::ostream& os, const header_type& data)
{
    write_as_binary<std::int32_t>(os, 4);
    write_as_binary<std::int32_t>(os, data.nparticle);
    write_as_binary<std::int32_t>(os, 4);
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_trajectory(
        const std::string& fname, const trajectory_type& traj)
{
    std::ifstream ifs(fname);
    if(not ifs.good())
    {
        throw std::runtime_error("file open error: " + fname);
    }
    this->write_trajectory(ifs, traj);
    ifs.close();
    return ;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_trajectory(
        std::ostream& os, const trajectory_type& traj)
{
    for(const auto& ss : traj)
    {
        this->write_snapshot(os, ss);
    }
    return;
}

template <typename vectorT>
void DCDWriter<vectorT>::write_snapshot(
        const std::string& fname, const snapshot_type& snapshot)
{
    std::ifstream ifs(fname);
    if(not ifs.good())
    {
        throw std::runtime_error("file open error: " + fname);
    }
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

    std::size_t counter = 0;
    for(const auto& pos : snapshot)
    {
        x[counter] = static_cast<float>(pos[0]);
        y[counter] = static_cast<float>(pos[1]);
        z[counter] = static_cast<float>(pos[2]);
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
    const std::int32_t size = 4 * coord.size();
    write_as_binary(os, size);
    for(const float v : coord)
    {
        write_as_binary(os, v);
    }
    write_as_binary(os, size);
    return;
}

}//mill
#endif //COFFEE_MILL_DCD_WRITER
