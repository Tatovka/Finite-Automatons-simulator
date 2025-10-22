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
    else if (argc < 2 || argv[1][0] != '-') throw std::invalid_argument(std::string("expected flag, but given ") + std::string(argv[1]));
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            curFlag = argv[i];
            detectedFlags.insert(curFlag);
            if (!mainFlags.contains(curFlag) && !optFlags.contains(curFlag)) {
                throw std::invalid_argument(std::string("unknown flag ") + curFlag);
            }
            mainFlags.erase(curFlag);
        }
        else {
            flags[curFlag] += argv[i];
        }
    }
    if (!mainFlags.empty()) {
        throw std::invalid_argument("some obligatory flags was not given");
    }
}

void CLIParser::addMainFlag(const std::string& flag) {
    mainFlags.insert(flag);
}

void CLIParser::addOptFlag(const std::string& flag) {
    optFlags.insert(flag);
}
std::string CLIParser::getFlag(const std::string& flag) const {
    return flags.at(flag);
}

std::optional<std::string> CLIParser::getOptFlag(const std::string& flag) const {
    if (flags.contains(flag)) {
        return {flags.at(flag)};
    }
    return std::nullopt;
}
