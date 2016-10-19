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
    using vector_type   = vectorT;
    using data_type     = DCDData<vector_type>;
    using header_type   = typename data_type::header_type;
    using snapshot_type = typename data_type::snapshot_type;

  public:

    DCDWriter(){}
    ~DCDWriter() = default;

    void write(const std::string& fname, const data_type& dcd);
    void write(std::ostream& os, const data_type& dcd);
    void write_header(const std::string& fname, const header_type& dcd);
    void write_header(std::ostream& os, const header_type& dcd);
    void write_snapshot(const std::string& fname, const snapshot_type& snapshot);
    void write_snapshot(std::ostream& os, const snapshot_type& snapshot);

  private:

    void write_header     (std::ostream& dcdfile);
    void write_head_block1(std::ostream& dcdfile);
    void write_head_block2(std::ostream& dcdfile);
    void write_head_block3(std::ostream& dcdfile);
    void write_trajectory (std::ostream& dcdfile);
    void write_snapshot   (std::ostream& dcdfile, const snapshot_type& snapshot);
    void write_coord      (std::ostream& dcdfile, const std::vector<float>& coord);

  private:

    static constexpr int size_int   = sizeof(int);
    static constexpr int size_float = sizeof(float);
    static constexpr int size_char  = sizeof(char);
};

template <typename vectorT>
void DCDWriter<vectorT>::write(const std::string& filename, const data_type& dcd)
{
    std::ifstream ifs(filename);
    if(not ifs.good()) throw std::rumtime_error("file open error: " + filename);
    this->write(ifs, dcd);
    ifs.close();
    return;
}

void DCDWriter::write(std::ostream& os, const data_type& dcd)
{
    write_header(os, dcd.header());
    write_core(dcdfile);

#ifdef COFFEE_MILL_DEBUG
    std::cout << "Info   : write_file completed" << std::endl;
#endif

    return;
}

void DCDWriter::write_header(const std::string& fname, const header_type& dcd)
{
    std::ofstream dcdfile(this->filename_, std::ios::binary);
    if(!dcdfile.good())
        throw std::runtime_error("file open error: " + filename_);

    write_header(dcdfile);
    dcdfile.close();

    return;
}

void DCDWriter::write_header(std::ofstream& dcdfile)
{
    write_head_block1(dcdfile);
    write_head_block2(dcdfile);
    write_head_block3(dcdfile);
    header_written = true;
    return;
}

void DCDWriter::write_head_block1(std::ofstream& dcdfile)
{
    int wrote(0);
    int bytes(84);
    dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);

    const char* signeture = this->data_.signeture().c_str();
    dcdfile.write(signeture, size_char*4);
    wrote += size_char*4;

    dcdfile.write(reinterpret_cast<char*>(&(this->data_.nset())), size_int);
    wrote += size_int;

    dcdfile.write(reinterpret_cast<char*>(&(this->data_.istart())), size_int);
    wrote += size_int;

    dcdfile.write(reinterpret_cast<char*>(&(this->data_.nstep_save())), size_int);
    wrote += size_int;

    dcdfile.write(reinterpret_cast<char*>(&(this->data_.nstep())), size_int);
    wrote += size_int;

    dcdfile.write(reinterpret_cast<char*>(&(this->data_.nunit())), size_int);
    wrote += size_int;

    // null data
    int fill_zero(0);
    for(int i(0); i<4; ++i)
    {
        dcdfile.write(reinterpret_cast<char*>(&fill_zero), size_int);
        wrote += size_int;
    }

    float dt = static_cast<float>(this->data_.delta_t());
    dcdfile.write(reinterpret_cast<char*>(&dt), size_float);
    wrote += size_float;

    // null data
    for(int i(0); i<9; ++i)
    {
        dcdfile.write(reinterpret_cast<char*>(&fill_zero), size_int);
        wrote += size_int;
    }

    dcdfile.write(reinterpret_cast<char*>(&(this->data_.verCHARMM())), size_int);
    wrote += size_int;

    if(wrote != bytes)
    {
        throw std::invalid_argument("byte information error");
    }

    dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
    return;
}

void DCDWriter::write_head_block2(std::ofstream& dcdfile)
{
    if(this->data_.header().empty())
    {
        this->data_.push_header(
            "==================== Molecular Dynamics Code : CafeMol ver.  2.02 ==============");
        this->data_.push_header(
            "==================== Developed by Kyoto University =============================");
        this->data_.push_header(
            "==================== This file is modified using coffee-mill ===================");
    }

    int lines(this->data_.header().size());
    int bytes(4 + 80*lines);
    int wrote(0);

    dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);

    dcdfile.write(reinterpret_cast<char*>(&lines), size_int);
    wrote += size_int;

    for(auto iter = this->data_.header().cbegin();
            iter != this->data_.header().cend(); ++iter)
    {
        dcdfile << *iter;
        wrote += size_char*80;
    }

    if(wrote != bytes)
        throw std::invalid_argument("internal byte information error");

    dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
    return;
}

void DCDWriter::write_head_block3(std::ofstream& dcdfile)
{
    int bytes(size_int);
    dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
    dcdfile.write(reinterpret_cast<char*>(&(this->data_.nparticle())), size_int);
    dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
    return;
}

void DCDWriter::write_core(std::ofstream& dcdfile)
{
    for(auto iter = this->data_.cbegin(); iter != this->data_.cend(); ++iter)
    {
        write_snapshot(dcdfile, *iter);
    }
    return;
}

void DCDWriter::write_snapshot(std::ofstream& dcdfile, const snapshot_type& snapshot)
{
    std::vector<double> x(snapshot.size());
    std::vector<double> y(snapshot.size());
    std::vector<double> z(snapshot.size());
    std::size_t counter(0);
    for(auto iter = snapshot.cbegin(); iter != snapshot.cend(); ++iter)
    {
        x[counter] = (*iter)[0];
        y[counter] = (*iter)[1];
        z[counter] = (*iter)[2];
        ++counter;
    }

    write_coord(dcdfile, x);
    write_coord(dcdfile, y);
    write_coord(dcdfile, z);

    return;
}


void DCDWriter::write_snapshot(const snapshot_type& snapshot)
{
    if(this->filename_.empty())
        throw std::invalid_argument("Error: filename is not specified!");

    if(!header_written)
        throw std::invalid_argument("Error: writing DCD file without header");

    std::ofstream dcdfile(this->filename_, std::ios::binary | std::ios::app);
    if(!dcdfile.good())
        throw std::runtime_error("file open error: " + filename_);

    write_snapshot(dcdfile, snapshot);

    dcdfile.close();

    return;
}

void DCDWriter::write_coord(std::ofstream& dcdfile, const std::vector<double>& coord)
{
    int size(coord.size());
    int bytes(size * size_float);
    dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);

    for(auto iter = coord.cbegin(); iter != coord.cend(); ++iter)
    {
        float temp(static_cast<float>(*iter));
        dcdfile.write(reinterpret_cast<char*>(&temp), size_float);
    }

    dcdfile.write(reinterpret_cast<char*>(&bytes), size_int);
    return;
}


}//mill

#endif //COFFEE_MILL_DCD_WRITER
