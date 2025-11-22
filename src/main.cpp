#include <filesystem>
#include <iostream>
#include <fstream>
#include "NFA.h"
#include "cli_parser.h"
#include "../include/DFA.h"
#include "antlr4-runtime.h"

#include "antlr4-runtime.h"
#include "RegExBaseListener.h"
#include "RegExParser.h"
#include "RegExLexer.h"
#include "RegExVisitor.h"

using namespace antlr4;

str_type readStr(const std::string& inp) {
    std::stringstream ss(inp);
    return std::vector(std::istream_iterator<uint32_t>(ss), std::istream_iterator<uint32_t>());
}

void runFromFile(std::istream& inputFile, const std::string& str) {
    NFA nfa = NFA::loadFromStream(inputFile);
    str_type tape = readStr(str);
    std::cout << "result on this string: " << (nfa.run(tape)? "true" : "false") << std::endl;
}

void determinizeFromFile(std::istream& inputFile, const std::string&& outputPath) {
    NFA nfa = NFA::loadFromStream(inputFile);
    std::ofstream outputFile (outputPath, std::ios::binary);
    if (!outputFile.is_open()) throw std::runtime_error("Could not open output file");
    nfa.determinize().saveToStream(outputFile);
    outputFile.close();
}

void minimizeFromFile(std::istream& inputFile, const std::string&& outputPath) {
    std::ofstream outputFile (outputPath, std::ios::binary);
    if (!outputFile.is_open()) throw std::runtime_error("Could not open output file");
    DFA dfa = DFA::loadFromStream(inputFile);
    dfa.minimize().saveToStream(outputFile);
    outputFile.close();
}

void areEqual(std::istream& inputFile, const std::string&& secondFilePath) {
    std::ifstream inputFile2 (secondFilePath, std::ios::binary);
    if (!inputFile2.is_open()) throw std::runtime_error("Could not open second input file");
    DFA dfa = DFA::loadFromStream(inputFile);
    DFA dfa2 = DFA::loadFromStream(inputFile2);
    inputFile2.close();
    cout << "automatons " << (dfa.minimize() == dfa2.minimize()? "are ": "are not ") << "equal" << endl;
}

void reToAutomaton(std::string& inputRe, const std::string&& outputPath) {
    std::ofstream outputFile (outputPath, std::ios::binary);
    if (!outputFile.is_open()) throw std::runtime_error("Could not open output file");

    ANTLRInputStream input (inputRe);
    RegExLexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    RegExParser parser(&tokens);
    RegExVisitor visitor;
    auto tree = parser.re();

    auto enfa = visitor.visitRE(tree);
    auto nfa = enfa.toNFA();
    nfa.saveToStream(outputFile);
    outputFile.close();
}

int main(int argc,  char **argv) {
    try {
        CLIParser cli(argc, argv);
        cli.addMainFlag("-f");
        cli.addOptFlag("-s");
        cli.addOptFlag("-d");
        cli.addOptFlag("-m");
        cli.addOptFlag("-e");
        cli.addOptFlag("-a");
        cli.addOptFlag("-re");
        cli.parse();

        std::string inputPath = cli.getFlag("-f");
        std::ifstream inputFile (inputPath, std::ios::binary);
        if (!inputFile.is_open()) throw std::runtime_error("Could not open input file");

        std::stringstream inputFileStream;
        inputFileStream << inputFile.rdbuf();
        inputFile.close();

        std::string inputFileData = inputFileStream.str();

        auto str = cli.getOptFlag("-s");
        if (str.has_value()) {
            runFromFile(inputFileStream, str.value());
            inputFileStream.str(inputFileData);
        }

        auto output = cli.getOptFlag("-d");
        if (output.has_value()) {
            determinizeFromFile(inputFileStream, std::move(output.value()));
            inputFileStream.str(inputFileData);
        }

        auto minOut = cli.getOptFlag("-m");
        if (minOut.has_value()) {
            determinizeFromFile(inputFileStream, std::move(minOut.value()));
            inputFileStream.str(inputFileData);
        }

        auto eq = cli.getOptFlag("-e");
        if (eq.has_value()) {
            areEqual(inputFileStream, std::move(eq.value()));
            inputFileStream.str(inputFileData);
        }

        if (cli.detectedFlag("-a")) {
            DFA dfa = DFA::loadFromStream(inputFileStream);
            cout << "automaton " <<
                (dfa.minimize() == DFA::alwaysAccepting(dfa.alphabetSize)? "accepts ": "does not accept ")
                << "every word" << endl;
            inputFileStream.str(inputFileData);
        }

        auto re = cli.getOptFlag("-re");
        if (re.has_value()) {
            reToAutomaton(inputFileData, std::move(re.value()));
        }

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}