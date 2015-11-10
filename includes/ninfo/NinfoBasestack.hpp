#ifndef COFFEE_MILL_NINFO_BASESTACK
#define COFFEE_MILL_NINFO_BASESTACK
#include "NinfoBase.hpp"

namespace coffeemill
{

    class BasestackLine : public LineBase
    {
        int icon, iunit1, iunit2;
        int imp1, imp2, imp1un, imp2un;
        double go_nat, factor_go, dummy_mgo, coef_go;
        std::string bstype;
    public:
        BasestackLine() : LineBase(),
            icon(0), iunit1(0), iunit2(0), imp1(0), imp2(0), 
            imp1un(0), imp2un(0), go_nat(0e0), factor_go(0e0),
            dummy_mgo(0e0), coef_go(0e0), bstype(){}
        ~BasestackLine(){}

        LineType readline(std::ifstream& file);
        void read_stream(std::istream& is);
        void write_stream(std::ostream& os);

        int get_index() const {return icon;}
        int get_iunit1() const {return iunit1;}
        int get_iunit2() const {return iunit2;}
        std::vector<int> get_imps() const {return std::vector<int>({imp1, imp2});}
        std::vector<int> get_impuns() const {return std::vector<int>({imp1un, imp2un});}
        double get_nat() const {return go_nat;}
        double get_factor() const {return factor_go;}
        double get_mgo() const {return dummy_mgo;}
        double get_coef() const {return coef_go;}
        std::string get_type() const {return bstype;}
        std::string get_ClassName() const {return "BasepaitLine";}
        BlockType get_BlockType() const {return N_BASESTACK;}

        void set_index(int i){icon = i;}
        void set_iunit1(int i){iunit1 = i;}
        void set_iunit2(int i){iunit2 = i;}
        void set_imps(std::vector<int> imps)
        {
            if(imps.size() < 2)
                throw std::invalid_argument("there are no enough argument");
            if(imps.size() > 2) 
                std::cout << "Warning: set_imps has more than 2 argument in BasestackLine"
                          << std::endl;
            imp1 = imps.at(1);
            imp2 = imps.at(2);
            return;
        }
        void set_impuns(std::vector<int> impuns)
        {
            if(impuns.size() < 2)
                throw std::invalid_argument("there are no enough argument");

            if(impuns.size() > 2) 
                std::cout << "Warning: set_impuns has more than 2 argument in BasestackLine"
                          << std::endl;
            imp1un = impuns.at(1);
            imp2un = impuns.at(2);
            return;
        }
        void set_nat(double n){ go_nat = n;}
        void set_factor(double f){factor_go = f;}
        void set_mgo(double m){dummy_mgo = m;}
        void set_coef(double c){coef_go = c;}
        void set_type(std::string s){bstype = s;}

    };

    LineType BasestackLine::readline(std::ifstream& file)
    {
        std::string line;
        std::getline(file, line);
        if(line.empty()) return EMPTY;
        if(line[0] == '\x2a') return COMMENT;

        std::string head;
        std::istringstream ls(line);

        ls >> head;
        if(head != "basestack")
        {
            if(head == ">>>>") return BLOCK_END;
            std::cout << "this line is not Basestack line." << std::endl;
            std::cout << line << std::endl;
            return WRONG;
        }

        read_stream(ls);
        return NINFO_LINE;
    }

    void BasestackLine::write_stream(std::ostream& os)
    {
        os << "basestack";
        os << std::setw(7) << this->icon;
        os << std::setw(7) << this->iunit1 << std::setw(7) << this->iunit2;
        os << std::setw(7) << this->imp1 << std::setw(7) << this->imp2;
        os << std::setw(7) << this->imp1un << std::setw(7) << this->imp2un;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->go_nat;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->factor_go;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->dummy_mgo;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->coef_go;
        os << " " << this->bstype;
        os << std::endl;
        return;
    }

    void BasestackLine::read_stream(std::istream& is)
    {
        is >> this->icon >> this->iunit1 >> this->iunit2;
        is >> this->imp1 >> this->imp2 >> this->imp1un >> this->imp2un;
        is >> this->go_nat >> this->factor_go >> this->dummy_mgo >> this->coef_go;
        is >> this->bstype;
        return;
    }


    class BasestackBlock : public BlockBase
    {
    public:
        BasestackBlock(): BlockBase(N_BASESTACK) {}
        ~BasestackBlock(){}
        void write_block(std::ofstream& os);
        void read_block(std::ifstream& is);
    };

    void BasestackBlock::write_block(std::ofstream& os)
    {
        if(lines.empty())
        {
            std::cout << "Warning: trying to write ninfo block having no line" << std::endl;
            return;
        }

        os << std::endl;
        os << "<<<< native Basestack" << std::endl;
        //TODO: total contact, definition, unit name, etc...

        for(std::vector<LineSptr>::iterator iter = lines.begin(); iter != lines.end(); ++iter)
        {
            (*iter)->write_stream(os);
        }

        os << ">>>>" << std::endl;
        os << std::endl;
        return;
    }

    void BasestackBlock::read_block(std::ifstream& is)
    {
        // it is expected that
        // this reads a file after checked that
        // the first line "<<<<" has valid block type.
        //
        // this may be completed by NinfoFileReader class.

        while(!is.eof())
        {
            LineSptr line(new BasestackLine);

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
                throw std::invalid_argument("File Reading Error. Basestack block in ninfo file has invalid line.");
            case BLOCK_START:
                throw std::invalid_argument("File Reading Error. Basestack block in ninfo file is not closed by <<<<.");
            default:
                throw std::invalid_argument("File Reading Error. Unknown LineType.");
            }
        }
        throw std::invalid_argument("File Reading Error. Basestack block in ninfo file did not end correctly.");
    }
}
#endif //COFFEE_MILL_NINFO_BASESTACK
