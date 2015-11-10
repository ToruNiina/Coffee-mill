#ifndef COFFEE_MILL_NINFO_BASE
#define COFFEE_MILL_NINFO_BASE
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <memory>
#include "NinfoDef.hpp"

namespace coffeemill
{

    class LineBase
    {

    public:
        LineBase(){}
        virtual ~LineBase(){}

        virtual LineType readline(std::ifstream& file) = 0;
        virtual void read_stream(std::istream& is) = 0;
        virtual void write_stream(std::ostream& os) = 0;

        virtual int get_index() const = 0;
        virtual int get_iunit1() const = 0;
        virtual int get_iunit2() const = 0;
        virtual std::vector<int> get_imps() const = 0;
        virtual std::vector<int> get_impuns() const = 0;
        virtual double get_nat() const = 0;
        virtual double get_factor() const = 0;
        // correct or dummy
        virtual double get_mgo() const = 0;
        virtual double get_coef() const = 0;
        // wid in aicg or coef3 in dihd
        virtual double get_wid() const;
        virtual double get_coef3() const;
        virtual int get_nhb_bp() const;
        virtual std::string get_type() const = 0;
        virtual BlockType get_BlockType() const;
        virtual std::string get_ClassName() const;

        virtual void set_index(int i) = 0;
        virtual void set_iunit1(int i) = 0;
        virtual void set_iunit2(int i) = 0;
        virtual void set_imps(std::vector<int> imps) = 0;
        virtual void set_impuns(std::vector<int> impuns) = 0;
        virtual void set_nat(double n) = 0;
        virtual void set_factor(double f) = 0;
        virtual void set_mgo(double m) = 0;// correct or dummy
        virtual void set_coef(double c) = 0;
        virtual void set_coef3(double c3);
        virtual void set_wid(double w);
        virtual void set_nhb_bp(int n);
        virtual void set_type(std::string s) = 0;

    };

    double LineBase::get_wid() const
    {
        std::cout << "Warning: this does not have wid_gauss. return 0." << std::endl;
        std::cout << "Class: " << get_ClassName() << std::endl;
        return 0e0;
    }

    double LineBase::get_coef3() const
    {
        std::cout << "Warning: this does not have coef_3. return 0." << std::endl;
        std::cout << "Class: " << get_ClassName() << std::endl;
        return 0e0;
    }

    int LineBase::get_nhb_bp() const
    {
        std::cout << "Warning: this does not have nhb_bp. return 0." << std::endl;
        std::cout << "Class: " << get_ClassName() << std::endl;
        return 0;
    }

    void LineBase::set_wid(double w)
    {
        std::cout << "Warning: this does not have wid_gauss. do nothing." << std::endl;
        std::cout << "Class: " << get_ClassName() << std::endl;
        return;
    }

    void LineBase::set_coef3(double c3)
    {
        std::cout << "Warning: this does not have coef_3. do nothing." << std::endl;
        std::cout << "Class: " << get_ClassName() << std::endl;
        return;
    }

    void LineBase::set_nhb_bp(int n)
    {
        std::cout << "Warning: this does not have nhb_bp. do nothing." << std::endl;
        std::cout << "Class: " << get_ClassName() << std::endl;
        return;
    }

    BlockType LineBase::get_BlockType() const
    {
        return N_BASE;
    }

    std::string LineBase::get_ClassName() const
    {
        return "LineBase";
    }

    typedef std::shared_ptr<LineBase> LineSptr;
    typedef std::weak_ptr<LineBase> LineWptr;


    class BlockBase
    {
    protected:
        BlockType type;

    public:
        std::vector<LineSptr> lines;

    public:
        BlockBase(BlockType T): type(T){}
        virtual ~BlockBase(){}

        void push_back(LineSptr line);
        BlockType get_BlockType() const { return type; }
        int size() const { return lines.size(); }
        std::vector<LineSptr>::iterator begin(){ return lines.begin(); }
        std::vector<LineSptr>::iterator end(){ return lines.end(); }
        void set_iunits(iUnits iunit);

        virtual void write_block(std::ofstream& os) = 0;
        virtual void read_block(std::ifstream& is) = 0;

    };

    void BlockBase::push_back(LineSptr line)
    {
        if( line->get_BlockType() != type )
            throw std::invalid_argument("Block must not contain other type line");

        lines.push_back(line);
        return;
    }

    void BlockBase::set_iunits(iUnits iunit)
    {
        for(auto iter = lines.begin(); iter != lines.end(); ++iter)
        {
            (*iter)->set_iunit1(iunit.first);
            (*iter)->set_iunit2(iunit.second);
        }
        return;
    }


    typedef std::shared_ptr<BlockBase> BlockSptr;
    typedef std::weak_ptr<BlockBase> BlockWptr;
    typedef std::vector<BlockSptr> NinfoData;

}
#endif //COFFEE_MILL_NINFO_BASE
