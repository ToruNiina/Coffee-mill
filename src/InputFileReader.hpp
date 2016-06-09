#ifndef COFFEE_MILL_INPUT_FILE_READER
#define COFFEE_MILL_INPUT_FILE_READER
#include <string>
#include <vector>
#include <map>

namespace coffeemill
{

class InputFileReader
{
  public:

    using key_type_            = std::string;
    using value_type_          = std::string;
    using element_type_        = std::pair<key_type_, value_type_>;
    using table_name_type_     = std::string;
    using table_type_          = std::map<key_type_, value_type_>;

    using parameter_type_      = std::string;
    using parameter_list_type_ = std::vector<parameter_type_>;
    using iterator             = typename parameter_list_type_::iterator;
    using const_iterator       = typename parameter_list_type_::const_iterator;

  public:

    InputFileReader(const std::string& fname)
        : filename_(fname),
          integer_elements{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
              '+', '-', '_'},
          floating_elements{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
              '+', '-', '_', 'e', 'E', '.'}
    {}
    ~InputFileReader() = default;

    void read();

    const table_type_& at(const table_name_type_& tablename) const
    {
        return tables_.at(tablename);
    }

    const value_type_& at(const table_name_type_& tablename,
                          const key_type_& key) const
    {
        return tables_.at(tablename).at(key);
    }

    const std::string& get_filename() const {return filename_;}
    const_iterator     cbegin()       const {return parameters_.cbegin();}
    const_iterator     cend()         const {return parameters_.cend();}

  protected:

    std::string read_line(std::ifstream& ifs) const;

    // string operation
    std::string remove_all  (const std::string& str, const char c) const;
    std::string remove_front(const std::string& str, const char c) const;
    std::string remove_back (const std::string& str, const char c) const;
    std::string remove_comment(const std::string& line) const;

    bool          is_table_line     (const std::string& line) const;
    bool          is_key_value_line (const std::string& line) const;
    std::string   get_table_name    (const std::string& line) const;
    element_type_ get_key_value_pair(const std::string& line) const;

    bool is_boolean(const std::string& val) const;
    bool is_integer(const std::string& val) const;
    bool is_real   (const std::string& val) const;
    bool is_string (const std::string& val) const;
    bool is_oneline_array  (const std::string& val) const;
    bool is_multiline_array(const std::string& val) const;
    bool is_closed(const std::string& val,
                   const char open, const char close) const;

  public:

    template<typename T_val>
    T_val get_as(const std::string& val) const;

    template<typename T_elem>
    std::vector<T_elem> get_as_list(const std::string& val) const
    {
        std::vector<std::string> splitted = this->split_list(val);
        std::vector<T_elem> retval(splitted.size());
        for(std::size_t i=0; i<splitted.size(); ++i)
        {
            retval.at(i) = get_as<T_elem>(splitted.at(i));
        }
        return retval;
    }

    std::vector<std::string> split_list(const std::string& val) const;

  private:

    const std::string filename_;

    // toml-like key-value table
    std::map<table_name_type_, table_type_> tables_;

    // ninfo-like parameters
    parameter_list_type_ parameters_;

    // for parsing
    const std::vector<char> integer_elements;
    const std::vector<char> floating_elements;

  private:

    class EndOfFile : public std::exception
    {
      public:
        EndOfFile(const std::string& str) : str_(str){}
        virtual ~EndOfFile() override = default;

        virtual const char* what() const throw() override {return str_.c_str();}

      private:
        std::string str_;
    };

};

}

#endif /*MJOLNIR_FILE_READER_BASE*/
