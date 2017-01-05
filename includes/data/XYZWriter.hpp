/*!
  @file XYZWriter.hpp
  @brief definition of a class that reads xyz file.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2017-01-05 17:30
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_XYZ_WRITER
#define COFFEE_MILL_XYZ_WRITER
#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace mill
{

//! write trajectory data to XYZfile
/*!
 *  @tparam vectorT type of position
 */
template<typename vectorT>
class XYZWriter
{
  public:
    using vector_type         = vectorT;
    using position_type       = vector_type;
    using real_type           = typename scalar_type_extractor<vector_type>::type;
    using snapshot_type       = std::vector<position_type>;
    using trajectory_type     = std::vector<snapshot_type>;
    using xyz_snapshot_type   = std::vector<std::pair<std::string, position_type>>;
    using xyz_trajectory_type = std::vector<xyz_trajectory_type>;

  public:

    DCDReader()  = default;
    ~DCDReader() = default;

    void write(const std::string& fname, const trajectory_type& traj) const;
    void write(const std::string& fname, const xyz_trajectory_type& traj) const;

    void write(std::ostream& os, const trajectory_type& traj) const;
    void write(std::ostream& os, const xyz_trajectory_type& traj) const;

    void write_snapshot(std::ostream& os, const xyz_snapshot_type& ss) const;
    void write_snapshot(std::ostream& os, const snapshot_type& ss) const;
};

template<typename vectorT>
void XYZWriter<vectorT>::write(
        const std::string& fname, const trajectory_type& traj) const
{
    std::ofstream ofs(fname);
    if(not ofs.good())
        throw std::runtime_error("file open error: " + fname);

    for(auto iter = traj.cbegin(); iter != traj.cend(); ++iter)
        write_snapshot(ofs, *iter);
    return;
}

template<typename vectorT>
void XYZWriter<vectorT>::write(
        const std::string& fname, const xyz_trajectory_type& traj) const
{
    std::ofstream ofs(fname);
    if(not ofs.good())
        throw std::runtime_error("file open error: " + fname);

    for(auto iter = traj.cbegin(); iter != traj.cend(); ++iter)
        write_snapshot(ofs, *iter);
    return;
}

template<typename vectorT>
void XYZWriter<vectorT>::write(
        std::ostream& os, const trajectory_type& traj) const
{
    for(auto iter = traj.cbegin(); iter != traj.cend(); ++iter)
        write_snapshot(os, *iter);
    return;
}

template<typename vectorT>
void XYZWriter<vectorT>::write(
        std::ostream& os, const xyz_trajectory_type& traj) const
{
    for(auto iter = traj.cbegin(); iter != traj.cend(); ++iter)
        write_snapshot(os, *iter);
    return;
}

template<typename vectorT>
void XYZWriter<vectorT>::write_snapshot(
        std::ostream& os, const xyz_snapshot_type& ss) const
{
    os << ss.size() << std::endl;
    os << "generated by Coffee-mill" << std::endl;
    for(auto iter = ss.cbegin(); iter != ss.cend(); ++iter)
    {
        os << iter->first
           << iter->second[0] << " " << iter->second[1] << " "
           << iter->second[2] << std::endl;
    }
    return;
}

template<typename vectorT>
void XYZWriter<vectorT>::write_snapshot(
        std::ostream& os, const snapshot_type& ss) const
{
    os << ss.size() << std::endl;
    os << "generated by Coffee-mill" << std::endl;
    for(auto iter = ss.cbegin(); iter != ss.cend(); ++iter)
    {
        os << "CA    "
           << (*iter)[0] << " " << (*iter)[1] << " " << (*iter)[2] << std::endl;
    }
    return;   
}

}// mill
#endif //COFFEE_MILL_DCD_READER
