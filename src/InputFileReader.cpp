#include "InputFileReader.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace coffeemill
{

void InputFileReader::read()
{
    std::ifstream ifs(this->filename_);
    if(!ifs.good())
        throw std::runtime_error("file open error: " + this->filename_);

    std::string tablename("unnamed");
    table_type_ table;
    bool is_parameter_table = false;
    while(!ifs.eof())
    {
        try
        {
            const std::string line = this->read_line(ifs);
            if(is_table_line(line))
            {
                tables_.emplace(tablename, table);
                tablename = get_table_name(line);
                table.clear();

                if(tablename == "parameters")
                    is_parameter_table = true;
                else if(is_parameter_table)
                    is_parameter_table = false;
            }
            else if(is_key_value_line(line))
            {
                auto pair = get_key_value_pair(line);

                if(is_multiline_array(pair.second))
                {
                    while(!ifs.eof())
                    {
                        try{
                            const std::string arrayline = this->read_line(ifs);
                            pair.second += arrayline;
                            if(is_closed(pair.second, '[', ']'))
                            {
                                table.emplace(pair.first, pair.second);
                                break;
                            }
                        }//try
                        catch(EndOfFile& excpt)
                        {
                            throw std::invalid_argument(
                                    "invalid syntax in array definition: " + 
                                    pair.first + " = " + pair.second);
                        }
                    }
                }
                else
                {
                    table.emplace(pair);
                }
            }
            else if(is_parameter_table)
            {
                parameters_.push_back(line);
            }
            else
            {
                throw std::runtime_error("unknown line: " + line);
            }
        }
        catch(EndOfFile& excpt)
        {
            if(!is_parameter_table)
                tables_.emplace(tablename, table);
            break;
        }
    }

    ifs.close();
    return;
}


std::string InputFileReader::read_line(std::ifstream& ifs) const
{
    while(!ifs.eof())
    {
        std::string line;
        std::getline(ifs, line);
        line = remove_front(line, ' ');
        line = remove_comment(line);
        if(line.empty()) continue;

        return line;
    }
    throw EndOfFile("eof");
}

std::string InputFileReader::remove_all(const std::string& line, const char c) const
{
    std::string retval;
    for(auto iter = line.cbegin(); iter != line.cend(); ++iter)
    {
        if(*iter != c)
            retval += *iter;
    }
    return retval;
}

std::string InputFileReader::remove_front(const std::string& line, const char c) const
{
    if(line.empty())
        return line;

    std::string retval(line);
    while(retval.front() == c)
        retval.erase(retval.begin());
    return retval;
}

std::string InputFileReader::remove_back(const std::string& line, const char c) const
{
    if(line.empty())
        return line;

    std::string retval(line);
    while(retval.back() == c)
        retval.pop_back();
    return retval;
}

std::string InputFileReader::remove_comment(const std::string& line) const
{
    if(line.empty())
        return line;

    std::string retval;
    auto iter = line.cbegin();
    while(iter != line.cend() && *iter != '#')
    {
        retval += *iter;
        ++iter;
    }
    return remove_back(retval, ' ');
}

bool
InputFileReader::is_table_line(const std::string& line) const
{
    if(line.empty()) return false;
    return line.front() == '[' && line.back() == ']';
}

bool
InputFileReader::is_key_value_line(const std::string& line) const
{
    if(line.empty()) return false;
    std::istringstream iss(line);
    std::string key;
    char eq;
    iss >> key >> eq;
    return eq == '=';
}

std::string
InputFileReader::get_table_name(const std::string& line) const
{
    std::istringstream iss(line);
    iss >> std::ws;
    std::string ln;
    std::getline(iss, ln);

    std::string retval;
    auto iter = ln.cbegin();
    iter++; //XXX: the line is like this -> "[foobar]"
    while(*iter != ']')
    {
        if(iter == ln.cend())
            throw std::invalid_argument("[] not closed");
        retval += *iter;
        ++iter;
    }

    return retval;
}

std::pair<std::string, std::string>
InputFileReader::get_key_value_pair(const std::string& line) const
{
    std::istringstream iss(line);
    std::string key, value;
    char        equal;
    iss >> key >> equal;
    iss >> std::ws;
    std::getline(iss, value);

    return std::make_pair(key, value);
}

bool InputFileReader::is_boolean(const std::string& val) const
{
    return val == "true" || val == "false";
}

bool InputFileReader::is_integer(const std::string& val) const
{
    for(auto iter = val.cbegin(); iter != val.cend(); ++iter)
    {
        const char elem = *iter;
        auto found =
            std::find(integer_elements.cbegin(), integer_elements.cend(), elem);
        if(found == integer_elements.cend()) return false;
    }
    return true;
}

bool InputFileReader::is_real(const std::string& val) const
{
    for(auto iter = val.cbegin(); iter != val.cend(); ++iter)
    {
        const char elem = *iter;
        auto found =
            std::find(floating_elements.cbegin(), floating_elements.cend(), elem);
        if(found == floating_elements.cend()) return false;
    }
    return true;
}

bool InputFileReader::is_string(const std::string& val) const
{
    return (val.front() == '\'' && val.back() == '\'') ||
           (val.front() == '\"' && val.back() == '\"');
}

bool InputFileReader::is_oneline_array(const std::string& val) const
{
    return val.at(0) == '[' && is_closed(val, '[', ']');
}

bool InputFileReader::is_multiline_array(const std::string& val) const
{
    return val.at(0) == '[' && !(is_closed(val, '[', ']'));
}

bool InputFileReader::is_closed(const std::string& val,
                               const char open, const char close) const
{
    bool is_open = false;
    unsigned int counter = 0;
    for(auto iter = val.cbegin(); iter != val.cend(); ++iter)
    {
        if(*iter == open)
        {
            if(!is_open)
                is_open = true;
            if(is_open)
                ++counter;
        }
        else if(*iter == close)
        {
            if(!is_open)
                throw std::invalid_argument("syntax error: " + val);
            else
                --counter;
        }
    }

    return is_open && counter == 0;
}

template<>
bool InputFileReader::get_as<bool>(const std::string& val) const
{
    if(val == "true") return true;
    else if(val == "false") return false;
    else throw std::invalid_argument("not boolean value: " + val);
}

template<>
int InputFileReader::get_as<int>(const std::string& val) const
{
    const std::string parsed = remove_all(val, '_');
    return std::stoi(parsed);
}

template<>
long InputFileReader::get_as<long>(const std::string& val) const
{
    const std::string parsed = remove_all(val, '_');
    return std::stol(parsed);
}

template<>
long long InputFileReader::get_as<long long>(const std::string& val) const
{
    const std::string parsed = remove_all(val, '_');
    return std::stoll(parsed);
}

template<>
unsigned int InputFileReader::get_as<unsigned int>(const std::string& val) const
{
    const std::string parsed = remove_all(val, '_');
    return std::stoul(parsed);
}

template<>
std::size_t InputFileReader::get_as<std::size_t>(const std::string& val) const
{
    const std::string parsed = remove_all(val, '_');
    return std::stoull(parsed);
}

template<>
double InputFileReader::get_as<double>(const std::string& val) const
{
    const std::string parsed = remove_all(val, '_');
    return std::stod(parsed);
}

template<>
std::string InputFileReader::get_as<std::string>(const std::string& val) const
{
    return val.substr(1, val.size() - 2);
}

std::vector<std::string>
InputFileReader::split_list(const std::string& val) const
{
    if(val.empty()) return std::vector<std::string>{val};

    std::string array_inside = val.substr(1, val.size() - 2);

    std::string elem;
    std::vector<std::string> retval;

    unsigned int brace_counter = 0;
    for(auto iter = array_inside.cbegin(); iter != array_inside.cend(); ++iter)
    {
        if(*iter == '[')
        {
            ++brace_counter;
            elem += *iter;
        }
        else if(*iter == ']')
        {
            --brace_counter;
            elem += *iter;
        }
        else if(*iter == ',' && brace_counter == 0)
        {
            elem = remove_front(elem, ' ');
            elem = remove_back(elem, ' ');
            retval.push_back(elem);
            elem.clear();
        }
        else
        {
            elem += *iter;
        }
    }
    if(!elem.empty())
    {
        elem = remove_front(elem, ' ');
        elem = remove_back(elem, ' ');
        retval.push_back(elem);
    }
    return retval;
}

}
