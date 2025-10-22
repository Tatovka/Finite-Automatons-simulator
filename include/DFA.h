//
// Created by tatovka on 10/15/25.
//

#ifndef DFA_H
#define DFA_H
#include <algorithm>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <unordered_map>

using std::pair;

struct transition {
    int to;
    int sym;
};

struct DFAState {
    int group = 0;
    std::vector<int> transitions;
    bool isAccepting = false;
    DFAState(int alphabetSize) : transitions(alphabetSize, -1) {}
    DFAState(const DFAState& dfa) = default;
    bool addTransition(transition t) {
        if (t.sym >= transitions.size()){ return false; }
        transitions[t.sym] = t.to;
        return true;
    }
    void setAccepting() {
        isAccepting = true;
        group = 1;
    }
};

struct DFA {
    int alphabetSize;
    std::vector<DFAState> states;
    int startState;

    DFA(int alphabetSize, int statesCount, int startState,
        const std::vector<pair<int, transition>>& transitions,
        const std::vector<int>& acceptingStates) :
    alphabetSize(alphabetSize),
    states(statesCount, {alphabetSize}),
    startState(startState) {
        if (startState >= statesCount || startState < 0)
            throw std::invalid_argument("invalid start state");
        for (auto p: transitions) {
            if (!states[p.first].addTransition(p.second))
                throw std::invalid_argument("character is not in alphabet");
        }
        for (auto s  : acceptingStates) {
            if (s > statesCount)
                throw std::invalid_argument("invalid accepting state");
            states[s].setAccepting();
        }
    }

    DFA(int alphabetSize, int startState,
        std::vector<DFAState>&& stateList) :
    alphabetSize(alphabetSize),
    states(std::move(stateList)),
    startState(startState) {}

    static DFA loadFromStream(std::istream& stream) {
        int statesCount, alphabetSize;
        stream >> statesCount >> alphabetSize;

        int startState;
        stream >> startState;

        std::string acceptingLine;
        std::getline(stream, acceptingLine);
        std::getline(stream, acceptingLine);
        std::stringstream ss(acceptingLine);
        std::vector<int> acceptingStates;
        int state;
        while (ss >> state) {
            acceptingStates.push_back(state);
        }

        std::vector<pair<int, transition>> transitions;
        int from, chr, to;
        while (stream >> from >> chr >> to) {
            transitions.push_back({from, {to, chr}});
        }

        return {alphabetSize, statesCount, startState, transitions, acceptingStates};
    }

    void saveToStream(std::ostream& stream) {
        stream << states.size() << '\n' << alphabetSize << '\n';
        stream << startState << '\n';
        std::vector<pair<int, transition>> transitions;
        for (int i = 0; i < states.size(); ++i) {
            if (states[i].isAccepting)
                stream << i << " ";
            for (int sym = 0; sym < alphabetSize; ++sym) {
                if (states[i].transitions[sym] != -1)
                    transitions.push_back({i, {states[i].transitions[sym], sym}});
            }
        }
        stream << "\n";
        for (auto tr: transitions) {
            stream << tr.first << " " << tr.second.sym << " " << tr.second.to << '\n';
        }
    }

    struct PairHash {
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ h2 << 1;
        }
    };

    struct reachable {
        std::vector<int> rStates;
        std::unordered_map<int, int> reachableIndex;
        reachable(std::vector<int>&& indices) : rStates(std::move(indices)) {
            for (int i = 0; i < rStates.size(); ++i) {
                reachableIndex[indices[i]] = i;
            }
        }
        int size() const {
            return rStates.size();
        }
        int isReachable(int state) const {
            return reachableIndex.contains(state);
        }
    };

    [[nodiscard]]reachable selectReachable() const {
        std::vector<bool> isReachable(states.size());
        std::stack<int> stack;
        stack.emplace(startState);
        while (!stack.empty()) {
            int curState = stack.top();
            stack.pop();
            for (auto next : states[curState].transitions) {
                if (isReachable[next]) continue;
                isReachable[next] = true;
                stack.emplace(next);
            }
        }
        std::vector<int> indices;
        for (int i = 0; i < states.size(); ++i) {
            if (isReachable[i]) {
                indices.push_back(i);
            }
        }
        return {std::move(indices)};
    }

    struct groups {
        std::vector<int> groupsBuffer1;
        std::vector<int> groupsBuffer2;
        std::vector<int>* newGroups = &groupsBuffer1;
        std::vector<int>* prevGroups = &groupsBuffer2;
        std::vector<int> groupsMembers;

        const reachable& reachableStates;

        groups(const reachable& r):
            reachableStates(r),
            groupsBuffer1(r.size()),
            groupsBuffer2(r.size()),
            groupsMembers(r.size()) {}

        void setGroupByDefault(int defaultIndex, int group) {
            newGroups->at(reachableStates.reachableIndex.at(defaultIndex)) = group;
            groupsMembers.at(group) = defaultIndex;
        }

        void setGroup(int index, int group) {
            newGroups->at(index) = group;
            groupsMembers.at(group) = index;
        }

        int getGroup(int index) {
            return prevGroups->at(index);
        }
        int getGroupByDefault(int defaultIndex) {
            return prevGroups->at(reachableStates.reachableIndex.at(defaultIndex));
        }
        void swapBuffers() {
            std::swap(*newGroups, *prevGroups);
        }

    };
    DFA minimize() const {
        auto reachableStates = selectReachable();
        groups groups(reachableStates);

        std::vector<int> groupsBuffer1(states.size());
        std::vector<int> groupsBuffer2(states.size());

        std::vector<int>* newGroups = &groupsBuffer1;
        std::vector<int>* prevGroups = &groupsBuffer2;
        std::vector<int> groupsMembers(states.size());
        int oldGroupsCount = 2;
        int curGroupsCount = 0;
        for (int i = 0; i < states.size(); i++) {
            prevGroups->at(i) = states[i].isAccepting;
            if (reachableStates.isReachable(i)) groups.setGroupByDefault(i, 1);
        }
        while (oldGroupsCount != curGroupsCount) {
            oldGroupsCount = curGroupsCount;
            for (int sym = 0; sym < alphabetSize; sym++) {
                curGroupsCount = 0;
                std::unordered_map<pair<int,int>, int, PairHash> splitting;
                for (int i = 0; i < states.size(); i++) {
                    int toGroup = prevGroups->at(states[i].transitions[sym]);
                    pair stateKey = {prevGroups->at(i), toGroup};
                    if (splitting.find(stateKey) == splitting.end()) {
                        splitting[stateKey] = curGroupsCount;
                        newGroups->at(i) = curGroupsCount;
                        groupsMembers[curGroupsCount] = i;
                        curGroupsCount++;
                    }
                    else newGroups->at(i) = splitting[stateKey];
                }
                std::swap(prevGroups, newGroups);
            }
        }

        std::vector<DFAState> minDfaStates(curGroupsCount, {alphabetSize});
        for (int i = 0; i < curGroupsCount; i++) {
            const DFAState& groupMember = states[groupsMembers[i]];
            for (int sym = 0; sym < alphabetSize; sym++) {
                if (groupMember.transitions[sym] == -1) continue;
                int toGroup = prevGroups->at(groupMember.transitions[sym]);
                minDfaStates[i].addTransition({toGroup, sym});
                if (groupMember.isAccepting) minDfaStates[i].setAccepting();
            }
        }
        return {alphabetSize, prevGroups->at(startState), std::move(minDfaStates)};
    }
};



#endif //DFA_H
