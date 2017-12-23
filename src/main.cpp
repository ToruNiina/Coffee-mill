#include <mill/math/Vector.hpp>
#include <iostream>
#include <src/mode_help.hpp>
#include <src/mode_dcd.hpp>
#include <src/mode_pdb.hpp>
#include <src/mode_ninfo.hpp>

template<std::size_t MAJOR_V, std::size_t MINOR_V>
void print_logo();

int main(int argc, char **argv)
{
    print_logo<2, 0>();

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

template<std::size_t MAJOR_V, std::size_t MINOR_V>
void print_logo()
{
    std::cerr << "  ___        __  __             __    __ _  _  _  \n";
    std::cerr << " / __| ___  / _|/ _| ___  ___   | \\  / |(_)| || | \n";
    std::cerr << "| |   / _ \\| |_| |_ / _ \\/ _ \\  |  \\/  || || || | \n";
    std::cerr << "| |__| (_) |  _|  _|  __|  __/  | |\\/| || || || | \n";
    std::cerr << " \\___|\\___/|_| |_|  \\___|\\___|  |_|  |_||_||_||_| \n";
    std::cerr << "                         Copyright 2016- Toru Niina \n";
    std::cerr << std::endl;
    return;
}
