/*!
  @file DCDJoiner.hpp
  @brief definition of a class that joins some dcd files.

  DCDJoiner joins some dcdfiles to one dcd file. This help the analysis or 
  visualization of cafemol restarting simulation.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 16:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCDJOINER
#define COFFEE_MILL_DCDJOINER
#include "DCDReader.hpp"
#include "DCDWriter.hpp"
#include "InputFileReader.hpp"

namespace coffeemill
{

//! DCD Joiner class.
class DCDJoiner
{
  public:
    //! ctor.
    /*!
     *  ctor. set member values from input file reader.
     *  @param input InputFileReader. it is required that input already read the file.
     *  @sa    InputFileReader
     */
    explicit DCDJoiner(const InputFileReader& input)
        : redundancy_(input.get_as<bool>(
                      input.at("information", "redundancy"))),
          output_(input.get_as<std::string>(
                      input.at("information", "output"))),
          filenames_(input.get_as_list<std::string>(
                      input.at("information", "dcdfiles")))
    {}
    //! dtor.
    ~DCDJoiner() = default;

    //! join the files
    void join() const;

  private:
    /*!
     * redundancy parameter. if true, join the dcdfiles as they are.
     * if false, remove initial snapshot from the files except for 1st file.
     */
    bool                     redundancy_;
    std::string              output_;     //!< output filename
    std::vector<std::string> filenames_;  //!< dcd filenames. join as this order.
};


}

#endif /* COFFEE_MILL_DCDJOINER */
