//
// Created by guillaume_guy on 12/10/21.
//

#ifndef AOC2021_DAY10_H
#define AOC2021_DAY10_H

#endif //AOC2021_DAY10_H


#include "vector"
#include "string"
#include "unordered_map"
#include "stack"
#include "iostream"
#include "numeric"
#include "macro.h"

using namespace std;

bool opening(char c) {
    return c == '(' || c == '[' || c == '<' || c == '{';
}

bool closing(char c) {
    return !opening(c);
}

typedef long long ll;

// opening ==> closing
unordered_map<char, char> lookup({{'(', ')'},
                                  {'[', ']'},
                                  {'<', '>'},
                                  {'{', '}'}});

ll score(char c) {
    if (c == ')') {
        return 3LL;
    } else if (c == ']') {
        return 57LL;
    } else if (c == '}') {
        return 1197LL;
    } else if (c == '>') {
        return 25137LL;
    } else {
        throw runtime_error("missing symbol" + string({c}));
    }
}

ll auto_complete_score(char c) {
    c = lookup[c]; // suggestion = opposite of input;
    if (c == ')') {
        return 1;
    } else if (c == ']') {
        return 2;
    } else if (c == '}') {
        return 3;
    } else if (c == '>') {
        return 4;
    } else {
        throw runtime_error("missing symbol" + string({c}));
    }
}

int task1() {
    /*
     * We use a stack to keep track of open symbols. The last symbol added is the first one to close
     * If we encounter a closing symbol that matches, we just pop the stack.
     */
    string s;
    ll score_tracker = 0;
    vector<ll> score_tracker_2;
    vector<string> incomplete_lines;
    while (getline(cin, s)) {
        stack<char> st;
        bool corrupt = false;
        for (const char c: s) {
            if (opening(c)) {
                st.emplace(c);
            } else {
                // closing
                if (st.empty()) {
                    score_tracker += score(c);
                    corrupt = true;
                    break;
                } else { // non-empty
                    char t = st.top();
                    if (c == lookup[t]) {
                        st.pop();
                    } else {
                        score_tracker += score(c);
                        corrupt = true;
                        break;
                    }
                }
            }
        }
        if (!corrupt) {
            ll score_2 = 0;
            while (!st.empty()) {
                score_2 *= 5;
                score_2 += auto_complete_score(st.top());
                st.pop();
            }
            /*
             * SOL 1=362271
               SOL 2=1698395182
             */

            score_tracker_2.push_back(score_2);

        }
    }
    cout << "SOL 1=" << score_tracker << endl;

    // compute the median
    auto m = score_tracker_2.begin() + score_tracker_2.size() / 2;
    std::nth_element(score_tracker_2.begin(), m, score_tracker_2.end());

    cout << "SOL 2=" << score_tracker_2[score_tracker_2.size() / 2] << endl;

}

