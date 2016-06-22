#include "InputFileReader.hpp"
#include "PDBReader.hpp"
#include "DCDJoiner.hpp"
#include "DCDtoMovie.hpp"
#include "SequenceExtractor.hpp"
#include "NucleicSequence.hpp"
#include "DNAMutator.hpp"
#include "NinfoSplitter.hpp"
#include "SuperImpose.hpp"
#include "coffee_mill.hpp"
#include <fstream>
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
            {
            coffeemill::InputFileReader input(command.file());
            input.read();
            std::ofstream movie(input.get_as<std::string>(input.at("information", "output")));
            if(!movie.good()) throw std::runtime_error("output file open error");
            auto pdbnames = input.get_as_list<std::string>(input.at("information", "pdbfiles"));
            bool pymol = input.get_as<bool>(input.at("information", "pymol_format"));
            std::size_t index = 0;
            for(auto iter = pdbnames.cbegin(); iter != pdbnames.cend(); ++iter)
            {
                ++index;
                coffeemill::PDBReader reader(*iter);
                reader.read();
                auto structure = reader.atoms();
                if(pymol)
                    movie << "MODEL " << std::setw(9) << std::right << index << std::endl;
                else
                    movie << "<<<<" << std::setw(12) << std::right << index << std::endl;
                for(auto atom = structure.cbegin(); atom != structure.cend(); ++atom)
                    movie << **atom << std::endl;
                if(pymol)
                    movie << "TER " << std::endl << "ENDMDL" << std::endl;
                else
                    movie << ">>>>" << std::endl << "END" << std::endl;
            }
            break;
            }
          case coffeemill::CommandLine::JOB::COMPLEMENTAL:
            throw std::runtime_error("command not defined");
          case coffeemill::CommandLine::JOB::MUTATE:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::IMPOSE:
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
            }// job dna::seq (equal to pdb::seq)
          case coffeemill::CommandLine::JOB::SPLIT:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::JOIN:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::COMPLEMENTAL:
            {
            coffeemill::NucleicSequence seq_(command.at(3));
            std::cout << seq_.complemental();
            break;
            }
          case coffeemill::CommandLine::JOB::MUTATE:
            {
            coffeemill::InputFileReader input(command.file());
            input.read();
            coffeemill::PDBReader pdbreader(
                    input.get_as<std::string>(input.at("information", "pdbfile")));
            pdbreader.read();
            auto structure = pdbreader.parse();
            coffeemill::DNAMutator mutator;
            coffeemill::PDBChain chain = mutator.mutate(
                    input.get_as<std::string>(input.at("information", "sequence")),
                    structure.at(input.get_as<std::size_t>(input.at("information", "chain_index"))));
            std::ofstream ofs(
                    input.get_as<std::string>(input.at("information", "output")));
            if(!ofs.good()) throw std::runtime_error("output file open error");
            for(auto res = chain.cbegin(); res != chain.cend(); ++res)
                for(auto atom = (*res)->cbegin(); atom != (*res)->cend(); ++atom)
                    ofs << **atom << std::endl;;
 
            break;
            }
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
          case coffeemill::CommandLine::JOB::IMPOSE:
            {
            coffeemill::InputFileReader input(command.file());
            input.read();
            const std::string dcdfname = input.get_as<std::string>(
                    input.at("information", "dcdfile"));
            coffeemill::DCDReader reader(dcdfname);
            reader.read();
            auto data = reader.data();
            std::list<std::size_t> except_list;
            try{
                auto list_list = input.split_list(input.at("information", "except"));
                for(auto item : list_list)
                {
                    auto fragment = input.get_as_list<std::size_t>(item);
                    for(auto pid : fragment)
                    {
                        except_list.push_back(pid - 1);
                    }
                }
            }
            catch(std::out_of_range& except){}
            coffeemill::SuperImpose impose(except_list);
            data.traj() = impose(data.traj());

            const std::string outfname = input.get_as<std::string>(
                    input.at("information", "output"));
            coffeemill::DCDWriter writer(outfname);
            writer.data() = data;
            writer.write();
            break;
            }
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
        command.print_logo<MAJOR_VERSION, MINOR_VERSION>();

        switch(command.job())
        {
          case coffeemill::CommandLine::JOB::SEQ:
            {
            throw std::runtime_error("command not defined");
            }
          case coffeemill::CommandLine::JOB::SPLIT:
            {
            coffeemill::InputFileReader input(command.file());
            input.read();
//             std::cout << input.get_as<std::string>(
//                     input.at("information", "filename")) << std::endl;
            coffeemill::NinfoSplitter splitter(
                    input.get_as<std::string>(input.at("information", "filename")));
            auto data = splitter.split();
            splitter.write(data);
            if(input.get_as<bool>(input.at("information", "cafemol_input")))
            {
                splitter.write_cafe_inp(
                    input.get_as<std::size_t>(input.at("information", "simN")),
                    input.get_as<std::string>(input.at("information", "input_name")),
                    data);
            }
            break;
            }
          case coffeemill::CommandLine::JOB::JOIN:
            {
            throw std::runtime_error("not implemented yet");
            }
          case coffeemill::CommandLine::JOB::COMPLEMENTAL:
            {
            throw std::runtime_error("command not defined");
            }
          case coffeemill::CommandLine::JOB::MUTATE:
            {
            throw std::runtime_error("not implemented yet");
            }
          case coffeemill::CommandLine::JOB::SHOW:
            {
            throw std::runtime_error("not implemented yet");
            }
          case coffeemill::CommandLine::JOB::MAKE_CG:
            {
            throw std::runtime_error("not implemented yet");
            }
          case coffeemill::CommandLine::JOB::MAKE_NINFO:
            {
            throw std::runtime_error("command not defined");
            }
          case coffeemill::CommandLine::JOB::MAKE_MOVIE:
            {
            throw std::runtime_error("command not defined");
            }
          default:
            throw std::logic_error("never reach here");
        }
        break;
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
