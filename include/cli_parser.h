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
#include <optional>

class CLIParser {
public:
    CLIParser(int argc, char** argv);
    void addMainFlag(const std::string& flag);
    void addOptFlag(const std::string& flag);
    void parse();
    std::string getFlag(const std::string& flag) const;
    std::optional<std::string> getOptFlag(const std::string& flag) const;
    bool detectedFlag(const std::string& flag) const { return detectedFlags.contains(flag); }
private:
    int argc;
    char** argv;
    std::set<std::string> mainFlags;
    std::set<std::string> optFlags;
    std::set<std::string> detectedFlags;
    std::unordered_map<std::string, std::string> flags;
};

#endif //CLI_PARSER_H
