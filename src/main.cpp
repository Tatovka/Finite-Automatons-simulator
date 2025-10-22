#include <filesystem>
#include <iostream>
#include <fstream>
#include "NFA.h"
#include "cli_parser.h"
#include "../include/DFA.h"

str_type readStr(std::string inp) {
    std::stringstream ss(inp);
    return std::vector(std::istream_iterator<uint32_t>(ss), std::istream_iterator<uint32_t>());
}

int main(int argc,  char **argv) {
    try {


        CLIParser cli(argc, argv);
        cli.addMainFlag("-f");
        cli.addOptFlag("-s");
        cli.addOptFlag("-o");
        cli.addOptFlag("-m");
        cli.parse();

        std::string inputPath = cli.getFlag("-f");
        std::ifstream inputFile (inputPath, std::ios::binary);
        if (!inputFile.is_open()) throw std::runtime_error("Could not open input file");

        NFA nfa = NFA::loadFromStream(inputFile);
        inputFile.close();
        auto str = cli.getOptFlag("-s");
        if (str.has_value()) {
            str_type tape = readStr(str.value());
            std::cout << (nfa.run(tape)? "true" : "false") << std::endl;
        }

        auto output = cli.getOptFlag("-o");
        if (output.has_value()) {
            std::ofstream outputFile (output.value(), std::ios::binary);
            if (!outputFile.is_open()) throw std::runtime_error("Could not open output file");
            nfa.determinize().saveToStream(outputFile);
            outputFile.close();
        }
        inputFile.close();
        inputFile.open(inputPath, std::ios::binary);

        DFA dfa = DFA::loadFromStream(inputFile);
        dfa.minimize().saveToStream(std::cout);
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}