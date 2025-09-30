//
// Created by tatovka on 9/19/25.
//

#include "NFA.h"

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
        //if cannot go forward
        curPos? --curPos : 0;
        stack.pop();
    }
    return false;
}

std::set<uint32_t> NFA::getTransitions(const std::set<uint32_t>& fromStates, uint32_t chr) const {
    std::set<uint32_t> toStates;
    for (auto s: fromStates) {
        if (hasTransition(s, chr)) {
            auto transitions = getTransitions(s, chr);
            toStates.insert(transitions.begin(), transitions.end());
        }
    }
    return toStates;
}

struct DFAStates {
    uint32_t statesCount = 0;
    std::map<std::set<uint32_t>, uint32_t> statesMap;
    std::queue<decltype(statesMap)::iterator> mainQueue;
    uint32_t addState(std::set<uint32_t>&& states) {
        if (!statesMap.contains(states)) {
            statesMap.emplace(states, statesCount);
            auto node = statesMap.find(states);
            mainQueue.emplace(node);
            return statesCount++;
        }
        return statesMap[states];
    }

    uint32_t getNumber(const std::set<uint32_t>& states) {
        if (statesMap.contains(states)) {
            return statesMap[states];
        }
        statesMap[states] = statesCount;
        return statesCount++;
    }

    bool shouldContinue() { return !mainQueue.empty(); }

    auto nextState() {
        auto res = mainQueue.front();
        mainQueue.pop();
        return res;
    }

    uint32_t countStates() const {
        return statesCount;
    }
};

NFA NFA::determinize() const {
    DFAStates dfaStates;
    NFA dfa(-1, alphabetSize);
    dfa.addStartState(0);
    std::set startState(startStates.begin(), startStates.end());
    dfaStates.addState(std::move(startState));
    while (dfaStates.shouldContinue()) {
        auto node = dfaStates.nextState();
        std::set curState = node->first;
        uint32_t curNumber = node->second;
        std::set<uint32_t> transitionSet;
        for (uint32_t i = 0; i < alphabetSize; ++i) {
            auto toState = getTransitions(curState, i);
            bool isAccept = false;
            for (auto s: toState) {
                if (acceptStates.contains(s)) {
                    isAccept = true;
                    break;
                }
            }
            uint32_t toNumber = dfaStates.addState(std::move(toState));
            dfa.addTransition(curNumber, i, toNumber);
            if (isAccept) dfa.addAcceptState(toNumber);
        }
    }
    dfa.stateCount = dfaStates.countStates();
    return dfa;
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

void NFA::saveToStream(std::ostream& stream) const {
    stream << stateCount << '\n' << alphabetSize << '\n';
    for (auto ss: startStates) {
        stream << ss << " ";
    }
    stream << '\n';
    for (auto ss: acceptStates) {
        stream << ss << " ";
    }
    stream << "\n";
    for (auto tr: transitionFunc) {
        for (auto e: tr.second) {
            uint32_t first = tr.first >> 32;
            uint32_t second = tr.first & 0xFFFFFFFF;
            stream << first << " " << second << " " << e << "\n";
        }
    }
}