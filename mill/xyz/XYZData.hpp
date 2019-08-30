/*!
  @file XYZData.hpp
  @brief definition of a data struct for xyz file.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2017-03-23 01:00
  @copyright Toru Niina 2017 on MIT License
*/

#ifndef COFFEE_MILL_XYZ_DATA_HPP
#define COFFEE_MILL_XYZ_DATA_HPP
#include <utility>
#include <vector>
#include <string>

namespace mill
{

//! XYZFrame contains one frame.
template<typename vectorT>
struct XYZFrame
{
  public:
    using vector_type         = vectorT;
    using position_type       = vector_type;
    using string_type         = std::string;

    std::string comment;
    std::vector<std::pair<std::string, position_type>> particles;
};

}// mill
#endif// COFFEE_MILL_XYZ_DATA
