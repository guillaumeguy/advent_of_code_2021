//
// Created by guillaume_guy on 12/11/21.
//

#ifndef AOC2021_DAY12_H
#define AOC2021_DAY12_H

#endif //AOC2021_DAY12_H

#include "vector"
#include "string"
#include "iostream"
#include "unordered_map"
#include "unordered_set"

using namespace std;

unordered_map<string, vector<string>> edges;
unordered_set<string> small_caverns;

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

bool is_lower(const string &s) {
    for (int i = 0; s[i] != 0; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z')
            return false;
    }
    return true;
}

bool is_small_cavern(string s, unordered_set<string> &small_caverns) {
    return small_caverns.count(s) == 1;
}

int visit(string loc, unordered_set<string>& visited_small_caverns) {
    /*
     * Given current node s and current visit list (of small caverns)
     * Returns the count of paths
     */

    if (loc == "end") {
        return 1;
    }

    int cnt = 0;
    for (const auto &neighbor: edges[loc]) {

        // Big cavern, visit it
        if (!is_small_cavern(neighbor, small_caverns)) {
            // visit it
            cnt += visit(neighbor, visited_small_caverns);

        } else if (visited_small_caverns.count(neighbor) == 0) {
            // small cavern, not visited yet
            visited_small_caverns.insert(neighbor);  // add to visit list
            cnt += visit(neighbor, visited_small_caverns);
            visited_small_caverns.erase(neighbor);
        } else {
            // nothing, we already visited that small cavern
        }
    }
    return cnt;
}


int visit2(string loc, unordered_set<string>& visited_small_caverns, bool small_bonus_used) {
    /*
     * Given current node s and current visit list (of small caverns)
     * Keep track of the bonus (to visit a small cave twice)
     * Returns the count of paths
     */

    if (loc == "end") {
        return 1;
    }

    if (small_bonus_used)
        return visit(loc, visited_small_caverns);

    int cnt = 0;
    for (const auto &neighbor: edges[loc]) {

        // Big cavern, visit it
        if (!is_small_cavern(neighbor, small_caverns)) {
            // visit it
            cnt += visit2(neighbor, visited_small_caverns, small_bonus_used);

        } else if (visited_small_caverns.count(neighbor) == 0) {
            // small cavern, not visited yet
            visited_small_caverns.insert(neighbor);  // add to visit list
            cnt += visit2(neighbor, visited_small_caverns, small_bonus_used);
            visited_small_caverns.erase(neighbor);  // add to visit list
        } else if (neighbor != "start") {
            // Small cave already visited
            // We already visited that small cavern, but we can do it again with our bonus!
            cnt += visit2(neighbor, visited_small_caverns, true);
        } else {

        }
    }

    return cnt;
}


int task1() {
    string s;

    while (getline(cin, s)) {
        auto v = split(s, "-");
        // 2 way edge
        for (const string &cavern: v) {
            if (is_lower(cavern)) {
                small_caverns.insert(cavern);
            }
        }

        edges[v[0]].push_back(v[1]);
        edges[v[1]].push_back(v[0]);
    }


    unordered_set<string>  visited_small_caves = {"start"};
    unordered_set<string>  visited_small_caves2 = {"start"};
    cout << "sol1= " << visit("start", visited_small_caves) << endl;
    cout << "sol2= " << visit2("start", visited_small_caves2, false) << endl;

};