#ifndef COFFEE_MILL_UTIL_LOGGER_HPP
#define COFFEE_MILL_UTIL_LOGGER_HPP
#include <mill/util/isatty.hpp>
#include <map>
#include <iostream>
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

template<typename ... Ts>
void error(Ts&& ... args)
{
    if(not logger.is_activated(level::error)) {return;}

    if(isatty(std::cerr)) {std::cerr << "\x1b[31mError:\x1b[0m ";}
    else                  {std::cerr <<         "Error: ";}
    (std::cerr << ... << args);
    std::cerr << std::endl; // logs should be flashed every time
    return;
}
template<typename ... Ts>
void warn(Ts&& ... args)
{
    if(not logger.is_activated(level::warn)) {return;}

    if(isatty(std::cerr)) {std::cerr << "\x1b[33mWarn :\x1b[0m ";}
    else                  {std::cerr <<         "Warn : ";}
    (std::cerr << ... << args);
    std::cerr << std::endl; // logs should be flashed every time
    return;
}
template<typename ... Ts>
void info(Ts&& ... args)
{
    if(not logger.is_activated(level::info)) {return;}

    if(isatty(std::cerr)) {std::cerr << "\x1b[32mInfo :\x1b[0m ";}
    else                  {std::cerr <<         "Info : ";}
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

} // log
} // mill
#endif// COFFEE_MILL_UTIL_LOGGER_HPP
