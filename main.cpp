#include <assert.h>
#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <set>
#include <stack>

using transition = std::unordered_map<uint64_t, std::vector<uint32_t>>;
using str_type = std::vector<uint32_t>;

struct NFA {
    uint32_t stateCount;
    uint32_t alphabetSize;
    std::vector<uint32_t> startStates;
    std::set<uint32_t> acceptStates;
    transition transitionFunc;
    NFA(uint32_t stateCount, uint32_t alphabetSize) : stateCount(stateCount), alphabetSize(alphabetSize), transitionFunc(stateCount) {}
    void addStartState(uint32_t state) {
        assert(stateCount > state);
        startStates.push_back(state);
    }
    void addAcceptState(uint32_t state) {
        assert(stateCount > state);
        acceptStates.insert(state);
    }

    void addTransition(uint32_t fromState, uint32_t chr, uint32_t toState) {
        assert(stateCount > fromState && stateCount > toState && alphabetSize > chr);
        transitionFunc[(uint64_t)fromState << 32 | chr].push_back(toState);
    }

    const std::vector<uint32_t>& getTransitions(uint32_t fromState, uint32_t chr) const {
        assert(stateCount > fromState && alphabetSize > chr);
        return transitionFunc.at((uint64_t)fromState << 32 | chr);
    }

    bool hasTransition(uint32_t fromState, uint32_t chr) const {
        assert(stateCount > fromState && alphabetSize > chr);
        return transitionFunc.find((uint64_t)fromState << 32 | chr) != transitionFunc.end();
    }

    static NFA loadFromStream(std::istream& stream) {
        uint32_t stateCount, alphabetSize;
        stream >> stateCount >> alphabetSize;
        NFA automaton(stateCount, alphabetSize);
        std::string statesS;
        std::getline(stream, statesS);
        assert(statesS.empty());
        std::getline(stream, statesS);
        std::stringstream ss(statesS);
        uint32_t state;
        while (ss >> state) {
            automaton.addStartState(state);
        }
        std::getline(stream, statesS);
        ss.clear();
        ss.str(statesS);
        while (ss >> state) {
            automaton.addAcceptState(state);
        }
        uint32_t from, chr, to;
        while (stream >> from >> chr >> to) {
            automaton.addTransition(from, chr, to);
        }
        return automaton;
    }
};

str_type readStr() {
    std::string inp;
    std::getline(std::cin, inp);
    std::stringstream ss(inp);
    return std::vector(std::istream_iterator<uint32_t>(ss), std::istream_iterator<uint32_t>());
}

bool runNFA(const NFA& a, const str_type& str, std::size_t pos = 0, int state = -1) {

    if (state == -1) {
        for (auto s: a.startStates) {
            if (runNFA(a, str, 0, s)) return true;
        }
        return false;
    }

    if (pos == str.size()) {
        return a.acceptStates.find(state) != a.acceptStates.end();
    }

    if (!a.hasTransition(state, str[pos])) return false;

    for (auto s: a.getTransitions(state, str[pos])) {
        std::cout << state <<"->"<< s  << " by " << str[pos]<< std::endl;
        if (runNFA(a, str, pos + 1, s)) {
            return true;
        }
    }

    return false;
}

int main() {
    std::ifstream inputFile ("automaton.txt", std::ios::binary);
    NFA nfa = NFA::loadFromStream(inputFile);
    str_type str = readStr();
    std::cout << (runNFA(nfa, str)? "true" : "false") << std::endl;
    return 0;
}