#include "InputFileReader.hpp"
#include "PDBReader.hpp"
#include "DCDJoiner.hpp"
#include "DCDtoMovie.hpp"
#include "SequenceExtractor.hpp"
#include "coffee_mill.hpp"
#ifndef MAJOR_VERSION
#define MAJOR_VERSION 0
#endif
#ifndef MINOR_VERSION
#define MINOR_VERSION 0
#endif

int main(int argc, char *argv[])
{
    coffeemill::CommandLine command(argc, argv);
    if(!command.parse())
    {
        command.print_logo<MAJOR_VERSION, MINOR_VERSION>();
        command.print_help();
        return EXIT_FAILURE;
    }

    try
    {
    switch(command.mode())
    {
      case coffeemill::CommandLine::MODE::PDB:
        {
        command.print_logo<MAJOR_VERSION, MINOR_VERSION>();

        switch(command.job())
        {
          case coffeemill::CommandLine::JOB::SEQ:
            {
            coffeemill::PDBReader reader(command.file());
            reader.read();
            auto structure = reader.parse();

            coffeemill::SequenceExtractor seqextr;
            for(auto chain = structure.cbegin();
                    chain != structure.cend(); ++chain)
            {
                std::cout << "chain " << chain->chain_id() << ": "
                          << seqextr(*chain) << std::endl;
            }

            break;
            }// job pdb::seq
          case coffeemill::CommandLine::JOB::SPLIT:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::JOIN:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::COMPLEMENTAL:
            throw std::runtime_error("command not defined");
          case coffeemill::CommandLine::JOB::MUTATE:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::SHOW:
            throw std::runtime_error("command not defined");
          case coffeemill::CommandLine::JOB::MAKE_CG:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::MAKE_NINFO:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::MAKE_MOVIE:
            throw std::runtime_error("command not defined");
          default:                           
            throw std::logic_error("never reach here");
        }
        break;
        }// mode pdb
      case coffeemill::CommandLine::MODE::DNA:
        {
            throw std::runtime_error("not implemented yet");
        }// mode dna
      case coffeemill::CommandLine::MODE::DCD:
        {
        command.print_logo<MAJOR_VERSION, MINOR_VERSION>();

        switch(command.job())
        {
          case coffeemill::CommandLine::JOB::SEQ:
            {
            throw std::runtime_error("command not defined");
            }
          case coffeemill::CommandLine::JOB::SPLIT:
            {
            throw std::runtime_error("not implemented yet");
            }
          case coffeemill::CommandLine::JOB::JOIN:
            {
            coffeemill::InputFileReader input(command.file());
            input.read();
            coffeemill::DCDJoiner joiner(input);
            joiner.join();
            break;
            }
          case coffeemill::CommandLine::JOB::COMPLEMENTAL:
            {
            throw std::runtime_error("command not defined");
            }
          case coffeemill::CommandLine::JOB::MUTATE:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::SHOW:
            throw std::runtime_error("command not defined");
          case coffeemill::CommandLine::JOB::MAKE_CG:
            throw std::runtime_error("command not defined");
          case coffeemill::CommandLine::JOB::MAKE_NINFO:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::MAKE_MOVIE:
            {
            coffeemill::InputFileReader input(command.file());
            input.read();
            coffeemill::DCDtoMovie dcd2movie(input);
            dcd2movie.convert();
            break;
            }
          default:
            throw std::logic_error("never reach here");
        }
        break;
        }// mode dcd
      case coffeemill::CommandLine::MODE::NINFO:
        {
            throw std::runtime_error("not implemented yet");
        }// mode ninfo
      case coffeemill::CommandLine::MODE::HELP:
        {
            command.print_help();
            break;
        }// mode help
      case coffeemill::CommandLine::MODE::MANUAL:
        {
            command.print_man();
            break;
        }// mode manual
      default:
        throw std::logic_error("never reach here");
    }// switch
    }// try
    catch(std::exception& except)
    {
        std::cerr << "Error: " << except.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
