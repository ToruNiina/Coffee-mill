#include "includes/CGMutator.hpp"
using namespace coffeemill;

int main(int argc, char *argv[])
{
    const size_t width_of_shell(75);
    std::ifstream ifs(argv[1]);
    if(ifs.fail())
    {
        std::cout << "file open error" << std::endl;
        return -1;
    }

    std::cout << "PDB CG(CafeMol) style Mutator" << std::endl;            
    std::cout << "please input sequence that you want." << std::endl;            
    std::cout << "letter = means the residue does not change" << std::endl;
    std::cout << "sequence of input PDB file indicated below." << std::endl;
    std::cout << "Note: only mutation is supported. insertion,";
    std::cout << " elongation, deletion is not supported yet." << std::endl;
    std::cout << std::endl;

    if(argc != 2)
    {
        std::cout << "Usage : ./mutator <filename>.pdb (CG style)" << std::endl;
        std::cout << std::endl;
        return -1;
    }

    const std::string prefix("out");
    const std::string suffix(".pdb");

    while(!ifs.eof())
    {
        CGChnSptr chain(new CGChain);
        chain->read_block(ifs);
        std::string input_sequence(chain->get_sequence());

        std::string mutated_sequence;

        bool pass_chain(false);
        if(input_sequence.size() < width_of_shell)
        {
            std::cout << "<<<< " << input_sequence << std::endl;
            std::cout << ">>>> ";
            std::cin >> mutated_sequence;
            if(mutated_sequence == "pass")
            {
                pass_chain = true;
            }
            if(mutated_sequence == "end"  ||
               mutated_sequence == "quit" ||
               mutated_sequence == "exit" ||
               mutated_sequence == "bye")
            {
                std::cout << "quit" << std::endl;
                break;
            }
        }
        else
        {
            std::string test;
            size_t current_pos(0);
            std::string buffer;

            bool end_program(false);
            for(size_t full_length(input_sequence.size());
                full_length > width_of_shell;
                full_length -= width_of_shell)
            {
                std::cout << "<<<< "
                          << input_sequence.substr(current_pos, width_of_shell)
                          << std::endl;
                std::cout << ">>>> ";
                std::cin  >> buffer;
                if(buffer == "pass")
                {
                    pass_chain = true;
                    break;//breaks "for sequence size" statement
                }
                if(buffer == "end" || buffer == "quit" ||
                   buffer == "exit" || buffer == "bye")
                {
                    end_program = true;
                    break;//breaks "for sequence size" statement
                }

                mutated_sequence += buffer;
                test += input_sequence.substr(current_pos, width_of_shell);
                current_pos += width_of_shell;
            }

            if(end_program)
            {
                std::cout << "quit" << std::endl;
                break;//breaks "while !eof" statement
            }
            if(!pass_chain)
            {
                std::cout << "<<<< " << input_sequence.substr(current_pos)
                          << std::endl;
                std::cout << ">>>> ";
                std::cin  >> buffer;

                mutated_sequence += buffer;
                test += input_sequence.substr(current_pos);
                if(test != input_sequence)
                    throw std::invalid_argument("test != input_sequence");
            }
        }

        char Chain_ID(chain->get_chainID());
        if(!pass_chain)
        {
            std::string outputfilename = prefix + Chain_ID + suffix;
            std::ofstream ofs(outputfilename.c_str());

            CGMutator mut(chain, mutated_sequence);
            mut.mutate();
            std::cout << "mutation completed" << std::endl;
            mut.output(ofs);
            std::cout << "output completed ==> " << outputfilename << std::endl;
        }
        else
        {
            std::cout << "chainID: " << Chain_ID << std::endl;
            std::cout << "passed. " << std::endl;
        }
    }
    return 0;
}
