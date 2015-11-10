#ifndef COFFEE_MILL_NINFO_ANGL
#define COFFEE_MILL_NINFO_ANGL
#include "NinfoBase.hpp"

namespace coffeemill
{

    class AnglLine : public LineBase
    {
        int iba, iunit1, iunit2;
        int imp1, imp2, imp3, imp1un, imp2un, imp3un;
        double ba_nat, factor_ba, correct_ba_mgo, coef_ba;
        std::string batype;
    public:
        AnglLine(): LineBase(),
            iba(0), iunit1(0), iunit2(0), imp1(0), imp2(0), imp3(0),
            imp1un(0), imp2un(0), imp3un(0), ba_nat(0e0), factor_ba(0e0),
            correct_ba_mgo(0e0), coef_ba(0e0), batype(){}
        ~AnglLine(){}

        LineType readline(std::ifstream& file);
        void read_stream(std::istream& is);
        void write_stream(std::ostream& os);
        int get_index() const {return iba;}
        int get_iunit1() const {return iunit1;}
        int get_iunit2() const {return iunit2;}
        std::vector<int> get_imps() const {return std::vector<int>({imp1, imp2, imp3});}
        std::vector<int> get_impuns() const {return std::vector<int>({imp1un, imp2un, imp3un});}
        double get_nat() const {return ba_nat;}
        double get_factor() const {return factor_ba;}
        double get_mgo() const {return correct_ba_mgo;}
        double get_coef() const {return coef_ba;}
        std::string get_type() const {return batype;}
        std::string get_ClassName() const {return "AnglLine";}
        BlockType get_BlockType() const {return N_ANGL;}

        void set_index(int i){iba = i;}
        void set_iunit1(int i){iunit1 = i;}
        void set_iunit2(int i){iunit2 = i;}
        void set_imps(std::vector<int> imps)
        {
            if(imps.size() < 3)
                throw std::invalid_argument("there are no enough argument");
            if(imps.size() > 3) 
                std::cout << "Warning: set_imps has more than 2 argument in BondLine"
                          << std::endl;
            imp1 = imps.at(1);
            imp2 = imps.at(2);
            imp3 = imps.at(3);
            return;
        }
        void set_impuns(std::vector<int> impuns)
        {
            if(impuns.size() < 3)
                throw std::invalid_argument("there are no enough argument");

            if(impuns.size() != 3) 
                std::cout << "Warning: set_impuns has more than 2 argument in BondLine"
                          << std::endl;
            imp1un = impuns.at(1);
            imp2un = impuns.at(2);
            imp3un = impuns.at(3);
            return;
        }
        void set_nat(double n){ ba_nat = n;}
        void set_factor(double f){factor_ba = f;}
        void set_mgo(double m){correct_ba_mgo = m;}
        void set_coef(double c){coef_ba = c;}
        void set_type(std::string s){batype = s;}

    };

    LineType AnglLine::readline(std::ifstream& file)
    {
        std::string line;
        std::getline(file, line);
        if(line.empty()) return EMPTY;
        if(line[0] == '\x2a') return COMMENT;

        std::string head;
        std::istringstream ls(line);

        ls >> head;
        if(head != "angl")
        {
            if(head == ">>>>") return BLOCK_END;
            std::cout << "this line is not angl line." << std::endl;
            std::cout << line << std::endl;
            return WRONG;
        }

        read_stream(ls);

        return NINFO_LINE;
    }

    void AnglLine::write_stream(std::ostream& os)
    {
        os << "angl";
        os << std::setw(7) << this->iba;
        os << std::setw(7) << this->iunit1 << std::setw(7) << this->iunit2;
        os << std::setw(7) << this->imp1 << std::setw(7) << this->imp2;
        os << std::setw(7) << this->imp3;
        os << std::setw(7) << this->imp1un << std::setw(7) << this->imp2un;
        os << std::setw(7) << this->imp3un;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->ba_nat;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->factor_ba;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->correct_ba_mgo;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->coef_ba;
        os << " " << this->batype;
        os << std::endl;
        return;
    }

    void AnglLine::read_stream(std::istream& is)
    {
        is >> this->iba >> this->iunit1 >> this->iunit2;
        is >> this->imp1 >> this->imp2 >> this->imp3;
        is >> this->imp1un >> this->imp2un >> this->imp3un;
        is >> this->ba_nat >> this->factor_ba >> this->correct_ba_mgo >> this->coef_ba;
        is >> this->batype;
        return;
    }


    class AnglBlock : public BlockBase
    {
    public:
        AnglBlock(): BlockBase(N_ANGL) {}
        ~AnglBlock(){}
        void write_block(std::ofstream& os);
        void read_block(std::ifstream& is);
    };

    void AnglBlock::write_block(std::ofstream& os)
    {
        if(lines.empty())
        {
            std::cout << "Warning: trying to write ninfo block having no line" << std::endl;
            return;
        }

        os << std::endl;
        os << "<<<< native bond angles" << std::endl;
        os << "** coef_ba(kcal/mol) = ";
        os << "factor_ba * correct_ba_mgo * cba * energy_unit_protein" << std::endl;
        os << "**      iba iunit1-iunit2   imp1 - imp2 - imp3 imp1un-imp2un-imp3un      ba_nat    factor_ba  correct_mgo      coef_ba" << std::endl;

        for(std::vector<LineSptr>::iterator iter = lines.begin(); iter != lines.end(); ++iter)
        {
            (*iter)->write_stream(os);
        }

        os << ">>>>" << std::endl;
        os << std::endl;
        return;
    }

    void AnglBlock::read_block(std::ifstream& is)
    {
        // it is expected that
        // this reads a file after checked that
        // the first line "<<<<" has valid block type.
        //
        // this may be completed by NinfoFileReader class.

        while(!is.eof())
        {
            LineSptr line(new AnglLine);

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
                throw std::invalid_argument("File Reading Error. angl block in ninfo file has invalid line.");
            case BLOCK_START:
                throw std::invalid_argument("File Reading Error. angl block in ninfo file is not closed by <<<<.");
            default:
                throw std::invalid_argument("File Reading Error. Unknown LineType.");
            }
        }
        throw std::invalid_argument("File Reading Error. angl block in ninfo file did not end correctly.");
    }



}
#endif //COFFEE_MILL_NINFO_ANGL
