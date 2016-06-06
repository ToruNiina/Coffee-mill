#ifndef COFFEE_MILL_COMMAND_LINE_ARGUMENT_PARSER
#define COFFEE_MILL_COMMAND_LINE_ARGUMENT_PARSER
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

namespace coffeemill
{

class CommandLine
{
  public:
    enum class MODE;
    enum class JOB;

  public:

    CommandLine(int argc, char **argv) : argc_(argc)
    {
        for(int i = 0; i < argc_; ++i)
            argv_.push_back(argv[i]);
    }
    ~CommandLine() = default;

    bool parse();

    template<std::size_t MAJOR_V, std::size_t MINOR_V>
    void print_logo() const;
    void print_help() const;
    void print_man() const;

    MODE mode() const;
    JOB  job() const;
    std::string file() const;

  public:

    enum class MODE
    {
        PDB,
        DCD,
        NINFO,
        DNA,
        HELP,
        MANUAL,
    };

    enum class JOB
    {
        SEQ,
        SPLIT,
        JOIN,
        COMPLEMENTAL,
        MUTATE,
        SHOW,
        MAKE_CG,
        MAKE_NINFO,
        MAKE_MOVIE,
    };

  public:

    const std::string HELP_MODE_LONG  = "--help";
    const std::string HELP_MODE_SHORT = "-h";
    const std::string MANUAL_MODE     = "--man";

    const std::string PDB_MODE = "pdb";
    const std::string DNA_MODE   = "dna";
    const std::string DCD_MODE   = "dcd";
    const std::string NINFO_MODE = "ninfo";

    const std::string JOB_SEQ   = "--seq";
    const std::string JOB_SPLIT = "--split";
    const std::string JOB_JOIN  = "--join";
    const std::string JOB_SHOW  = "--show";
    const std::string JOB_COMPLEMENTAL = "--complemental";
    const std::string JOB_MUTATE       = "--mutate";
    const std::string JOB_MAKE_CG    = "--make-cg";
    const std::string JOB_MAKE_NINFO = "--make-ninfo";
    const std::string JOB_MAKE_MOVIE = "--make-movie";

  private:
    int  argc_;
    std::vector<std::string> argv_;

