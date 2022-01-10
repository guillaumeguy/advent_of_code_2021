//
// Created by guillaume_guy on 12/2/21.
//

#ifndef AOC2021_DAY3_H
#define AOC2021_DAY3_H

#endif //AOC2021_DAY3_H

#include <vector>

using namespace std;

constexpr bool VERBOSE = false;

/*
 * We will use a trie to store the string (1 char per node)
 * The approach below is inspired by this doc: https://www.geeksforgeeks.org/trie-insert-and-search/
 */
struct TrieNode {
    struct TrieNode *children[2] = {nullptr, nullptr};
    int net_ones = 0; // cnt(1) - cnt(0)

    ~TrieNode() { // this will recursively delete all children. Nice!
            delete children[0];
            delete children[1];
    };
};

void insert(TrieNode *root, string &s) {
    for (char c: s) {
        int ind = c == '1';
        root->net_ones += (ind ? 1 : -1);
        if (!root->children[ind]) {
            // there is no child that matches the prefix, let's create a new one
            root->children[ind] = new TrieNode();
        }
        root = root->children[ind];
    }
}

unsigned long long bin_to_int(string &s) {
    unsigned long long result = 0;

    for (char c: s) {
        result = (result << 1) + (c == '1');
    }
    return result;
}

string find_trie(TrieNode *root, bool reverse, int n) {
    string sol;
    if (VERBOSE) cout << "starting match" << endl;
    for (int i = 0; i < n; ++i) {
        char c = root->net_ones >= 0 ? '1' : '0';

        // reverse (oxygen): flip if possible
        if (reverse && root->children[c != '1']) c = c == '1' ? '0' : '1';
        int ind = c == '1';

        if (VERBOSE) cout << c << " with net cnt " << root->net_ones << endl;
        sol += c;
        root = root->children[ind];
    }
    return sol;
}

int task1() {

    string s;

    cin >> s;
    int n = s.size();
    vector<int> delta_cnt(n, 0);

    do {
        for (int i = 0; i < n; ++i) { ;
            delta_cnt[i] += s[i] == '1' ? 1 : -1;
        }
    } while (cin >> s);

    unsigned long long result = 0;

    for (int net_cnt: delta_cnt) {
        result = (result << 1) + (net_cnt >= 0);
    }

    unsigned long long bitmask = (1 << (n+1)) -1; // first n bits as 1. Trick: 2^(n+1) - 1

    cout << result << " " << (bitmask & (~result)) << endl;

}


int task2() {

    string s;

    TrieNode root;

    cin >> s;
    int n = s.size();
    vector<int> net_ones(n, 0);

    do {
        for (int i = 0; i < n; ++i) { ;
            net_ones[i] += s[i] == '1' ? 1 : -1;
        }
        insert(&root, s);
    } while (cin >> s);

    unsigned long long result = 0;
    string bin_result; // results in binary form

    for (int i = 0; i < n; ++i) {
        result = (result << 1) + (net_ones[i] >= 0);
        bin_result += (net_ones[i] >= 0 ? '1' : '0');
    }

    string oxygen = find_trie(&root, false, n);
    string co2_scrubber = find_trie(&root, true, n);

    cout << oxygen << " " << bin_to_int(oxygen) << endl;
    cout << co2_scrubber << " " << bin_to_int(co2_scrubber) << endl;

}