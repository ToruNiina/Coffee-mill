#ifndef COFFEE_MILL_UTIL_LOGGER_HPP
#define COFFEE_MILL_UTIL_LOGGER_HPP
#include <mill/util/color.hpp>
#include <vector>
#include <deque>
#include <map>
#include <iostream>
#include <sstream>
#include <cstdint>

namespace mill
{
namespace log
{
enum class level : std::uint8_t
{
    error,
    warn ,
    info ,
    debug,
};

struct Logger
{
    Logger(): filter_{{level::debug, false}, {level::info,  true},
                      {level::warn,  true},  {level::error, true}}
    {}
    ~Logger() = default;

    bool is_activated(const level lv) const {return filter_.at(lv);}

    void activate  (const level lv) {filter_[lv] = true;}
    void deactivate(const level lv) {filter_[lv] = false;}

  private:
    std::map<level, bool> filter_;
};
inline Logger logger;

template<typename T, typename ... Ts>
std::string fatal_printer(std::ostringstream& oss, T&& head, Ts&& ... args)
{
    if(oss.str().back() == '\n')
    {
        //     "FATAL: "
        oss << "     : ";
    }
    oss << std::forward<T>(head);

    if constexpr (sizeof...(Ts) == 0)
    {
        return oss.str();
    }
    else
    {
        return fatal_printer(oss, std::forward<Ts>(args)...);
    }
}

template<typename Exception = std::runtime_error, typename ... Ts>
[[noreturn]] void fatal(Ts&& ... args)
{
    using namespace std::literals::string_literals;

    std::ostringstream oss;
    const auto msg = fatal_printer(oss, std::forward<Ts>(args)...);
    std::cerr << color::bold << color::red << "FATAL: " << color::clear
              << msg << std::endl;

    throw Exception("FATAL: "s + msg);
}

template<typename ... Ts>
void error(Ts&& ... args)
{
    if(not logger.is_activated(level::error)) {return;}

    std::cerr << color::bold << color::red << "Error: " << color::clear;
    (std::cerr << ... << args);
    std::cerr << std::endl; // logs should be flashed every time
    return;
}
template<typename ... Ts>
void warn(Ts&& ... args)
{
    if(not logger.is_activated(level::warn)) {return;}

    std::cerr << color::bold << color::yellow << "Warn : " << color::clear;
    (std::cerr << ... << args);
    std::cerr << std::endl; // logs should be flashed every time
    return;
}
template<typename ... Ts>
void info(Ts&& ... args)
{
    if(not logger.is_activated(level::info)) {return;}

    std::cerr << color::bold << color::green << "Info : " << color::clear;
    (std::cerr << ... << args);
    std::cerr << std::endl; // logs should be flashed every time
    return;
}
template<typename ... Ts>
void debug(Ts&& ... args)
{
    if(not logger.is_activated(level::debug)) {return;}

    if(isatty(std::cerr)) {std::cerr << "\x1b[34mDebug:\x1b[0m ";}
    else                  {std::cerr <<         "Debug: ";}
    (std::cerr << ... << args);
    std::cerr << std::endl; // logs should be flashed every time
    return;
}

// ----------------------------------------------------------------------------
// log output operator

template<typename T, typename Alloc>
std::ostream& operator<<(std::ostream& os, const std::vector<T, Alloc>& vec)
{
    os << "vector[";
    for(std::size_t i=0; i<vec.size(); ++i)
    {
        if(i != 0) {os << ", ";}
        os << vec[i];
    }
    os << "]";
    return os;
}
template<typename T, typename Alloc>
std::ostream& operator<<(std::ostream& os, const std::deque<T, Alloc>& deq)
{
    os << "deque[";
    for(std::size_t i=0; i<deq.size(); ++i)
    {
        if(i != 0) {os << ", ";}
        os << deq[i];
    }
    os << "]";
    return os;
}


} // log
} // mill
#endif// COFFEE_MILL_UTIL_LOGGER_HPP