    MODE mode_;
    JOB  job_;
};

template<std::size_t MAJOR_V, std::size_t MINOR_V>
void CommandLine::print_logo() const
{
// not escaped
//     std::cerr << "  ____        __  __             __    __ _  _  _  " << std::endl;
//     std::cerr << " / ___| ___  / _|/ _| ___  ___   | \  / |(_)| || | " << std::endl;
//     std::cerr << "| |    / _ \| |_| |_ / _ \/ _ \  |  \/  || || || | " << std::endl;
//     std::cerr << "| |___| (_) |  _|  _|  __|  __/  | |\/| || || || | " << std::endl;
//     std::cerr << " \____|\___/|_| |_|  \___|\___|  |_|  |_||_||_||_| " << std::endl;
    std::cerr << "  ___        __  __             __    __ _  _  _  "     << std::endl;
    std::cerr << " / __| ___  / _|/ _| ___  ___   | \\  / |(_)| || | "    << std::endl;
    std::cerr << "| |   / _ \\| |_| |_ / _ \\/ _ \\  |  \\/  || || || | " << std::endl;
    std::cerr << "| |__| (_) |  _|  _|  __|  __/  | |\\/| || || || | "    << std::endl;
    std::cerr << " \\___|\\___/|_| |_|  \\___|\\___|  |_|  |_||_||_||_| " << std::endl;
    std::cerr << "                                      version "
              << MAJOR_V << "." << MINOR_V << std::endl;
    std::cerr << "                         Copyright 2016 Toru Niina " << std::endl;
    std::cerr << std::endl;
    std::cerr << std::endl;
 
    return;
}

void CommandLine::print_help() const
{
    std::cerr << "Coffee-Mill" << std::endl;
    std::cerr << "Usage: mill [mode] [job]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "command-line interface" << std::endl;
    std::cerr << std::endl;
    std::cerr << "list of modes" << std::endl;
    std::cerr << "\t" << PDB_MODE   << std::endl;
    std::cerr << "\t" << DNA_MODE   << std::endl;
    std::cerr << "\t" << DCD_MODE   << std::endl;
    std::cerr << "\t" << NINFO_MODE << std::endl;
    std::cerr << std::endl;
    std::cerr << "jobs"    << std::endl;
    std::cerr << "\t" << JOB_SEQ          << std::endl;
    std::cerr << "\t" << JOB_JOIN         << std::endl;
    std::cerr << "\t" << JOB_SHOW         << std::endl;
    std::cerr << "\t" << JOB_COMPLEMENTAL << std::endl;
    std::cerr << "\t" << JOB_MUTATE       << std::endl;
    std::cerr << "\t" << JOB_MAKE_CG      << std::endl;
    std::cerr << "\t" << JOB_MAKE_NINFO   << std::endl;
    std::cerr << "\t" << JOB_MAKE_MOVIE   << std::endl;
    std::cerr << std::endl;
    std::cerr << "some combination of mode and job is not defined." << std::endl;
    std::cerr << "to see more information, run `mill --man`."       << std::endl;
    return;
}

void CommandLine::print_man() const
{
    std::cerr << "\t\t\t\t Coffee Mill" << std::endl;

    std::cerr << "NAME" << std::endl;
    std::cerr << "\tcoffee mill - command line tools for using cafemol" << std::endl;

    std::cerr << std::endl;
    std::cerr << "SYNOPSIS" << std::endl;
    std::cerr << "\tmill [-h] {pdb | dcd | ninfo | dna}" << std::endl;
    std::cerr << std::endl;

    std::cerr << "DESCRIPTION" << std::endl;
    std::cerr << "\tcoffee mill is the command line tool for handling cafemol files."
              << std::endl;
    std::cerr << std::endl;

    std::cerr << "\tpdb" << std::endl;
    std::cerr << "\t\thandle pdb files. extract sequence, split for cafemol input, "
              << std::endl;
    std::cerr << "\t\tmake cg file, make ninfo file, etc..." << std::endl;
    std::cerr << std::endl;

    std::cerr << "\tdna" << std::endl;
    std::cerr << "\t\thandle dna sequence, mutate pdb file, etc..." << std::endl;
    std::cerr << std::endl;
 
    std::cerr << "\tdcd" << std::endl;
    std::cerr << "\t\thandle dcd files. make movie file, calculate some values, "
              << std::endl;
    std::cerr << "\t\tjoin some dcd files, etc..." << std::endl;
    std::cerr << std::endl;

    std::cerr << "\tninfo" << std::endl;
    std::cerr << "\t\thandle ninfo files. split ninfo files, show structure as XYZ file,"
              << std::endl;
    std::cerr << "\t\tetc..." << std::endl;

    return;
}

bool CommandLine::parse()
{
    if(argc_ < 2) return false;

    if(argv_[1] == HELP_MODE_SHORT || argv_[1] == HELP_MODE_LONG)
    {
        mode_ = MODE::HELP;
        return true;
    }
    else if(argv_[1] == MANUAL_MODE)
    {
        mode_ = MODE::MANUAL;
        return true;
    }

    if(argc_ < 3) return false;

    if(argv_[1] == PDB_MODE)
        mode_ = MODE::PDB;
    else if(argv_[1] == DNA_MODE)
        mode_ = MODE::DNA;
    else if(argv_[1] == NINFO_MODE)
        mode_ = MODE::NINFO;
    else if(argv_[1] == DCD_MODE)
        mode_ = MODE::DCD;
    else
        return false;

    if(argv_[2] == JOB_SEQ)
        job_ = JOB::SEQ;
    else if(argv_[2] == JOB_SPLIT)
        job_ = JOB::SPLIT;
    else if(argv_[2] == JOB_JOIN)
        job_ = JOB::JOIN;
    else if(argv_[2] == JOB_COMPLEMENTAL)
        job_ = JOB::COMPLEMENTAL;
    else if(argv_[2] == JOB_MUTATE)
        job_ = JOB::MUTATE;
    else if(argv_[2] == JOB_SHOW)
        job_ = JOB::SHOW;
    else if(argv_[2] == JOB_MAKE_CG)
        job_ = JOB::MAKE_CG;
    else if(argv_[2] == JOB_MAKE_NINFO)
        job_ = JOB::MAKE_NINFO;
    else if(argv_[2] == JOB_MAKE_MOVIE)
        job_ = JOB::MAKE_MOVIE;
    else
        return false;   

    return true;
}

CommandLine::MODE CommandLine::mode() const
{
    return mode_;
}

CommandLine::JOB CommandLine::job() const
{
    return job_;
}

std::string CommandLine::file() const
{
    try{return argv_.at(3);}
    catch(std::out_of_range& except)
    {throw std::runtime_error("File not specified");}
}

}

#endif /* COFFEE_MILL_COMMAND_LINE_ARGUMENT_PARSER */
