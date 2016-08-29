/*!
  @file DCDtoMovie.hpp
  @brief definition of a class that converts dcd file into movie file

  definition of a class DCDtoMovie. this class output trajectory data of
  dcd file into movie file (an array of pdb model) using the information
  included in pdb file. the pdb file should contain only one model and
  same number of particle in dcd snapshot.
  
  @author Toru Niina (niina.toru.68u@gmail.com)
  @date 2016-06-09 15:00
  @copyright Toru Niina 2016 on MIT License
*/

#ifndef COFFEE_MILL_DCD_TO_MOVIE
#define COFFEE_MILL_DCD_TO_MOVIE
#include <string>
#include "InputFileReader.hpp"

namespace coffeemill
{

//! DCDtoMovie class.
/*!
 *  write movie file using particle information included in pdb file and 
 *  trajectory data in dcd file.
 */
class DCDtoMovie
{
  public:

    //! ctor
    /*!
     *  ctor. set member values from input file reader.
     *  @param input InputFileReader. it is required that input already read the file.
     *  @sa    InputFileReader
     */
    explicit DCDtoMovie(const InputFileReader& input)
        : pymol_format_(input.get_as<bool>(
                      input.at("information", "pymol"))),
          dcdfile_(input.get_as<std::string>(
                      input.at("information", "dcdfile"))),
          pdbfile_(input.get_as<std::string>(
                      input.at("information", "reference"))),
          output_(input.get_as<std::string>(
                      input.at("information", "output")))
    {}
    //! dtor
    ~DCDtoMovie() = default;

    //! converts dcd file to movie file.
    void convert() const;

  private:

    /*!
     * if true, output movie file become pymol readable format(like NMR pdb file).
     * if false, output movie file become same as cafemol movie.
     */
    bool        pymol_format_; //!< whether output format is pymol readable or not
    std::string dcdfile_; //!< filename of input dcdfile
    std::string pdbfile_; //!< filename of input pdbfile
    std::string output_;  //!< filename of output movie file
};

}

#endif /* COFFEE_MILL_DCD_TO_MOVIE */
