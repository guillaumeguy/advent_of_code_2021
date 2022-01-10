//
// Created by guillaume_guy on 12/14/21.
//

#ifndef AOC2021_DAY14_H
#define AOC2021_DAY14_H

#endif //AOC2021_DAY14_H

#include "vector"
#include "string"
#include "iostream"
#include "unordered_map"
#include "unordered_set"
#include "macro.h"
#include "array"
#include <assert.h>     /* assert */

using namespace std;


constexpr int N = 256;
constexpr int e = 8;
constexpr int mask = (1 << e)-1;
static_assert((1 << e) == N);

struct Node {
    Node *next;
    char val;

    bool has_next() {
        return next != nullptr;
    }

    ~Node() {
        delete next;
    }
};

char left_char(int h) {
    return (char) (h >> e);
}

char right_char(int h) {
    return (char) (h & mask);
}

int get_hash(char c1, char c2) {
    return (c1 << e) + c2;
}

void print_counts(array<ll, N> &arr) {
    for (int i = 0; i < N; ++i) {
        if (arr[i] > 0) {
            cout << (char) i << " " << arr[i] << endl;
        }
    }
}

int task1() {

    string s;
    string templt;

    array<char, N * N> insert_alt = {};
    insert_alt.fill('0');

    getline(cin, templt);

    while (getline(cin, s)) {

        // Separate instructions from points
        if (!s.empty()) {
            auto pos = s.find('>');
            int hash1 = get_hash(s[0], s[1]);
            insert_alt[hash1] = s[pos + 2];
        }
    }

    int steps = 10;

    Node poly_template = {nullptr, templt[0]};
    Node *current = &poly_template;
    for (int i = 1; i < templt.size(); ++i) {
        Node *next = new Node({nullptr, templt[i]});
        current->next = next;
        current = next;
    }


    for (int step = 0; step < steps; ++step) {
        current = &poly_template;
        while (current->has_next()) {
            int hash = get_hash(current->val, current->next->val);
            if (insert_alt[hash] != '0') {
                // We need to insert a node
                Node *node_to_insert = new Node({current->next, insert_alt[hash]});
                current->next = node_to_insert;
                current = current->next->next; // we need two jumps
            } else {
                current = current->next;
            }
        }
    }

    // print the final step
    unordered_map<char, int> counts;
    current = &poly_template;
    while (current) {
        counts[current->val]++;
        current = current->next;
    }

    for (const auto &item: counts) {
        cout << item.first << " " << item.second << endl;
    }

    int max_val = 0;
    int min_val = INT_MAX;

    for (const auto &item: counts) {
        if (item.second > max_val) {
            max_val = item.second;
        }
        if (item.second < min_val) {
            min_val = item.second;
        }
    }
    cout << "sol1=" << min_val * max_val << endl;


    return 0;
}


int task2() {
    string s;
    string templt;

    array<ll, N> counter = {}; // count the char at any given step
    counter.fill(0LL);

    array<char, N * N> insert_alt = {};
    insert_alt.fill('0');

    unordered_map<int, ll> tracker; // count the char pairs at any given step

    // 1- Parsing
    getline(cin, templt);

    for (int i = 0; i < templt.size() - 1; ++i) {
        // populate count table
        int hash1 = get_hash(templt[i], templt[i + 1]);
        tracker[hash1]++;
    }

    for (auto c: templt)
        counter[c]++;

    while (getline(cin, s)) {
        if (!s.empty()) {
            auto pos = s.find('>');
            int hash1 = get_hash(s[0], s[1]);
            insert_alt[hash1] = s[pos + 2];
        }
    }

    // 2- Processing
    int steps = 40;
    unordered_map<int, ll> new_tracker; // hash -> counts

    for (int step = 0; step < steps; ++step) {
        new_tracker.clear();
        for (const auto &it: tracker) {
            int old_hash = it.first;
            if (insert_alt[old_hash] != '0') {
                // we will insert a new char
                // AB ==> A-C and C-B
                // We recompute the hashes
                int left_hash = get_hash(left_char(old_hash), insert_alt[old_hash]);
                int right_hash = get_hash(insert_alt[old_hash], right_char(old_hash));

                new_tracker[left_hash] += it.second;
                new_tracker[right_hash] += it.second;

                counter[insert_alt[old_hash]] += it.second; // the middle char gets inserted
            } else {
                new_tracker[old_hash] += it.second; // carry over
            }
        }
        swap(new_tracker, tracker);
    }

    cout << "step=" << steps << endl;
    print_counts(counter);

    return 0;
}