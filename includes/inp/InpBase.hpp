#ifndef COFFEE_MILL_INP_BASE
#define COFFEE_MILL_INP_BASE
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <locale>
#include <algorithm>

namespace coffeemill
{
    class InpBase
    {
    protected:

        bool already_read;

    public:

        InpBase(): already_read(false){}
        ~InpBase(){}

    protected:

        std::vector<std::string> stringsplit(std::string str, char d, int b=0);

        static bool eq_ignorecase(const std::string& lhs,
                                  const std::string& rhs);

    private:

        static bool eq_char_ignorecase(const char lhs, const char rhs);
    };

    std::vector<std::string>
    InpBase::stringsplit(std::string str, char d, int b)
    {
        std::vector<std::string> retval;
        std::string::iterator iter = str.begin() + b;
        while(iter != str.end())
        {
            std::string::iterator lastiter = iter;
            while(lastiter != str.end())
            {
                if(*lastiter == d) break;
                ++lastiter;
            }
            retval.push_back(std::string(iter, lastiter));
            if(lastiter != str.end()) ++lastiter;
            iter = lastiter;
        }
        return retval;
    }

    bool InpBase::eq_ignorecase(const std::string& lhs, const std::string& rhs)
    {
        return lhs.size() == rhs.size() &&
               std::equal(lhs.begin(), lhs.end(), rhs.begin(),
                          eq_char_ignorecase);
    }

    bool InpBase::eq_char_ignorecase(const char lhs, const char rhs)
    {
        std::locale loc;
        return std::tolower(lhs, loc) == std::tolower(rhs, loc);
    }
}
#endif //COFFEE_MILL_INP_BASE
