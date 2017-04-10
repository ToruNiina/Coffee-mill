#include <mill/math/Vector.hpp>
#include "mill_pdb.hpp"
#include "mill_dcd.hpp"
#include "mill_ninfo.hpp"
#include <iostream>

template<std::size_t MAJOR_V, std::size_t MINOR_V>
void print_logo();
void print_help();
void print_man();

int main(int argc, char **argv)
{
    print_logo<1, 0>();
    if(argc < 3)
    {
        print_help();
        return 1;
    }

    std::string mode(argv[1]);

    if(mode == "pdb")
    {
        try
        {
            return mill::mode_pdb<mill::Vector<double, 3>>(argc-1, ++argv);
        }
        catch(std::exception& excpt)
        {
            std::cerr << "exception thrown: " << excpt.what() << std::endl;
            return 1;
        }
    }
    else if(mode == "ninfo")
    {
        try
        {
            return mill::mode_ninfo<mill::Vector<double, 3>>(argc-1, ++argv);
        }
        catch(std::exception& excpt)
        {
            std::cerr << "exception thrown: " << excpt.what() << std::endl;
            return 1;
        }
    }
    else if(mode == "dcd")
    {
        try
        {
            return mill::mode_dcd<mill::Vector<double, 3>>(argc-1, ++argv);
        }
        catch(std::exception& excpt)
        {
            std::cerr << "exception thrown: " << excpt.what() << std::endl;
            return 1;
        }
    }
    else if(mode == "help")
    {
        print_help();
        return 1;
    }
    else
    {
        std::cerr << "unknown command: " << mode << std::endl;
        print_help();
        return 1;
    }
}


template<std::size_t MAJOR_V, std::size_t MINOR_V>
void print_logo()
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

void print_help()
{
    std::cerr << "Usage: mill [mode] [job]" << std::endl;
    std::cerr << std::endl;
    std::cerr << "command-line interface" << std::endl;
    std::cerr << std::endl;
    std::cerr << "list of modes" << std::endl;
    std::cerr << "\t" << "pdb" << std::endl;
    std::cerr << "\t" << "dcd" << std::endl;
    std::cerr << "\t" << "ninfo" << std::endl;
    std::cerr << std::endl;
    std::cerr << "jobs"    << std::endl;
    std::cerr << "\t" << "seq" << std::endl;
    std::cerr << "\t" << "join" << std::endl;
    std::cerr << "\t" << "split" << std::endl;
    std::cerr << "\t" << "make-movie" << std::endl;
//     std::cerr << "\t" << JOB_SHOW         << std::endl;
//     std::cerr << "\t" << JOB_MUTATE       << std::endl;
//     std::cerr << "\t" << JOB_MAKE_CG      << std::endl;
//     std::cerr << "\t" << JOB_MAKE_NINFO   << std::endl;
    std::cerr << std::endl;
    std::cerr << "some combination of mode and job is not defined." << std::endl;
    std::cerr << "to see more information, run `mill --man`."       << std::endl;
    return;
}

void print_man()
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
