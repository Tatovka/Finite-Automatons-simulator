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

str_type readStr(std::string inp) {
    std::stringstream ss(inp);
    return std::vector(std::istream_iterator<uint32_t>(ss), std::istream_iterator<uint32_t>());
}

int main(int argc,  char **argv) {

    ANTLRInputStream input ("(0+|10)*9");
    RegExLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();
    for (auto token : tokens.getTokens()) {
        std::cout << token->toString() << std::endl;
    }
    RegExParser parser(&tokens);
    RegExVisitor visitor;
    auto tree = parser.re();

    auto enfa = visitor.visitRE(tree);
    cout << endl;
    cout << enfa.startStates[0]<<" " << *enfa.acceptStates.begin()<<endl;
    for (auto s: enfa.transitionFunc) {
        auto [x, y] = NFA::unzipTransition(s.first);
        cout << x<<" "<<y<<" "<<s.second[0]<<endl;
    }
    cout<<"eps"<<endl;
    for (auto s: enfa.epsTransitions) {
        for (int i:s.second)
        cout << s.first <<" "<<i<<endl;
    }


    // tree::ParseTree* tree = parser.main();

    // std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

    return 0;
    try {
        CLIParser cli(argc, argv);
        cli.addMainFlag("-f");
        cli.addOptFlag("-s");
        cli.addOptFlag("-o");
        cli.addOptFlag("-m");
        cli.addOptFlag("-e");
        cli.addOptFlag("-a");
        cli.parse();

        std::string inputPath = cli.getFlag("-f");
        std::ifstream inputFile (inputPath, std::ios::binary);
        if (!inputFile.is_open()) throw std::runtime_error("Could not open input file");
        auto str = cli.getOptFlag("-s");
        if (str.has_value()) {
            NFA nfa = NFA::loadFromStream(inputFile);
            inputFile.close();
            str_type tape = readStr(str.value());
            std::cout << "result on this string: " << (nfa.run(tape)? "true" : "false") << std::endl;
        }
        auto output = cli.getOptFlag("-o");
        if (output.has_value()) {
            inputFile.open(inputPath, std::ios::binary);
            if (!inputFile.is_open()) throw std::runtime_error("Could not open input file");
            NFA nfa = NFA::loadFromStream(inputFile);
            inputFile.close();
            std::ofstream outputFile (output.value(), std::ios::binary);
            if (!outputFile.is_open()) throw std::runtime_error("Could not open output file");
            nfa.determinize().saveToStream(outputFile);
            outputFile.close();
        }

        auto minOut = cli.getOptFlag("-m");
        if (minOut.has_value()) {
            inputFile.open(inputPath, std::ios::binary);
            if (!inputFile.is_open()) throw std::runtime_error("Could not open input file");
            std::ofstream outputFile (minOut.value(), std::ios::binary);
            if (!outputFile.is_open()) throw std::runtime_error("Could not open output file");
            DFA dfa = DFA::loadFromStream(inputFile);
            inputFile.close();
            dfa.minimize().saveToStream(outputFile);
        }
        auto eq = cli.getOptFlag("-e");
        if (eq.has_value()) {
            inputFile.open(inputPath, std::ios::binary);
            if (!inputFile.is_open()) throw std::runtime_error("Could not open first input file");
            std::ifstream inputFile2 (eq.value(), std::ios::binary);
            if (!inputFile2.is_open()) throw std::runtime_error("Could not open second input file");
            DFA dfa = DFA::loadFromStream(inputFile);
            DFA dfa2 = DFA::loadFromStream(inputFile2);
            inputFile.close();
            inputFile2.close();
            cout << "automatons " << (dfa.minimize() == dfa2.minimize()? "are ": "are not ") << "equal" << endl;
        }
        if (cli.detectedFlag("-a")) {
            inputFile.open(inputPath, std::ios::binary);
            if (!inputFile.is_open()) throw std::runtime_error("Could not open first input file");
            DFA dfa = DFA::loadFromStream(inputFile);
            cout << "automaton " <<
                (dfa.minimize() == DFA::alwaysAccepting(dfa.alphabetSize)? "accepts ": "does not accept ")
                << "every word" << endl;
        }
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}