#include <mill/math/Vector.hpp>
#include <iostream>
#include <src/mode_help.hpp>
#include <src/mode_dcd.hpp>
#include <src/mode_pdb.hpp>
#include <src/mode_ninfo.hpp>
#include <src/mode_calc.hpp>

void print_logo();
std::pair<int, std::vector<const char*>> setup_logger(int argc, char **argv);

int main(int argc, char **argv)
{
    using vector_type = mill::Vector<double, 3>;

    print_logo();
    auto [argc_, argv_v] = setup_logger(argc, argv);
    const char** argv_ = argv_v.data();

    if(argc_ < 2)
    {
        std::cerr << mill::main_usage() << std::endl;
        return 1;
    }

    const std::string mode(argv_[1]);
    if(mode == "dcd")
    {
        try
        {
            return mill::mode_dcd<vector_type>(argc_ - 1, argv_ + 1);
        }
        catch(std::exception& excpt)
        {
            mill::log(mill::log_level::error, excpt.what(), '\n');
            std::cerr << mill::main_usage() << std::endl;
            return 1;
        }
    }
    else if(mode == "pdb")
    {
        try
        {
            return mill::mode_pdb<vector_type>(argc_ - 1, argv_ + 1);
        }
        catch(std::exception& excpt)
        {
            mill::log(mill::log_level::error, excpt.what(), '\n');
            std::cerr << mill::main_usage() << std::endl;
            return 1;
        }
    }
    else if(mode == "ninfo")
    {
        try
        {
            return mill::mode_ninfo<vector_type>(argc_ - 1, argv_ + 1);
        }
        catch(std::exception& excpt)
        {
            mill::log(mill::log_level::error, excpt.what(), '\n');
            std::cerr << mill::main_usage() << std::endl;
            return 1;
        }
    }
    else if(mode == "calc")
    {
        try
        {
            return mill::mode_calc<vector_type>(argc_ - 1, argv_ + 1);
        }
        catch(std::exception& excpt)
        {
            std::cerr << "error: " << excpt.what() << std::endl;
            return 1;
        }
    }
    else if(mode == "help")
    {
        return mill::mode_help(argc_ - 1, argv_ + 1);
    }
    else
    {
        mill::log(mill::log_level::error, "unknown mode: ", mode, '\n');
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

std::pair<int, std::vector<const char*>> setup_logger(int argc, char **argv)
{
    // TODO simplify
    std::vector<const char*> commands;
    for(int i=0; i<argc; ++i)
    {
        const std::string opt(argv[i]);
        if(opt == "--debug")
        {
            mill::logger.activate(mill::log_level::debug);
            mill::logger.activate(mill::log_level::info);
            mill::logger.activate(mill::log_level::warn);
            mill::logger.activate(mill::log_level::error);
        }
        else if(opt == "--quiet")
        {
            mill::logger.deactivate(mill::log_level::debug);
            mill::logger.deactivate(mill::log_level::info);
            mill::logger.deactivate(mill::log_level::warn);
            mill::logger.activate  (mill::log_level::error);
        }
        else
        {
            commands.push_back(argv[i]);
        }
    }
    return std::make_pair(static_cast<int>(commands.size()), commands);
}

