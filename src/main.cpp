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
    auto [argc_, argv_v] = setup_logger(argc, argv);
    const char** argv_ = argv_v.data();

    print_logo();

    if(argc_ < 2)
    {
        mill::log::error(mill::main_usage());
        return 1;
    }

    const std::string mode(argv_[1]);
    try
    {
        if(mode == "dcd")
        {
            return mill::mode_dcd(argc_ - 1, argv_ + 1);
        }
        else if(mode == "pdb")
        {
            return mill::mode_pdb(argc_ - 1, argv_ + 1);
        }
        else if(mode == "ninfo")
        {
            return mill::mode_ninfo(argc_ - 1, argv_ + 1);
        }
        else if(mode == "calc")
        {
            return mill::mode_calc(argc_ - 1, argv_ + 1);
        }
        else if(mode == "help")
        {
            return mill::mode_help(argc_ - 1, argv_ + 1);
        }
        else
        {
            return mill::mode_help(argc_ - 1, argv_ + 1);
        }
    }
    catch(std::exception& excpt)
    {
        mill::log::error(excpt.what());
        mill::log::error(mill::main_usage());
        return 1;
    }
}

void print_logo()
{
    mill::log::info(R"(  ___        __  __             __    __ _  _  _ )");
    mill::log::info(R"( / __| ___  / _|/ _| ___  ___   | \  / |(_)| || |)");
    mill::log::info(R"(| |   / _ \| |_| |_ / _ \/ _ \  |  \/  || || || |)");
    mill::log::info(R"(| |__| (_) |  _|  _|  __|  __/  | |\/| || || || |)");
    mill::log::info(R"( \___|\___/|_| |_|  \___|\___|  |_|  |_||_||_||_|)");
    mill::log::info(R"(                   Copyright 2016-now Toru Niina))");
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
            mill::log::logger.activate(mill::log::level::debug);
            mill::log::logger.activate(mill::log::level::info);
            mill::log::logger.activate(mill::log::level::warn);
            mill::log::logger.activate(mill::log::level::error);
        }
        else if(opt == "--quiet")
        {
            mill::log::logger.deactivate(mill::log::level::debug);
            mill::log::logger.deactivate(mill::log::level::info);
            mill::log::logger.deactivate(mill::log::level::warn);
            mill::log::logger.activate  (mill::log::level::error);
        }
        else
        {
            commands.push_back(argv[i]);
        }
    }
    return std::make_pair(static_cast<int>(commands.size()), commands);
}

