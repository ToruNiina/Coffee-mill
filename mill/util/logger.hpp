#ifndef COFFEE_MILL_UTIL_LOGGER_HPP
#define COFFEE_MILL_UTIL_LOGGER_HPP
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
    Logger(): filter_{{log_level::debug, false}, {log_level::info,  true},
                      {log_level::warn,  true},  {log_level::error, true}}
    {}
    ~Logger() = default;

    bool is_activated(const log_level lv) const {return filter_.at(lv);}

    void activate  (const log_level lv) {filter_[lv] = true;}
    void deactivate(const log_level lv) {filter_[lv] = false;}

  private:
    std::map<log_level, bool> filter_;
};
inline Logger logger;

template<typename ... Ts>
void error(Ts&& ... args) const
{
    if(not this->is_activated(log_level::error)) {return;}
    std::cerr << "\x1b[31mError:\x1b[0m ";
    (std::cerr << ... << args);
    std::cerr << std::flush;
    return;
}
template<typename ... Ts>
void warn(Ts&& ... args) const
{
    if(not this->is_activated(log_level::warn)) {return;}
    std::cerr << "\x1b[33mWarn :\x1b[0m ";
    (std::cerr << ... << args);
    std::cerr << std::flush;
    return;
}
template<typename ... Ts>
void info(Ts&& ... args) const
{
    if(not this->is_activated(log_level::info)) {return;}
    std::cerr << "\x1b[32mInfo :\x1b[0m ";
    (std::cerr << ... << args);
    std::cerr << std::flush;
    return;
}
template<typename ... Ts>
void debug(Ts&& ... args) const
{
    if(not this->is_activated(log_level::debug)) {return;}
    std::cerr << "\x1b[34mDebug:\x1b[0m ";
    (std::cerr << ... << args);
    std::cerr << std::flush;
    return;
}

} // log
} // mill
#endif// COFFEE_MILL_UTIL_LOGGER_HPP
