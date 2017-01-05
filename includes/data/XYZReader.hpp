/*!
  @file XYZReader.hpp
  @brief definition of a class that reads xyz file.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2017-01-05 17:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_XYZ_READER
#define COFFEE_MILL_XYZ_READER
#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace mill
{

//! read XYZfile and return the data as std::vector<std::vector<position>>.
/*!
 *  @tparam vectorT type of position
 */
template<typename vectorT>
class XYZReader
{
  public:
    using vector_type     = vectorT;
    using position_type   = vector_type;
    using real_type       = typename scalar_type_extractor<vector_type>::type;
    using snapshot_type       = std::vector<position_type>;
    using trajectory_type     = std::vector<snapshot_type>;
    using xyz_snapshot_type   = std::vector<std::pair<std::string, position_type>>;
    using xyz_trajectory_type = std::vector<xyz_trajectory_type>;

  public:

    DCDReader()  = default;
    ~DCDReader() = default;

    xyz_trajectory_type read(const std::string& fname);
    xyz_trajectory_type read(std::istream& is);

    trajectory_type read_traj(const std::string& fname);
    trajectory_type read_traj(std::istream& is);

//     snapshot_type read_snapshot(const std::string& fname, const std::size_t i);
    xyz_snapshot_type read_xyz_snapshot(std::istream& is);
    snapshot_type     read_snapshot(std::istream& is);
};

template<typename vectorT>
typename XYZReader<vectorT>::xyz_trajectory_type
XYZReader<vectorT>::read(const std::string& fname)
{
    std::ifstream ifs(fname);
    if(not ifs.good())
        throw std::runtime_error("file open error: " + fname);

    xyz_trajectory_type traj;
    while(not ifs.eof())
    {
        traj.emplace_back(read_xyz_snapshot(ifs));
    }

    return traj;
}

template<typename vectorT>
typename XYZReader<vectorT>::trajectory_type
XYZReader<vectorT>::read(const std::string& fname)
{
    std::ifstream ifs(fname);
    if(not ifs.good())
        throw std::runtime_error("file open error: " + fname);

    trajectory_type traj;
    while(not ifs.eof())
    {
        traj.emplace_back(read_snapshot(ifs));
    }

    return traj;
}

template<typename vectorT>
typename XYZReader<vectorT>::xyz_trajectory_type
XYZReader<vectorT>::read(std::istream& is)
{
    xyz_trajectory_type traj;
    while(not is.eof())
    {
        traj.emplace_back(read_xyz_snapshot(is));
    }
    return traj;
}

template<typename vectorT>
typename XYZReader<vectorT>::trajectory_type
XYZReader<vectorT>::read_traj(std::istream& is)
{
    trajectory_type traj;
    while(not is.eof())
    {
        traj.emplace_back(read_snapshot(is));
    }
    return traj;
}

template<typename vectorT>
typename XYZReader<vectorT>::xyz_snapshot_type
XYZReader<vectorT>::read_xyz_snapshot(std::istream& is)
{
    std::string num, comment;
    std::getline(is, num);
    std::getline(is, comment);
    std::size_t N;
    try
    {
        if(num.empty) throw std::excpetion;
        N = std::stoi(num);
    }
    catch(std::exception& except)
    {
        throw std::runtime_error("invalid XYZ file format");
    }

    std::string line;
    snapshot_type snapshot(N);
    for(std::size_t i=0; i<N; ++i)
    {
        if(not is.good()) throw std::runtime_error("invalid xyz file");

        std::getline(is, line);
        std::istringstream iss(line);
        std::string header;
        real_type x, y, z;
        iss >> header >> x >> y >> z;
        snapshot.at(i) = std::make_pair(header, position_type(x, y, z));
    }

    return snapshot;
}

template<typename vectorT>
typename XYZReader<vectorT>::snapshot_type
XYZReader<vectorT>::read_snapshot(std::istream& is)
{
    std::string num, comment;
    std::getline(is, num);
    std::getline(is, comment);
    std::size_t N;
    try
    {
        if(num.empty) throw std::excpetion;
        N = std::stoi(num);
    }
    catch(std::exception& except)
    {
        throw std::runtime_error("invalid XYZ file format");
    }

    std::string line;
    snapshot_type snapshot(N);
    for(std::size_t i=0; i<N; ++i)
    {
        if(not is.good()) throw std::runtime_error("invalid xyz file");

        std::getline(is, line);
        std::istringstream iss(line);
        std::string header;
        real_type x, y, z;
        iss >> header >> x >> y >> z;
        snapshot.at(i) = position_type(x, y, z);
    }

    return snapshot;
}

}// mill
#endif //COFFEE_MILL_DCD_READER
