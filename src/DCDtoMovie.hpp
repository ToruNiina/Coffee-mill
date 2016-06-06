#ifndef COFFEE_MILL_DCD_TO_MOVIE
#define COFFEE_MILL_DCD_TO_MOVIE
#include <string>
#include "InputFileReader.hpp"

namespace coffeemill
{

class DCDtoMovie
{
  public:
    DCDtoMovie(const InputFileReader& input)
        : pymol_format_(input.get_as<bool>(
                      input.at("information", "pymol"))),
          dcdfile_(input.get_as<std::string>(
                      input.at("information", "dcdfile"))),
          pdbfile_(input.get_as<std::string>(
                      input.at("information", "reference"))),
          output_(input.get_as<std::string>(
                      input.at("information", "output")))
    {}
    ~DCDtoMovie() = default;

    void convert() const;

  private:

    bool        pymol_format_;
    std::string dcdfile_;
    std::string pdbfile_;
    std::string output_;
};

}

#endif /* COFFEE_MILL_DCD_TO_MOVIE */
