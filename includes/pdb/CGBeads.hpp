#ifndef COFFEE_MILL_CG_BEAD
#define COFFEE_MILL_CG_BEAD
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <memory>
#include "../defs/Defs.hpp"

namespace coffeemill
{
    class CGBead
    {
    public:
        CGBead(){}
        ~CGBead(){}

        void get_line(std::string& line);

        char        get_chainID()  const {return chainID;}
        int         get_imp()      const {return imp;}
        int         get_iResNum()  const {return iResNum;}
        double      get_coordx()   const {return x;}
        double      get_coordy()   const {return y;}
        double      get_coordz()   const {return z;}
        std::string get_beadname() const {return bead;}
        std::string get_seq()      const {return seq;}

        void set_chainID (char id)       {chainID = id;}
        void set_imp     (int im)        {imp = im;}
        void set_iResNum (int rn)        {iResNum = rn;}
        void set_coordx  (double _x)     {x = _x;}
        void set_coordy  (double _y)     {y = _y;}
        void set_coordz  (double _z)     {z = _z;}
        void set_beadname(std::string bn){bead = bn;}
        void set_seq     (std::string sq){seq = sq;}
        void set_headder (std::string hd){head = hd;}

        friend std::ostream& operator<<(std::ostream& os, const CGBead& a);

    private:

        void line_input(std::string line);
        std::string DeleteSpace(const std::string& str);

    private:

        char chainID;
        int imp;//mass particle number
        int iResNum;//residue sequence number
        double x, y, z;
        std::string head;//hedder. like ATOM, HEATOM
        std::string bead;//bead type. like CA(carbon alpha), DB(DNA base), etc.
        std::string seq;//sequence. like ALA(alanine), DG(DNA guanine), etc.
    };
    typedef std::shared_ptr<CGBead> BeadSptr;

    void CGBead::get_line(std::string& line)
    {
        if(line.substr(0,6) == "ATOM  " ||
           line.substr(0,6) == "HEATOM")
        {
            line_input(line);
            return;
        }

#ifdef _DEBUG
        std::cout << "unknown line: " << line << std::endl;
#endif //_DEBUG
        throw std::invalid_argument("CGBead: invalid linetype");
    }

    void CGBead::line_input(std::string line)
    {/* typical example
0         1         2         3         4         5
ATOM    880 DB   DG  B 147     -34.372   7.815  22.375
ATOM    881  CA  ALA C   1      50.743   2.865 -23.747
^^^^^^*****_^^^^_^^^_^_***____************************
        */
        this->head       = DeleteSpace(line.substr(0,6));
        this->imp        = std::stoi(line.substr(6,5));
        this->bead       = DeleteSpace(line.substr(12,4));
        this->seq        = DeleteSpace(line.substr(17,3));
        this->chainID    = line.at(21);
        this->iResNum    = std::stoi(line.substr(22, 4));
        this->x          = std::stod(line.substr(30, 8));
        this->y          = std::stod(line.substr(38, 8));
        this->z          = std::stod(line.substr(46, 8));

        return;
    }

    std::string CGBead::DeleteSpace(const std::string& str)
    {
        std::string retval = str;
        std::size_t iter;
        while((iter = retval.find_first_of(" ")) != std::string::npos)
        {
            retval.erase(iter, 1);
        }
        return retval;
    }

    std::ostream& operator<<(std::ostream& os, const CGBead& a)
    {
        os << std::setw(6) << std::left << a.head;
        os << std::setw(5) << std::right << a.imp;
        os << " ";
        if(a.bead == "CA")
        {
            os << std::setw(3) << a.bead;
        }
        else if(a.bead == "DB" || a.bead == "DS" || a.bead == "DP")
        {
            os << std::setw(3) << std::left << a.bead;
        }
        else
        {
            os << std::setw(3) << a.bead;
        }
        os << "  ";
        os << std::setw(4) << std::left << a.seq;
        os << std::setw(1) << a.chainID;
        os << std::setw(4) << std::right << a.iResNum;
        os << "    ";
        os << std::setw(8) << std::fixed << std::setprecision(3) << a.x;
        os << std::setw(8) << std::fixed << std::setprecision(3) << a.y;
        os << std::setw(8) << std::fixed << std::setprecision(3) << a.z;
        return os;
    }

    bool less_imp(const BeadSptr& lhs, const BeadSptr& rhs)
    {//lhs.imp < rhs.imp
        return lhs->get_imp() < rhs->get_imp();
    }
}

#endif//COFFEE_MILL_CG_BEADS
