#include "DCDWriter.hpp"
#include <iostream>
#include <fstream>

namespace coffeemill
{

void DCDWriter::write(const std::string& filename)
{
    if(filename_ != filename)
    {
        std::cerr << "Warning: you already specify filename in dcdwriter "
                  << filename << " but now change filename to "
                  << filename_ << std::endl;
    }

    filename_ = filename;
    this->write();
    return;
}

void DCDWriter::write()
{
    if(header_written)
        throw std::invalid_argument("file is partially written");

    std::ofstream dcdfile(this->filename_, std::ios::binary);
    if(!dcdfile.good())
        throw std::runtime_error("file open error: " + filename_);

    write_header(dcdfile);
    write_core(dcdfile);

#ifdef COFFEE_MILL_DEBUG
    std::cout << "Info   : write_file completed" << std::endl;
#endif
    dcdfile.close();
    return;
}

void DCDWriter::write_header()
{
    if(header_written)
        std::cerr << "Warning: header is already written "
                  << "but now you trying to re-write header" << std::endl;

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

}
