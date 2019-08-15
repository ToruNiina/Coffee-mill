#include <mill/math/Vector.hpp>
#include <iostream>
#include <src/mode_help.hpp>
#include <src/mode_dcd.hpp>
#include <src/mode_pdb.hpp>
#include <src/mode_ninfo.hpp>
#include <src/mode_calc.hpp>

void print_logo();

int main(int argc, char **argv)
{
    print_logo();

    if(argc < 2)
    {
        std::cerr << mill::main_usage() << std::endl;
        return 1;
    }
    const std::string mode(argv[1]);

    if(mode == "dcd")
    {
        try
        {
            return mill::mode_dcd<mill::Vector<double, 3>>(
                    argc-1, const_cast<const char**>(argv+1));
        }
        catch(std::exception& excpt)
        {
            std::cerr << "error: " << excpt.what() << std::endl;
            std::cerr << mill::main_usage() << std::endl;
            return 1;
        }
    }
    else if(mode == "pdb")
    {
        try
        {
            return mill::mode_pdb<mill::Vector<double, 3>>(
                    argc-1, const_cast<const char**>(argv+1));
        }
        catch(std::exception& excpt)
        {
            std::cerr << "error: " << excpt.what() << std::endl;
            std::cerr << mill::main_usage() << std::endl;
            return 1;
        }
    }
    else if(mode == "ninfo")
    {
        try
        {
            return mill::mode_ninfo<mill::Vector<double, 3>>(
                    argc-1, const_cast<const char**>(argv+1));
        }
        catch(std::exception& excpt)
        {
            std::cerr << "error: " << excpt.what() << std::endl;
            return 1;
        }
    }
    else if(mode == "calc")
    {
        try
        {
            return mill::mode_calc<mill::Vector<double, 3>>(
                    argc-1, const_cast<const char**>(argv+1));
        }
        catch(std::exception& excpt)
        {
            std::cerr << "error: " << excpt.what() << std::endl;
            return 1;
        }
    }
    else if(mode == "help")
    {
        return mill::mode_help(
                    argc-1, const_cast<const char**>(argv+1));
    }
    else
    {
        std::cerr << "unknown mode: " << mode << '\n';
        std::cerr << mill::main_usage() << std::endl;
        return 1;
    }
}

void print_logo()
{
    std::cerr << R"(  ___        __  __             __    __ _  _  _ )""\n"
                 R"( / __| ___  / _|/ _| ___  ___   | \  / |(_)| || |)""\n"
                 R"(| |   / _ \| |_| |_ / _ \/ _ \  |  \/  || || || |)""\n"
                 R"(| |__| (_) |  _|  _|  __|  __/  | |\/| || || || |)""\n"
                 R"( \___|\___/|_| |_|  \___|\___|  |_|  |_||_||_||_|)""\n"
                 R"(                   Copyright 2016-now Toru Niina))"
              << std::endl;
    return;
}
