//
// Created by tatovka on 9/19/25.
//

#ifndef NFA_H
#define NFA_H
#include <assert.h>
#include <stdint.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <set>
#include <stack>
#include <queue>
#include <map>

using transition = std::unordered_map<uint64_t, std::vector<uint32_t>>;
using str_type = std::vector<uint32_t>;

struct stateFrame {
    uint32_t state;
    uint32_t curChild;
    stateFrame(uint32_t state, uint32_t curChild) : state(state), curChild(curChild) {}
};

struct NFA {
    uint32_t stateCount;
    uint32_t alphabetSize;
    std::vector<uint32_t> startStates;
    std::set<uint32_t> acceptStates;
    transition transitionFunc;
    NFA(uint32_t stateCount, uint32_t alphabetSize) : stateCount(stateCount), alphabetSize(alphabetSize)/*, transitionFunc(stateCount)*/ {
        //std::cout << stateCount << std::endl;
    }
    void addStartState(uint32_t state);

    void addAcceptState(uint32_t state);

    void addTransition(uint32_t fromState, uint32_t chr, uint32_t toState);

    NFA determinize() const;

    const std::vector<uint32_t>& getTransitions(uint32_t fromState, uint32_t chr) const;

    bool hasTransition(uint32_t fromState, uint32_t chr) const;

    bool run(str_type& str) const;

    static NFA loadFromStream(std::istream& stream);

    void saveToStream(std::ostream& stream) const;

private:
    std::set<uint32_t> getTransitions(const std::set<uint32_t>& formStates, uint32_t chr) const;
};

#endif //NFA_H
