#ifndef COFFEE_MILL_INP_FILENAME
#define COFFEE_MILL_INP_FILENAME
#include <map>
#include "InpBase.hpp"

namespace coffeemill
{
//  <<<< filenames
//  path = ./data
//  filename = (required)
//  OUTPUT pdb crd velo movie dcd vdcd psf rst
//  path_para = ./para
//  path_pdb = ./pdb
//  path_ini = ./pdb
//  path_natinfo = ./ninfo
//  path_aicg = ./aicg
//  >>>> 
    class InpFileName : public InpBase
    {
    public:
        enum OUTPUT_TYPE
        {
            PDB,
            CRD,
            VELO,
            MOVIE,
            DCD,
            VDCD,
            PSF,
            RST
        };

    private:

        const int BLOCK_MAX_LINE;
        std::string out_path;
        std::string para_path;
        std::string pdb_path;
        std::string ini_path;
        std::string ninf_path;
        std::string aicg_path;
        std::string file_name;
        std::ifstream inpfile;
        std::map<OUTPUT_TYPE, bool> outfiles;
        std::map<OUTPUT_TYPE, std::string> outnames;

    public:

        InpFileName(int max = 100): InpBase(), BLOCK_MAX_LINE(max)
        {
            ;
        }

        InpFileName(const std::string& filename, int max = 100)
            : InpBase(), BLOCK_MAX_LINE(max), inpfile(filename)
        {
            ;
        }

        ~InpFileName(){}

        bool read_file(const std::string& filename);
        bool read_file();

        std::string get_out_path() const {return out_path;}
        std::string get_para_path() const {return para_path;}
        std::string get_pdb_path() const {return pdb_path;}
        std::string get_ini_path() const {return ini_path;}
        std::string get_ninf_path() const {return ninf_path;}
        std::string get_aicg_path() const {return aicg_path;}
        std::string get_file_name() const {return file_name;}
        std::map<OUTPUT_TYPE, bool> get_outfiles() const {return outfiles;}

        friend std::ostream& operator<<(std::ostream& os, const InpFileName& ifn);

    private:

        void read_block(std::ifstream& ifs);
        void set_output(std::istringstream& ls);
    };

    bool InpFileName::read_file(const std::string& filename)
    {
        if(already_read)
        {
            std::cout<<"Warning: trying to substitute filename for already object."<<std::endl;
        }
        already_read = true;

        if(inpfile.is_open())
        {
            std::cout << "Warning: trying to re-open file named "
                      << filename << std::endl;
        }

        inpfile.open(filename.c_str());

        while(!inpfile.eof())
        {
            std::string line;
            std::getline(inpfile, line);

            if(line.empty()) continue;
            if(line[0] == '\x2a') continue;

            if(line.substr(0, 14) == "<<<< filenames")
            {
                std::cout << "found block start line." << std::endl;
                read_block(inpfile);
            }
            
            return true;
        }
        return false;
    }

    bool InpFileName::read_file()
    {
        if(already_read)
        {
            std::cout<<"Warning: trying to substitute filename for already object."<<std::endl;
        }
        already_read = true;

        if(!inpfile.good())
        {
            throw std::invalid_argument("input file is not defined");
        }

        while(!inpfile.eof())
        {
            std::string line;
            std::getline(inpfile, line);

            if(line.empty()) continue;
            if(line[0] == '\x2a') continue;

            if(line.substr(0, 14) == "<<<< filenames")
            {
                std::cout << "found block start line." << std::endl;
                read_block(inpfile);
            }
            
            return true;
        }
        return false;
    }

    void InpFileName::read_block(std::ifstream& ifs)
    {
        std::string line;
        for(int i(0); i<BLOCK_MAX_LINE; ++i)
        {
            getline(ifs, line);
            if(line.empty()) continue;
            if(line[0] == '\x2a') continue;

            std::istringstream ls(line);
            std::string head;
            ls >> head;
            if(head == "path")
            {
                char equal;
                ls >> equal;
                if(equal != '=')
                {
                    std::cout << "Error: invalid format" << std::endl;
                    std::cout << line << std::endl;
                    throw std::invalid_argument("filename block :: invalid format in input file");
                }
                ls >> out_path;
                continue;
            }
            else if(head == "filename")
            {
                char equal;
                ls >> equal;
                if(equal != '=')
                {
                    std::cout << "Error: invalid format" << std::endl;
                    std::cout << line << std::endl;
                    throw std::invalid_argument("filename block :: invalid format in input file");
                }
                ls >> file_name;
                continue;
            }
            else if(head == "OUTPUT")
            {
                set_output(ls);
                continue;
            }
            else if(head == "path_para")
            {
                char equal;
                ls >> equal;
                if(equal != '=')
                {
                    std::cout << "Error: invalid format" << std::endl;
                    std::cout << line << std::endl;
                    throw std::invalid_argument("filename block :: invalid format in input file");
                }
                ls >> para_path;
                continue;
            }
            else if(head == "path_pdb")
            {
                char equal;
                ls >> equal;
                if(equal != '=')
                {
                    std::cout << "Error: invalid format" << std::endl;
                    std::cout << line << std::endl;
                    throw std::invalid_argument("filename block :: invalid format in input file");
                }
                ls >> pdb_path;
                continue;
            }else if(head == "path_ini")
            {
                char equal;
                ls >> equal;
                if(equal != '=')
                {
                    std::cout << "Error: invalid format" << std::endl;
                    std::cout << line << std::endl;
                    throw std::invalid_argument("filename block :: invalid format in input file");
                }
                ls >> ini_path;
                continue;
            }else if(head == "path_natinfo")
            {
                char equal;
                ls >> equal;
                if(equal != '=')
                {
                    std::cout << "Error: invalid format" << std::endl;
                    std::cout << line << std::endl;
                    throw std::invalid_argument("filename block :: invalid format in input file");
                }
                ls >> ninf_path;
                continue;
            }
            else if(head == "path_aicg")
            {
                char equal;
                ls >> equal;
                if(equal != '=')
                {
                    std::cout << "Error: invalid format" << std::endl;
                    std::cout << line << std::endl;
                    throw std::invalid_argument("filename block :: invalid format in input file");
                }
                ls >> aicg_path;
                continue;
            }
            else if(head == ">>>>")
            {
                return;
            }else
            {
                std::cout << "Warning: Unknown line in inp file filenames block." << std::endl;
                std::cout << line << std::endl;
                continue;
            }
        }
        throw std::invalid_argument("filename block in input file is too long(over 100 lines)");
    }
    
