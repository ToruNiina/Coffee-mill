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
            {
            coffeemill::InputFileReader input(command.file());
            input.read();
            std::ofstream output(input.get_as<std::string>(input.at("information", "output")));
            if(!output.good()) throw std::runtime_error("file open error");

            coffeemill::PDBReader ref(input.get_as<std::string>(input.at("information", "structure")));
            ref.read();
            auto pdbdata = ref.atoms();

            ax::Matrix3 R(0e0);
            {
            auto data = input.split_list(input.at("information", "R"));
            std::vector<std::vector<double>> mat;
            for(auto iter = data.cbegin(); iter != data.cend(); ++iter)
                mat.push_back(input.get_as_list<double>(*iter));
            for(auto iter = mat.cbegin(); iter != mat.cend(); ++iter)
                assert(iter->size() == 3);
            assert(mat.size() == 3);

            for(std::size_t i=0; i<3; ++i)
                for(std::size_t j=0; j<3; ++j)
                    R(i,j) = mat.at(i).at(j);
            }
            std::cerr << R << std::endl;

            const auto mean_vec = input.get_as_list<double>(input.at("information", "centroid"));
            const ax::Vector3d mean(mean_vec[0], mean_vec[1], mean_vec[2]);

            for(auto iter = pdbdata.begin(); iter != pdbdata.end(); ++iter)
            {
                const ax::Vector3d tmp = R * ((*iter)->pos() - mean);
                (*iter)->pos() = tmp + mean;
                output << **iter << std::endl;
            }

            break;
            }
          case coffeemill::CommandLine::JOB::MUTATE:
            throw std::runtime_error("not implemented yet");
          case coffeemill::CommandLine::JOB::IMPOSE:
            {
            coffeemill::InputFileReader input(command.file());
            input.read();

            std::list<std::size_t> except_list;
            try
            {
                auto list_list = input.split_list(input.at("information", "except"));
                for(auto item : list_list)
                {
                    auto fragment = input.get_as_list<std::size_t>(item);

                    for(std::size_t pid = fragment.front(); pid <= fragment.back(); ++pid)
                    {
                        except_list.push_back(pid - 1);
                    }
                }
            }
            catch(std::out_of_range& except){}

            std::ofstream imposed(input.get_as<std::string>(input.at("information", "output")));
            if(!imposed.good()) throw std::runtime_error("file open error");

            std::vector<ax::Vector3d> ref_structure;
            {
            coffeemill::PDBReader ref(input.get_as<std::string>(input.at("information", "reference")));
            ref.read();
            auto data_ref = ref.atoms();
            ref_structure.reserve(data_ref.size());
            for(auto iter = data_ref.cbegin(); iter != data_ref.cend(); ++iter)
                ref_structure.push_back((*iter)->pos());
            }

            coffeemill::PDBReader tgt(input.get_as<std::string>(input.at("information", "target")));
            tgt.read();
            auto data_tgt = tgt.atoms();
            std::vector<ax::Vector3d> tgt_structure;
            {
            tgt_structure.reserve(data_tgt.size());
            for(auto iter = data_tgt.cbegin(); iter != data_tgt.cend(); ++iter)
                tgt_structure.push_back((*iter)->pos());
            ax::Vector3d tgt_sum(0e0);
            for(auto iter = tgt_structure.cbegin(); iter != tgt_structure.cend(); ++iter)
                tgt_sum += *iter;
            std::cerr << (tgt_sum / static_cast<double>(tgt_structure.size())) << std::endl;
            }

            if(ref_structure.size() != tgt_structure.size())
            {
                std::cerr << ref_structure.size() << std::endl;
                std::cerr << tgt_structure.size() << std::endl;
                throw std::invalid_argument("size diff");
            }

            ax::Vector3d sum(0e0);
            for(auto iter = ref_structure.cbegin(); iter != ref_structure.cend(); ++iter)
                sum += *iter;
            const ax::Vector3d ref_mean = sum / static_cast<double>(ref_structure.size());
//             std::cout << "center of ref : " << ref_mean << std::endl;

            coffeemill::SuperImpose impose;
            if(!except_list.empty())
                impose.except_list() = except_list;

            auto imposed_str = impose(std::make_pair(ref_structure, tgt_structure));
            for(auto iter = imposed_str.second.begin(); iter != imposed_str.second.end(); ++iter)
                *iter += ref_mean;

            for(std::size_t i=0; i<data_tgt.size(); ++i)
            {
                data_tgt.at(i)->pos() = imposed_str.second.at(i);
                imposed << *(data_tgt.at(i)) << std::endl;
            }
            imposed.close();
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
            const std::string dcdfname = command.file();
            const std::size_t frames   = std::stoi(command.at(4));
            coffeemill::DCDReader reader(dcdfname);
            reader.read();
            coffeemill::DCDData data = reader.data();
            data.traj().erase(data.traj().begin() + frames, data.traj().end());
            data.nset() = frames;
            coffeemill::DCDWriter writer(
                    dcdfname.substr(0, dcdfname.size()-4) + "_splitted.dcd");
            writer.data() = data;
            writer.write();
            break;
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
            try
            {
                auto list_list = input.split_list(input.at("information", "except"));
                for(auto item : list_list)
                {
                    auto fragment = input.get_as_list<std::size_t>(item);

                    for(std::size_t pid = fragment.front(); pid <= fragment.back(); ++pid)
                    {
                        except_list.push_back(pid - 1);
                    }
                }
            }
            catch(std::out_of_range& except){}
            coffeemill::SuperImpose impose;
            if(!except_list.empty())
                impose.except_list() = except_list;

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
