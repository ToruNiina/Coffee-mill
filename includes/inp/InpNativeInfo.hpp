#ifndef COFFEE_MILL_INP_NINFO
#define COFFEE_MILL_INP_NINFO
#include "InpBase.hpp"
#include <cstdlib>
#include <map>

namespace coffeemill
{
//  <<<< Native_info_simN
//  NINFO(all/all) filename.ninfo
//
//  or
//
//  NINFO(1/1) 1
//  NINFO(1/2) 2
//  ...
//  NINFO(M/M)
//  1 = file1.ninfo
//  ...
//  M^2 = fileM_M.ninfo
//  >>>> 

    typedef std::pair<int, int> iUnits;

    class InpNativeInfo : public InpBase
    {
    public:
        
        enum STYLE
        {
            ALL_IN_ONE,
            ONE_BY_ONE
        };

    private:

        int simN;
        STYLE style;
        std::ifstream inpfile;
        std::map<std::pair<int, int>, std::string> ninfo_files;
       //if (all/all) -> pair(0,0)

    public:

        InpNativeInfo(int N = 1): InpBase(), simN(N)
        {
            ;
        }
        InpNativeInfo(const std::string& filename, int N = 1)
            : InpBase(), simN(N), inpfile(filename)
        {
            ;
        }
        ~InpNativeInfo(){}

        bool read_file(const std::string& file_name);
        bool read_file();

        std::map<std::pair<int, int>, std::string>& get_files()
        {
            return ninfo_files;
        }

    private:

        void read_block(std::ifstream& ifs);

    };

    bool InpNativeInfo::read_file(const std::string& file_name)
    {
        if(already_read)
            std::cout << "Warning: trying to substitute ninfo for already initiated object." 
                      << std::endl;
        already_read = true;

        if(inpfile.is_open())
        {
            std::cout << "Warning: trying to re-open file named " << file_name
                      << std::endl;
        }
        inpfile.open(file_name);
        bool read(false);
        
        while(!inpfile.eof())
        {
            std::string line;
            std::getline(inpfile, line);

            if(line.empty()) continue;
            if(line[0] == '\x2a') continue;

            if(eq_ignorecase(line.substr(0,20), "<<<< native_info_sim"))
            {
                read = true;
                std::string temp(line, 20);
                simN = std::stoi(temp);
                read_block(inpfile);
            }
        }
        return read;
    }

    bool InpNativeInfo::read_file()
    {
        if(already_read)
            std::cout << "Warning: trying to substitute ninfo for already initiated object." 
                      << std::endl;
        already_read = true;

        if(!inpfile.good())
        {
            throw std::invalid_argument("file open error");
        }
        bool read(false);
        
        while(!inpfile.eof())
        {
            std::string line;
            std::getline(inpfile, line);

            if(line.empty()) continue;
            if(line[0] == '\x2a') continue;

            if(eq_ignorecase(line.substr(0,20), "<<<< native_info_sim"))
            {
                read = true;
                std::string temp(line, 20);
                simN = std::stoi(temp);
                read_block(inpfile);
            }
        }
        return read;
    }

    void InpNativeInfo::read_block(std::ifstream& ifs)
    {
        while(!ifs.eof())
        {
            std::string line;
            std::getline(ifs, line);
            if(line.empty()) continue;
            if(line[0] == '\x2a') continue;

            if(eq_ignorecase(line.substr(0,14), "NINFO(all/all)"))
            {
                style = ALL_IN_ONE;
                iUnits iunits = std::make_pair(0,0);
                std::vector<std::string> subline = stringsplit(line, ' ');
                for(auto iter = subline.begin(); iter != subline.end(); ++iter)
                {
                    if(iter->substr(iter->size()-6, 6) == "\x2eninfo")
                    {
                        ninfo_files[iunits] = *iter;
                        return;
                    }
                }
                throw std::invalid_argument("read_block: cant find ninfo file");
            }
            else if(line.substr(0,6) == "NINFO(")
            {
                if(style != ONE_BY_ONE)
                    style = ONE_BY_ONE;
                std::vector<std::string> iunit1_str
                    = stringsplit(line, '/', 6);
                std::vector<std::string> iunit2_str
                    = stringsplit(iunit1_str.at(1), ')');

                int iunit1(std::stoi(iunit1_str.at(0)));
                int iunit2(std::stoi(iunit2_str.at(0)));

                std::vector<std::string> filename
                    = stringsplit(line, ' ');
                iUnits iunits = std::make_pair(iunit1, iunit2);
                for(auto itr = filename.begin(); itr != filename.end(); ++itr)
                {
                    if(itr->substr(itr->size()-6, 6) == "\x2eninfo")
                    {
                        ninfo_files[iunits] = *itr;
                        break;
                    }
                }
            }
            else if(line.substr(0,4) == ">>>>")
            {
                break;
            }
            else
            {
                std::cout << "Error: Unknown line" << std::endl;
                std::cout << line << std::endl;
            }
        }
        return;
    }

}
#endif//COFFEE_MILL_INP_NINFO