    void InpFileName::set_output(std::istringstream& ls)
    {
        std::string temp;

        while(!ls.eof() )
        {
            ls >> temp;

            if(temp == "pdb")
            {
                outfiles[PDB] = true;
                continue;
            }
            else if(temp == "crd")
            {
                outfiles[CRD] = true;
                continue;
            }
            else if(temp == "velo")
            {
                outfiles[VELO] = true;
                continue;
            }
            else if(temp == "movie")
            {
                outfiles[MOVIE] = true;
                continue;
            }
            else if(temp == "dcd")
            {
                outfiles[DCD] = true;
                continue;
            }
            else if(temp == "vdcd")
            {
                outfiles[VDCD] = true;
                continue;
            }
            else if(temp == "psf")
            {
                outfiles[PSF] = true;
                continue;
            }
            else if(temp == "rst")
            {
                outfiles[RST] = true;
                continue;
            }
            else
            {
                std::cout << "Error: Unknown output type." << std::endl;
                throw std::invalid_argument("Unknown Type");
            }
        }

        std::map<OUTPUT_TYPE, bool>::iterator iter;
        iter = outfiles.find(PDB);
        if(iter == outfiles.end()) outfiles[PDB] = false;
        iter = outfiles.find(CRD);
        if(iter == outfiles.end()) outfiles[CRD] = false;
        iter = outfiles.find(VELO);
        if(iter == outfiles.end()) outfiles[VELO] = false;
        iter = outfiles.find(MOVIE);
        if(iter == outfiles.end()) outfiles[MOVIE] = false;
        iter = outfiles.find(DCD);
        if(iter == outfiles.end()) outfiles[DCD] = false;
        iter = outfiles.find(VDCD);
        if(iter == outfiles.end()) outfiles[VDCD] = false;
        iter = outfiles.find(PSF);
        if(iter == outfiles.end()) outfiles[PSF] = false;
        iter = outfiles.find(RST);
        if(iter == outfiles.end()) outfiles[RST] = false;

        return;
    }

    std::ostream& operator<<(std::ostream& os, const std::map<InpFileName::OUTPUT_TYPE, bool> fmap)
    {
        if(fmap.at(InpFileName::PDB)) os << "pdb ";
        if(fmap.at(InpFileName::CRD)) os << "crd ";
        if(fmap.at(InpFileName::VELO)) os << "velo ";
        if(fmap.at(InpFileName::MOVIE)) os << "movie ";
        if(fmap.at(InpFileName::DCD)) os << "dcd ";
        if(fmap.at(InpFileName::VDCD)) os << "vdcd ";
        if(fmap.at(InpFileName::PSF)) os << "psf ";
        if(fmap.at(InpFileName::RST)) os << "rst ";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const InpFileName& ifn)
    {
        os << "<<<< filenames" <<std::endl;
        os << "path = " << ifn.out_path << std::endl;
        os << "filename = " << ifn.file_name << std::endl;
        bool output(false);
        for(unsigned int i(0); i < ifn.outfiles.size(); ++i)
        {
//          const InpFileName::OUTPUT_TYPE out_type(InpFileName::OUTPUT_TYPE(i));
            output = (output || ifn.outfiles.at(InpFileName::OUTPUT_TYPE(i)));
        }
        if(output)
        {
            os << "OUTPUT ";
            os << ifn.outfiles << std::endl;
        }
        if(!ifn.para_path.empty())
            os << "path_para = " << ifn.para_path << std::endl;
        if(!ifn.pdb_path.empty())
            os << "path_pdb = " << ifn.pdb_path << std::endl;
        if(!ifn.ini_path.empty())
            os << "path_ini = " << ifn.ini_path << std::endl;
        if(!ifn.ninf_path.empty())
            os << "path_natinfo = " << ifn.ninf_path << std::endl;
        if(!ifn.aicg_path.empty())
            os << "path_aicg = " << ifn.aicg_path << std::endl;
        os << ">>>>" << std::endl;
        return os;
    }

};

#endif//COFFEE_MILL_INP_FILENAME
