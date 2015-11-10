#ifndef COFFEE_MILL_NINFO_BOND
#define COFFEE_MILL_NINFO_BOND
#include "NinfoBase.hpp"

namespace coffeemill
{
    class BondLine : public LineBase
    {
    public:
        int ibd, iunit1, iunit2;
        int imp1, imp2, imp1un, imp2un;
        double bd_nat, factor_bd, correct_bd_mgo, coef_bd;
        std::string bdtype;
    public:
        BondLine(): LineBase(),
            ibd(0), iunit1(0), iunit2(0), imp1(0), imp2(0), 
            imp1un(0), imp2un(0), bd_nat(0e0), factor_bd(0e0),
            correct_bd_mgo(0e0), coef_bd(0e0), bdtype(){}
        ~BondLine(){}
        LineType readline(std::ifstream& file);
        void read_stream(std::istream& is);
        void write_stream(std::ostream& os);

        int get_index() const {return ibd;}
        int get_iunit1() const {return iunit1;}
        int get_iunit2() const {return iunit2;}
        std::vector<int> get_imps() const {return std::vector<int>({imp1, imp2});}
        std::vector<int> get_impuns() const {return std::vector<int>({imp1un, imp2un});}
        double get_nat() const {return bd_nat;}
        double get_factor() const {return factor_bd;}
        double get_mgo() const {return correct_bd_mgo;}
        double get_coef() const {return coef_bd;}
        std::string get_type() const {return bdtype;}
        std::string get_ClassName() const {return "BondLine";}
        BlockType get_BlockType() const {return N_BOND;}

        void set_index(int i){ibd = i;}
        void set_iunit1(int i){iunit1 = i;}
        void set_iunit2(int i){iunit2 = i;}
        void set_imps(std::vector<int> imps)
        {
            if(imps.size() < 2)
                throw std::invalid_argument("there are no enough argument");
            if(imps.size() != 2) 
                std::cout << "Warning: set_imps has more than 2 argument in BondLine"
                          << std::endl;
            imp1 = imps.at(1);
            imp2 = imps.at(2);
            return;
        }
        void set_impuns(std::vector<int> impuns)
        {
            if(impuns.size() < 2)
                throw std::invalid_argument("there are no enough argument");

            if(impuns.size() != 2) 
                std::cout << "Warning: set_impuns has more than 2 argument in BondLine"
                          << std::endl;
            imp1un = impuns.at(1);
            imp2un = impuns.at(2);
            return;
        }
        void set_nat(double n){ bd_nat = n;}
        void set_factor(double f){factor_bd = f;}
        void set_mgo(double m){correct_bd_mgo = m;}
        void set_coef(double c){coef_bd = c;}
        void set_type(std::string s){bdtype = s;}

    };

    LineType BondLine::readline(std::ifstream& file)
    {
        std::string line;
        std::getline(file, line);
        if(line.empty()) return EMPTY;
        if(line[0] == '\x2a') return COMMENT;

        std::string head;
        std::istringstream ls(line);

        ls >> head;
        if(head != "bond")
        {
            if(head == ">>>>") return BLOCK_END;
            std::cout << "this line is not bond line." << std::endl;
            std::cout << line << std::endl;
            return WRONG;
        }

        read_stream(ls);
        return NINFO_LINE;
    }

    void BondLine::write_stream(std::ostream& os)
    {
        os << "bond";
        os << std::setw(7) << this->ibd;
        os << std::setw(7) << this->iunit1 << std::setw(7) << this->iunit2;
        os << std::setw(7) << this->imp1   << std::setw(7) << this->imp2;
        os << std::setw(7) << this->imp1un << std::setw(7) << this->imp2un;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->bd_nat;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->factor_bd;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->correct_bd_mgo;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->coef_bd;
        os << " " << this->bdtype;
        os << std::endl;
        return;
    }

    void BondLine::read_stream(std::istream& is)
    {
        is >> this->ibd >> this->iunit1 >> this->iunit2;
        is >> this->imp1 >> this->imp2 >> this->imp1un >> this->imp2un;
        is >> this->bd_nat >> this->factor_bd >> this->correct_bd_mgo >> this->coef_bd;
        is >> this->bdtype;
        return;
    }


    class BondBlock : public BlockBase
    {
    public:
        BondBlock(): BlockBase(N_BOND) {}
        ~BondBlock(){}
        void write_block(std::ofstream& os);
        void read_block(std::ifstream& is);
    };

    void BondBlock::write_block(std::ofstream& os)
    {
        if(lines.empty())
        {
            std::cout << "Warning: trying to write ninfo block having no line" << std::endl;
            return;
        }

        os << std::endl;
        os << "<<<< native bond length" << std::endl;
        os << "** coef_bd(kcal/mol) = ";
        os << "factor_bd * correct_bd_mgo * cbd * energy_unit_protein" << std::endl;
        os << "**      ibd iunit1-iunit2   imp1 - imp2 imp1un-imp2un";
        os << "bd_nat    factor_bd  correct_mgo      coef_bd" << std::endl;

        for(std::vector<LineSptr>::iterator iter = lines.begin(); iter != lines.end(); ++iter)
        {
            (*iter)->write_stream(os);
        }

        os << ">>>>" << std::endl;
        os << std::endl;
        return;
    }

    void BondBlock::read_block(std::ifstream& is)
    {
        // it is expected that
        // this reads a file after checked that
        // the first line "<<<<" has valid block type.
        //
        // this may be completed by NinfoFileReader class.

        while(!is.eof())
        {
            LineSptr line(new BondLine);

            switch( line->readline(is) )
            {
            case NINFO_LINE:
                lines.push_back(line);
                break;
            case BLOCK_END:
                return;
            case COMMENT:
                break;
            case EMPTY:
                break;
            case WRONG:
                throw std::invalid_argument("File Reading Error. bond block in ninfo file has invalid line.");
            case BLOCK_START:
                throw std::invalid_argument("File Reading Error. bond block in ninfo file is not closed by <<<<.");
            default:
                throw std::invalid_argument("File Reading Error. Unknown LineType.");
            }
        }
        throw std::invalid_argument("File Reading Error. bond block in ninfo file did not end correctly.");
    }
}
#endif //COFFEE_MILL_NINFO_BOND
