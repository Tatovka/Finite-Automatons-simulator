//
// Created by tatovka on 4/28/25.
//

#ifndef CLI_PARSER_H
#define CLI_PARSER_H
#include <string>
#include <sstream>
#include <cstring>
#include <unordered_map>
#include <set>
#include <cassert>

class CLIParser {
public:
    CLIParser(int argc, char** argv);
    void addFlag(const std::string& flag);
    void parse();
    std::string getFlag(const std::string& flag) const;
private:
    int argc;
    char** argv;
    std::set<std::string> mainFlags;
    std::unordered_map<std::string, std::string> flags;
};

#endif //CLI_PARSER_H
