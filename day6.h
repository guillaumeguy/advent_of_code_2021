//
// Created by guillaume_guy on 12/6/21.
//

#ifndef AOC2021_DAY6_H
#define AOC2021_DAY6_H

#endif //AOC2021_DAY6_H

#include "vector"
#include <iostream>
#include "math.h"
#include "numeric"

using namespace std;

#define ll long long

constexpr bool VERBOSE = false;


template<typename T>
typename std::enable_if<std::is_signed<T>::value, int>::type
inline constexpr signum(T const x) {
    return (T(0) < x) - (x < T(0));
}

template<typename T>
void print_vec(vector<T> &v) {
    for (int r = 0; r < v.size(); ++r)
        cout << v[r] << " ";

    cout << endl;
}

std::vector<std::string> split(std::string to_split, std::string delimiter) {
    size_t pos = 0;
    std::vector<std::string> matches{};
    do {
        pos = to_split.find(delimiter);
        int change_end;
        if (pos == std::string::npos) {
            pos = to_split.length() - 1;
            change_end = 1;
        } else {
            change_end = 0;
        }
        matches.push_back(to_split.substr(0, pos + change_end));

        to_split.erase(0, pos + delimiter.length());

    } while (!to_split.empty());
    return matches;

}

int task1() {

    // Parse first line
    std::string s;
    cin >> s;

    int MAX_DAY = 8;

    std::vector<ll> state(MAX_DAY+1, 0);
    vector<string> lantern_str = split(s, ",");

    for (const auto &l: lantern_str)
        state[stoll(l, nullptr, 10)]++;


    print_vec(state);
    for (int day = 1; day <= 256; ++day) {
        ll zero_day = state[0]; // save this to avoid over-write. They will spawn new lanternfish!
        for (int i = 1; i < (int) state.size(); ++i) {
            // day N becomes day N-1
            state[i - 1] = state[i];
        }
        state[8] = zero_day;
        state[6] += zero_day;
        cout << "day=" << day << endl;
        print_vec(state);
    }
    cout << std::accumulate(state.begin(), state.end(), 0LL) << endl;
    return 0;
}
