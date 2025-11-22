//
// Created by tatovka on 11/22/25.
//

#include "ENFA.h"

ENFA::ENFA(uint32_t stateCount, uint32_t alphabetSize) : NFA(stateCount, alphabetSize) {};
ENFA::~ENFA() = default;
NFA ENFA::determinize() const { return toNFA().determinize(); }
bool ENFA::run(str_type& str) const { return false; }
void ENFA::saveToStream(std::ostream &stream) const { toNFA().saveToStream(stream); }

NFA ENFA::toNFA() const {
    NFA a(stateCount, alphabetSize);
    a.startStates = startStates;
    a.acceptStates = acceptStates;
    for (uint32_t i = 0; i < stateCount; ++i) {
        std::set<int> neighbours;
        std::set<int> curStates = std::set<int>({i});

        for (uint32_t j = 0; j <= stateCount; ++j) {
            if (curStates.empty()) break;
            if (j == stateCount) throw std::runtime_error("Epsilon cycle was founded");
            neighbours.insert(curStates.begin(), curStates.end());
            curStates = getEpsNeighbours(curStates);
        }

        for (uint32_t c = 0; c < alphabetSize; ++c) {
            for (int s: neighbours) {
                a.addTransitions(s, c, getTransitions(s, c));
            }
        }
    }
    return a;
}
