#include <mill/math/Vector.hpp>
#include <mill/util/logger.hpp>
#include <iostream>
#include "mode_help.hpp"
#include "mode_dcd.hpp"
#include "mode_pdb.hpp"
#include "mode_ninfo.hpp"
#include "mode_calc.hpp"

void print_logo();
std::deque<std::string_view> get_commands(int argc, char **argv);

int main(int argc, char **argv)
{
    auto args = get_commands(argc, argv);
    print_logo();

    if(args.size() < 2)
    {
        mill::log::error(mill::main_usage());
        return 1;
    }

    const auto mode = args.front();
    try
    {
        if(mode == "dcd")
        {
            return mill::mode_dcd(std::move(args));
        }
        else if(mode == "pdb")
        {
            return mill::mode_pdb(std::move(args));
        }
        else if(mode == "ninfo")
        {
            return mill::mode_ninfo(std::move(args));
        }
        else if(mode == "calc")
        {
            return mill::mode_calc(std::move(args));
        }
        else if(mode == "help")
        {
            return mill::mode_help(std::move(args));
        }
        else
        {
            return mill::mode_help(std::move(args));
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

std::deque<std::string_view> get_commands(int argc, char **argv)
{
    std::deque<std::string_view> commands;
    for(int i=1; i<argc; ++i) // XXX: discard 0-th argument ("mill", normally)
    {
        const std::string_view opt(argv[i]);
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
            commands.emplace_back(argv[i]);
        }
    }
    return commands;
}

