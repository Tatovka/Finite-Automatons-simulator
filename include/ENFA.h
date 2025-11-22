//
// Created by tatovka on 11/22/25.
//

#ifndef ENFA_H
#define ENFA_H
#include <utility>
#include "NFA.h"


struct ENFA : NFA {
    ENFA(uint32_t stateCount, uint32_t alphabetSize);
    ~ENFA() override;
    std::map<int, std::vector<int>> epsTransitions;
    void addEpsTransition(int from, int to, int shift = 0) { epsTransitions[from + shift].push_back(to + shift); }
    NFA toNFA() const;
    NFA determinize() const override;
    bool run(str_type& str) const override;
    void saveToStream(std::ostream &stream) const override;

    std::set<int> getEpsNeighbours(std::set<int> states) const {
        std::set<int> res;
        for (int state : states) {
            if (epsTransitions.contains(state))
                res.insert(epsTransitions.at(state).begin(), epsTransitions.at(state).end());
        }
        return res;
    }

    static ENFA Eps(int alphabetSize) {
        auto a = ENFA(2, alphabetSize);
        a.addStartState(0);
        a.addAcceptState(1);
        a.addEpsTransition(0, 1);
        return a;
    }

    static ENFA Char(int alphabetSize, int c) {
        auto a = ENFA(2, alphabetSize);
        a.addStartState(0);
        a.addAcceptState(1);
        a.addTransition(0, c, 1);
        return a;
    }

    static ENFA Combine(const ENFA& l, const ENFA& r) {
        auto a = l;
        a.stateCount += r.stateCount;
        a.acceptStates.clear();

        for (int s: r.acceptStates) {
            a.acceptStates.insert(s + l.stateCount);
        }

        for (auto s: r.transitionFunc) {
            auto [from, chr] = unzipTransition(s.first);
            for (int to : s.second) a.addTransition(from + l.stateCount, chr, to + l.stateCount);;
        }

        for (auto s: r.epsTransitions) {
            auto [from, tos] = s;
            for (int to : tos) a.addEpsTransition(from, to, l.stateCount);
        }
        return a;
    }

    static ENFA Concat(const ENFA &l, const ENFA &r) {
        auto a = Combine(l, r);
        for (auto s: l.acceptStates)
            for (auto t: r.startStates) {
                a.addEpsTransition(s, l.stateCount + t);
            }
        return a;
    }

    static ENFA Alternate(const ENFA &l, const ENFA &r) {
        auto a = Combine(l, r);
        int sState = (a.stateCount += 2) - 2;
        int aState = sState + 1;

        for (auto s: l.startStates) {
            a.addEpsTransition(sState, s);
        }
        for (auto s: r.startStates) {
            a.addEpsTransition(sState, s + l.stateCount);
        }
        for (auto s: l.acceptStates) {
            a.addEpsTransition(s, aState);
        }
        for (auto s: r.acceptStates) {
            a.addEpsTransition(s + l.stateCount, aState);
        }
        a.startStates = {sState};
        a.acceptStates = {aState};
        return a;
    }

    static ENFA Astr(ENFA a) {
        int sState = (a.stateCount += 2) - 2;
        int aState = sState + 1;

        for (auto s: a.startStates) {
            a.addEpsTransition(sState, s);
        }
        for (auto s: a.acceptStates) {
            a.addEpsTransition(s, sState);
        }
        a.addEpsTransition(sState, aState);
        a.startStates = {sState};
        a.acceptStates = {aState};
        return a;
    }

    static ENFA Plus(const ENFA& a) {
        return Concat(Astr(a), a);
    }

    static ENFA Question(const ENFA& a) {
        return Alternate(a, Eps(a.alphabetSize));
    }

};



#endif //ENFA_H
