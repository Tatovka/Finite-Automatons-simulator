#include <iostream>
#include "NFA.h"
#include "cli_parser.h"

str_type readStr(std::string inp) {
    std::stringstream ss(inp);
    return std::vector(std::istream_iterator<uint32_t>(ss), std::istream_iterator<uint32_t>());
}

int main(int argc,  char **argv) {
    CLIParser cli(argc, argv);
    cli.addFlag("-f");
    cli.addFlag("-s");
    cli.parse();
    std::string inputPath = cli.getFlag("-f");
    std::string inp = cli.getFlag("-s");
    std::ifstream inputFile (inputPath, std::ios::binary);
    NFA nfa = NFA::loadFromStream(inputFile);
    str_type str = readStr(inp);
    std::cout << (nfa.run(str)? "true" : "false") << std::endl;
    return 0;
}