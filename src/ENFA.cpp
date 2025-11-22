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
    // a.transitionFunc.clear();
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
        for (int s: neighbours)
            if (acceptStates.contains(s)) a.addAcceptState(i);
        for (uint32_t c = 0; c < alphabetSize; ++c) {
            for (int s: neighbours) {
                a.addTransitions(i, c, getTransitions(s, c));
            }
        }
    }
    return a;
}

// for (auto s: nfa.transitionFunc) {
//     auto [x, y] = NFA::unzipTransition(s.first);
//     cout << x<<" "<<y<<" "<<s.second[0]<<endl;
// }
//nfa.saveToStream(cout);

// std::ifstream inp("sample_nfa/regex");
// auto dfa = DFA::loadFromStream(inp).minimize();
// dfa.saveToStream(cout);
// tree::ParseTree* tree = parser.main();

// std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

// cout << endl;
// cout << enfa.startStates[0]<<" " << *enfa.acceptStates.begin()<<endl;
// for (auto s: enfa.transitionFunc) {
//     auto [x, y] = NFA::unzipTransition(s.first);
//     cout << x<<" "<<y<<" "<<s.second[0]<<endl;
// }
// cout<<"eps"<<endl;
// for (auto s: enfa.epsTransitions) {
//     for (int i:s.second)
//         cout << s.first <<" "<<i<<endl;
// }
//
// cout << endl;