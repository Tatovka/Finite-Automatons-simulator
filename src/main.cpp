#include <iostream>
#include <fstream>
#include "NFA.h"
#include "cli_parser.h"

str_type readStr(std::string inp) {
    std::stringstream ss(inp);
    return std::vector(std::istream_iterator<uint32_t>(ss), std::istream_iterator<uint32_t>());
}

int main(int argc,  char **argv) {
    CLIParser cli(argc, argv);
    cli.addMainFlag("-f");
    cli.addOptFlag("-s");
    cli.addOptFlag("-o");
    cli.parse();

    std::string inputPath = cli.getFlag("-f");
    std::ifstream inputFile (inputPath, std::ios::binary);
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
        nfa.determinize().saveToStream(outputFile);
        outputFile.close();
    }
    return 0;
}