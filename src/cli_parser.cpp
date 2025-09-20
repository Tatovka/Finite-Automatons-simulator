//
// Created by tatovka on 4/28/25.
//

#include "cli_parser.h"
#include "iostream"

CLIParser::CLIParser(int argc, char** argv): argc(argc), argv(argv) {}

void CLIParser::parse() {
    std::string curFlag;
    if (mainFlags.contains("")) {
        mainFlags.erase("");
    }
    else if (argv[1][0] != '-') throw std::invalid_argument("expected flag, but given " + argv[1][0]);
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            curFlag = argv[i];
            if (!mainFlags.contains(curFlag)) {
                throw std::invalid_argument("unknown flag " + curFlag);
            }
            mainFlags.erase(curFlag);
        }
        else {
            flags[curFlag] += " ";
            flags[curFlag] += argv[i];
        }
    }
    if (!mainFlags.empty()) {
        throw std::invalid_argument("some obligatory flags was not given");
    }
}

void CLIParser::addFlag(const std::string& flag) {
    mainFlags.insert(flag);
}

std::string CLIParser::getFlag(const std::string& flag) const {
    return flags.at(flag);
}

