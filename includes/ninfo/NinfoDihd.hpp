#ifndef COFFEE_MILL_NINFO_DIHD
#define COFFEE_MILL_NINFO_DIHD
#include "NinfoBase.hpp"

namespace coffeemill
{

    class DihdLine : public LineBase
    {
        int idih, iunit1, iunit2;
        int imp1, imp2, imp3, imp4;
        int imp1un, imp2un, imp3un, imp4un;
        double dih_nat, factor_dih, correct_dih_mgo, coef_dih_1, coef_dih_3;
        std::string dihtype;
    public:
        DihdLine(): LineBase(),
            idih(0), iunit1(0), iunit2(0), imp1(0), imp2(0), imp3(0), imp4(0),
            imp1un(0), imp2un(0), imp3un(0), imp4un(0), dih_nat(0e0), factor_dih(0e0),
            correct_dih_mgo(0e0), coef_dih_1(0e0), coef_dih_3(0e0), dihtype(){}
        ~DihdLine(){}

        LineType readline(std::ifstream& file);
        void read_stream(std::istream& is);
        void write_stream(std::ostream& os);
        int get_index() const {return this->idih;}
        int get_iunit1() const {return this->iunit1;}
        int get_iunit2() const {return this->iunit2;}
        std::vector<int> get_imps() const {return std::vector<int>({imp1, imp2, imp3, imp4});}
        std::vector<int> get_impuns() const {return std::vector<int>({imp1un, imp2un, imp3un, imp4un});}
        double get_nat() const {return dih_nat;}
        double get_factor() const {return factor_dih;}
        double get_mgo() const {return correct_dih_mgo;}
        double get_coef() const {return coef_dih_1;}
        double get_coef3() const {return coef_dih_3;}
        std::string get_type() const {return dihtype;}
        std::string get_ClassName() const {return "DihdLine";}
        BlockType get_BlockType() const {return N_DIHD;};

        void set_index(int i){idih = i;}
        void set_iunit1(int i){iunit1 = i;}
        void set_iunit2(int i){iunit2 = i;}
        void set_imps(std::vector<int> imps)
        {
            if(imps.size() < 4)
                throw std::invalid_argument("there are no enough argument");
            if(imps.size() > 4) 
                std::cout << "Warning: set_imps has more than 4 argument in BondLine"
                          << std::endl;
            imp1 = imps.at(1);
            imp2 = imps.at(2);
            imp3 = imps.at(3);
            imp4 = imps.at(4);
            return;
        }
        void set_impuns(std::vector<int> impuns)
        {
            if(impuns.size() < 4)
                throw std::invalid_argument("there are no enough argument");

            if(impuns.size() != 4) 
                std::cout << "Warning: set_impuns has more than 4 argument in BondLine"
                          << std::endl;
            imp1un = impuns.at(1);
            imp2un = impuns.at(2);
            imp3un = impuns.at(3);
            imp4un = impuns.at(4);
            return;
        }
        void set_nat(double n){ dih_nat = n;}
        void set_factor(double f){factor_dih = f;}
        void set_mgo(double m){correct_dih_mgo = m;}
        void set_coef(double c){coef_dih_1 = c;}
        void set_coef3(double c3){coef_dih_3 = c3;}
        void set_type(std::string s){dihtype = s;}

    };

    LineType DihdLine::readline(std::ifstream& file)
    {
        std::string line;
        std::getline(file, line);
        if(line.empty()) return EMPTY;
        if(line[0] == '\x2a') return COMMENT;
        
        std::string head;
        std::istringstream ls(line);

        ls >> head;
        if(head != "dihd")
        {
            if(head == ">>>>") return BLOCK_END;
            std::cout << "this line is not dihd line." << std::endl;
            std::cout << line << std::endl;
            return WRONG;
        }

        read_stream(ls);
        return NINFO_LINE;
    }

    void DihdLine::write_stream(std::ostream& os)
    {
        os << "dihd";
        os << std::setw(7) << this->idih;
        os << std::setw(7) << this->iunit1 << std::setw(7) << this->iunit2;
        os << std::setw(7) << this->imp1   << std::setw(7) << this->imp2;
        os << std::setw(7) << this->imp3   << std::setw(7) << this->imp4;
        os << std::setw(7) << this->imp1un << std::setw(7) << this->imp2un;
        os << std::setw(7) << this->imp3un << std::setw(7) << this->imp4un;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->dih_nat;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->factor_dih;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->correct_dih_mgo;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->coef_dih_1;
        os << std::setw(13) << std::fixed << std::setprecision(4) << this->coef_dih_3;
        os << " " << this->dihtype;
        os << std::endl;
        return;
    }

    void DihdLine::read_stream(std::istream& is)
    {
        is >> this->idih >> this->iunit1 >> this->iunit2;
        is >> this->imp1 >> this->imp2 >> this->imp3 >> this->imp4;
        is >> this->imp1un >> this->imp2un >> this->imp3un >> this->imp4un;
        is >> this->dih_nat >> this->factor_dih >> this->correct_dih_mgo;
        is >> this->coef_dih_1 >> this->coef_dih_3;
        is >> this->dihtype;
        return;
    }
    

    class DihdBlock : public BlockBase
    {
    public:
        DihdBlock(): BlockBase(N_DIHD) {}
        ~DihdBlock(){}

        void write_block(std::ofstream& os);
        void read_block(std::ifstream& is);
    };

    void DihdBlock::write_block(std::ofstream& os)
    {
        if(lines.empty())
        {
            std::cout << "Warning: trying to write ninfo block having no line" << std::endl;
            return;
        }
        
        os << std::endl;
        os << "<<<< native dihedral angles" << std::endl;
        os << "** coef_dih1(kcal/mol) = ";
        os << "factor_dih * correct_dih_mgo * cdih1 * energy_unit_protein" << std::endl;
        os << "** coef_dih3(kcal/mol) = ";
        os << "factor_dih * correct_dih_mgo * cdih3 * energy_unit_protein" << std::endl;
        os << "**      idih iunit1-iunit2   imp1 - imp2 - imp3 - imp4";
        os << " imp1un-imp2un-imp3un-imp4un      ";
        os << "dih_nat    factor_dih  correct_mgo      coef_dih1    coef_dih3" << std::endl;

        for(std::vector<LineSptr>::iterator iter = lines.begin(); iter != lines.end(); ++iter)
        {
            (*iter)->write_stream(os);
        }

        os << ">>>>" << std::endl;
        os << std::endl;
        return;
    }

    void DihdBlock::read_block(std::ifstream& is)
    {
        // it is expected that
        // this reads a file after checked that
        // the first line "<<<<" has valid block type.
        //
        // this may be completed by NinfoFileReader class.

        while(!is.eof())
        {
            LineSptr line(new DihdLine);

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
                throw std::invalid_argument("File Reading Error. dihd block in ninfo file has invalid line.");
            case BLOCK_START:
                throw std::invalid_argument("File Reading Error. dihd block in ninfo file is not closed by <<<<.");
            default:
                throw std::invalid_argument("File Reading Error. Unknown LineType.");
            }
        }
        throw std::invalid_argument("File Reading Error. dihd block in ninfo file did not end correctly.");
    }
}
#endif //COFFEE_MILL_NINFO_DIHD
