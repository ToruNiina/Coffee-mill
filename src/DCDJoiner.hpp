#ifndef COFFEE_MILL_DCDJOINER
#define COFFEE_MILL_DCDJOINER
#include "DCDReader.hpp"
#include "DCDWriter.hpp"
#include "InputFileReader.hpp"

namespace coffeemill
{

class DCDJoiner
{
  public:
    DCDJoiner(const InputFileReader& input)
        : redundancy_(input.get_as<bool>(
                      input.at("information", "redundancy"))),
          output_(input.get_as<std::string>(
                      input.at("information", "output"))),
          filenames_(input.get_as_list<std::string>(
                      input.at("information", "dcdfiles")))
    {}
    ~DCDJoiner() = default;

    void join() const;

  private:
    bool                     redundancy_;
    std::string              output_;
    std::vector<std::string> filenames_;
};


}

#endif /* COFFEE_MILL_DCDJOINER */
