#ifndef COFFEE_MILL_UTIL_LOGGER_HPP
#define COFFEE_MILL_UTIL_LOGGER_HPP
#include <mill/util/color.hpp>
#include <vector>
#include <deque>
#include <map>
#include <optional>
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

template<typename T>
std::void_t<decltype(std::declval<std::ostringstream&>() << std::declval<T>())>
log_formatter(std::ostringstream& oss, T&& head)
{
    oss << std::forward<T>(head);
    return ;
}

template<typename T1, typename T2>
void log_formatter(std::ostringstream&, const std::pair<T1, T2>&);
template<typename T, typename Alloc>
void log_formatter(std::ostringstream&, const std::vector<T, Alloc>&);
template<typename T, typename Alloc>
void log_formatter(std::ostringstream&, const std::deque<T, Alloc>&);
template<typename T>
void log_formatter(std::ostringstream&, const std::optional<T>&);

template<typename T1, typename T2>
void log_formatter(std::ostringstream& oss, const std::pair<T1, T2>& p)
{
    oss << "pair[";
    log_formatter(oss, p.first);
    oss << ", ";
    log_formatter(oss, p.second);
    oss << "]";
    return ;
}
template<typename T, typename Alloc>
void log_formatter(std::ostringstream& oss, const std::vector<T, Alloc>& vec)
{
    oss << "vector[";
    for(std::size_t i=0; i<vec.size(); ++i)
    {
        if(i != 0) {oss << ", ";}
        log_formatter(oss, vec[i]);
    }
    oss << "]";
    return ;
}
template<typename T, typename Alloc>
void log_formatter(std::ostringstream& oss, const std::deque<T, Alloc>& deq)
{
    oss << "deque[";
    for(std::size_t i=0; i<deq.size(); ++i)
    {
        if(i != 0) {oss << ", ";}
        log_formatter(oss, deq[i]);
    }
    oss << "]";
    return ;
}
template<typename T>
void log_formatter(std::ostringstream& oss, const std::optional<T>& opt)
{
    if(opt)
    {
        oss << "optional[";
        log_formatter(oss, *opt);
        oss << "]";
    }
    else
    {
        oss << "optional[null]";
    }
    return ;
}

template<typename T, typename ... Ts>
std::string log_printer_helper(std::ostringstream& oss, T&& head, Ts&& ... args)
{
    log_formatter(oss, std::forward<T>(head));

    if constexpr (sizeof...(Ts) == 0)
    {
        return oss.str();
    }
    else
    {
        return log_printer_helper(oss, std::forward<Ts>(args)...);
    }
}

template<typename ... Ts>
std::string log_printer(Ts&& ... args)
{
    std::ostringstream oss;
    auto output = log_printer_helper(oss, std::forward<Ts>(args)...);

    const std::string prefix("     : ");

    auto found = output.find('\n');
    while(found != std::string::npos && found + 1 != output.size() &&
          output.substr(found+1, prefix.size()) != prefix)
    {
        output.insert(found + 1, prefix);
        found = output.find('\n', found + 1);
    }
    return output;
}

template<typename Exception = std::runtime_error, typename ... Ts>
[[noreturn]] void fatal(Ts&& ... args)
{
    using namespace std::literals::string_literals;
    const auto msg = log_printer(std::forward<Ts>(args)...);

    std::cerr << color::bold << color::red << "FATAL: " << color::clear
              << msg << std::endl;
    throw Exception("FATAL: "s + msg);
}

template<typename ... Ts>
void error(Ts&& ... args)
{
    if(not logger.is_activated(level::error)) {return;}

    std::cerr << color::bold << color::red << "Error: " << color::clear;
    std::cerr << log_printer(std::forward<Ts>(args)...) << std::endl;
    return;
}
template<typename ... Ts>
void warn(Ts&& ... args)
{
    if(not logger.is_activated(level::warn)) {return;}

    std::cerr << color::bold << color::yellow << "Warn : " << color::clear;
    std::cerr << log_printer(std::forward<Ts>(args)...) << std::endl;
    return;
}
template<typename ... Ts>
void info(Ts&& ... args)
{
    if(not logger.is_activated(level::info)) {return;}

    std::cerr << color::bold << color::green << "Info : " << color::clear;
    std::cerr << log_printer(std::forward<Ts>(args)...) << std::endl;
    return;
}
template<typename ... Ts>
void debug(Ts&& ... args)
{
    if(not logger.is_activated(level::debug)) {return;}

    std::cerr << color::bold << color::blue << "Debug: " << color::clear;
    std::cerr << log_printer(std::forward<Ts>(args)...) << std::endl;
    return;
}

} // log
} // mill
#endif// COFFEE_MILL_UTIL_LOGGER_HPP
