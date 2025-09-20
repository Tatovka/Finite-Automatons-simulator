//
// Created by tatovka on 9/19/25.
//

#include "../include/NFA.h"

void NFA::addStartState(uint32_t state) {
    assert(stateCount > state);
    startStates.push_back(state);
}

void NFA::addAcceptState(uint32_t state) {
    assert(stateCount > state);
    acceptStates.insert(state);
}

void NFA::addTransition(uint32_t fromState, uint32_t chr, uint32_t toState) {
    assert(stateCount > fromState && stateCount > toState && alphabetSize > chr);
    transitionFunc[(uint64_t)fromState << 32 | chr].push_back(toState);
}

const std::vector<uint32_t>& NFA::getTransitions(uint32_t fromState, uint32_t chr) const {
    assert(stateCount > fromState && alphabetSize > chr);
    return transitionFunc.at((uint64_t)fromState << 32 | chr);
}

bool NFA::hasTransition(uint32_t fromState, uint32_t chr) const {
    assert(stateCount > fromState && alphabetSize > chr);
    return transitionFunc.find((uint64_t)fromState << 32 | chr) != transitionFunc.end();
}

bool NFA::run(str_type& str) const {
    std::stack<stateFrame> stack;
    for (auto s: startStates)
        stack.emplace(s, 0);

    uint32_t curPos = 0;

    while (!stack.empty()) {
        stateFrame& curState = stack.top();
        if (curPos == str.size()) { //end of string
            if (acceptStates.find(curState.state) != acceptStates.end()) return true;
            curPos? --curPos : 0;
            stack.pop();
            continue;
        }

        if (hasTransition(curState.state, str[curPos])) { //go forward to the next child
            auto& transitions = getTransitions(curState.state, str[curPos]);
            if (curState.curChild < transitions.size()) {
                stack.emplace(transitions[curState.curChild], 0);
                curState.curChild++;
                ++curPos;
                continue;
            }
        }
        //if cannot go forward anymore
        curPos? --curPos : 0;
        stack.pop();
    }
    return false;
}

NFA NFA::loadFromStream(std::istream& stream) {
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