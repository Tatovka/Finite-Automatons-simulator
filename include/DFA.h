//
// Created by tatovka on 10/15/25.
//

#ifndef DFA_H
#define DFA_H
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <stack>

using std::pair;
using std::cout;
using std::endl;

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
                reachableIndex[rStates[i]] = i;
            }
        }

        int size() const {
            return rStates.size();
        }

        int isReachable(int state) const {
            return reachableIndex.contains(state);
        }

        int getDefaultIndex(int state) const { //returns index in states vector
            return rStates.at(state);
        }

        int getReachableIndex(int state) const { //returns index in imaginary vector of reachable states
            return reachableIndex.at(state);
        }
    };

    [[nodiscard]]reachable selectReachable() const {
        std::vector isReachable(states.size(), false);

        isReachable[startState] = true;
        std::stack<int> stack;
        stack.emplace(startState);
        int reachableStates = 1;
        while (!stack.empty()) {
            int curState = stack.top();
            stack.pop();
            for (auto next : states[curState].transitions) {
                if (next == -1 || isReachable[next]) continue;
                isReachable[next] = true;
                reachableStates++;
                stack.emplace(next);
            }
        }
        std::vector<int> indices;
        indices.reserve(reachableStates);
        for (int i = 0; i < states.size(); ++i) {
            if (isReachable[i]) {
                indices.push_back(i);
            }
        }
        return reachable{std::move(indices)};
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

        void setGroupByDefault(int defaultIndex, int group) { //set group id to state by index in the vector of states
            int index = reachableStates.getReachableIndex(defaultIndex);
            newGroups->at(index) = group;
            groupsMembers.at(group) = index;
        }

        void setGroup(int index, int group) { //set group id to state by index in imaginary vector of reachable states
            newGroups->at(index) = group;
            groupsMembers.at(group) = index;
        }

        int getGroup(int index) const {
            return prevGroups->at(index);
        }

        int getGroupByDefault(int defaultIndex) const {
            if (defaultIndex == -1) return -1;
            int index = reachableStates.getReachableIndex(defaultIndex);
            return prevGroups->at(index);
        }

        void swapBuffers() {
            std::swap(newGroups, prevGroups);
        }

        int getMember(int group) const {
            return groupsMembers.at(group);
        }

        int getMembersDefaultIndex(int group) const {
            int index = getMember(group);
            return reachableStates.getDefaultIndex(index);
        }

    };
    DFA minimize() const {
        if (states.size() <= 1) return *this;

        auto reachableStates = selectReachable();
        groups groups(reachableStates);

        int oldGroupsCount = 2;
        int curGroupsCount = 0;
        for (int i = 0; i < states.size(); i++) {
            if (states[i].isAccepting && reachableStates.isReachable(i)) {
                groups.setGroupByDefault(i, 1);
            }
        }
        groups.swapBuffers();
        while (oldGroupsCount != curGroupsCount) {
            oldGroupsCount = curGroupsCount;
            for (int sym = 0; sym < alphabetSize; sym++) {
                curGroupsCount = 0;
                std::unordered_map<pair<int,int>, int, PairHash> splitting;
                for (int i = 0; i < reachableStates.size(); i++) {
                    int defaultIndex = reachableStates.getDefaultIndex(i);
                    int toGroup = groups.getGroupByDefault(states[defaultIndex].transitions[sym]);
                    pair stateKey = {groups.getGroup(i), toGroup};
                    if (!splitting.contains(stateKey)) {
                        splitting[stateKey] = curGroupsCount;
                        groups.setGroup(i, curGroupsCount);
                        curGroupsCount++;
                    }
                    else groups.setGroup(i, splitting[stateKey]);
                }
                groups.swapBuffers();
            }
        }

        std::vector<DFAState> minDfaStates(curGroupsCount, {alphabetSize});
        for (int i = 0; i < curGroupsCount; i++) {
            int memberIndex = groups.getMembersDefaultIndex(i);
            const DFAState& groupMember = states[memberIndex];
            for (int sym = 0; sym < alphabetSize; sym++) {
                if (groupMember.transitions[sym] == -1) continue;
                int toGroup = groups.getGroupByDefault(groupMember.transitions[sym]);
                minDfaStates[i].addTransition({toGroup, sym});
                if (groupMember.isAccepting) minDfaStates[i].setAccepting();
            }
        }
        return {alphabetSize, groups.getGroupByDefault(startState), std::move(minDfaStates)};
    }

    int goBy(int state, int chr) const{
        return states[state].transitions[chr];
    }

    bool operator ==(const DFA& r) {
        if (states.size() != r.states.size()) return false;

        std::vector<int> lColors(states.size(), -1),
            rColors(states.size(), -1);
        lColors[startState] = 0;
        rColors[r.startState] = 0;
        int nextColor = 1;
        std::stack<pair<int,int>> stack;
        stack.emplace(startState, r.startState);

        while (!stack.empty()) {
            int lQ = stack.top().first;
            int rQ = stack.top().second;
            if (states[lQ].isAccepting != r.states[rQ].isAccepting) return false;
            stack.pop();
            for (int sym = 0; sym < alphabetSize; sym++) {
                int goLQ = goBy(lQ, sym);
                int goRQ = r.goBy(rQ, sym);
                if (goLQ == -1 && goRQ == -1) continue; // no transitions
                if (goLQ == -1 || goRQ == -1) return false;

                int goLColor = lColors[goLQ];
                int goRColor = rColors[goRQ];
                if (goLColor != goRColor) return false;
                if (goLColor == -1) {
                    lColors[goLQ] = rColors[goRQ] = nextColor++;
                    stack.emplace(goLQ, goRQ);
                }
            }
        }
        return true;
    }
    static DFA alwaysAccepting(int alphabetSize) {
        std::vector<pair<int, transition>> transitions;
        for (int i = 0; i < alphabetSize; i++) {
            transitions.push_back({0, {0, i}});
        }
        return {alphabetSize, 1, 0, transitions, {0}};
    }
};



#endif //DFA_H
