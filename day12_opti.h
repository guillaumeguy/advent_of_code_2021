//
// Created by guillaume_guy on 12/12/21.
//

#ifndef AOC2021_DAY12_OPTI_H
#define AOC2021_DAY12_OPTI_H

#endif //AOC2021_DAY12_OPTI_H


#include "vector"
#include "string"
#include "iostream"
#include "unordered_map"
#include "unordered_set"
#include "macro.h"

using namespace std;

unordered_map<int, vector<int>> edges;
vector<bool> small_caverns;
int loc_end;
int loc_start;


bool is_lower(const string &s) {
    for (int i = 0; s[i] != 0; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z')
            return false;
    }
    return true;
}


int visit(int loc, vector<bool> &visited_small_caverns) {
    /*
     * Given current node s and current visit list (of small caverns)
     * Returns the count of paths
     */

    if (loc == loc_end) {
        return 1;
    }

    int cnt = 0;
    for (const auto &neighbor: edges[loc]) {
        // Big cavern, visit it
        if (!small_caverns[neighbor]) {
            // visit it
            cnt += visit(neighbor, visited_small_caverns);
        } else if (!visited_small_caverns[neighbor]) {
            // small cavern, not visited yet
            visited_small_caverns[neighbor] = true;  // add to visit list
            cnt += visit(neighbor, visited_small_caverns);
            visited_small_caverns[neighbor] = false;  // add to visit list
        } else {
            // nothing, we already visited that small cavern
        }
    }
    return cnt;
}


int visit2(int loc, vector<bool> &visited_small_caverns, bool small_bonus_used) {
    /*
     * Given current node s and current visit list (of small caverns)
     * Keep track of the bonus (to visit a small cave twice)
     * Returns the count of paths
     */

    if (loc == loc_end) {
        return 1;
    }

    if (small_bonus_used)
        return visit(loc, visited_small_caverns);

    int cnt = 0;
    for (const auto &neighbor: edges[loc]) {

        // Big cavern, visit it
        if (!small_caverns[neighbor]) {
            // visit it
            cnt += visit2(neighbor, visited_small_caverns, small_bonus_used);

        } else if (!visited_small_caverns[neighbor]) {
            // small cavern, not visited yet
            visited_small_caverns[neighbor] = true;  // add to visit list
            cnt += visit2(neighbor, visited_small_caverns, small_bonus_used);
            visited_small_caverns[neighbor] = false;  // add to visit list
        } else if (neighbor != loc_start) {
            // Small cave already visited
            // We already visited that small cavern, but we can do it again with our bonus!
            cnt += visit2(neighbor, visited_small_caverns, true);
        } else {

        }
    }

    return cnt;
}


int task1() {
    /*
     *
     * The optimization consists of:
     * Creating an incremental key for all strings and solving the solution with ints
     */
    string s;

    unordered_set<int> small_caverns_tmp;

    unordered_map<string, int> map;

    int cavern_counter = 0;
    while (getline(cin, s)) {
        auto v = split(s, "-");
        // 2 way edge
        for (const string &cavern: v) {
            // get cavern_id
            int cavern_id;

            // assign incremental value
            if (map.count(cavern) == 0) {
                map[cavern] = cavern_counter;
                cavern_id = cavern_counter;
                cavern_counter++;
                //what_is2(cavern_id,cavern_counter)
            } else {
                cavern_id = map[cavern];
            }

            if (is_lower(cavern)) {
                small_caverns_tmp.insert(cavern_id);
            }
            if (cavern == "start") {
                loc_start = cavern_id;
            } else if (cavern == "end") {
                loc_end = cavern_id;
            }
        }

        int c1 = map[v[0]];
        int c2 = map[v[1]];

        edges[c1].push_back(c2);
        edges[c2].push_back(c1);
    }


    for (int i = 0; i < cavern_counter; ++i)
        small_caverns.push_back(small_caverns_tmp.count(i));


    vector<bool> visited_small_caves(cavern_counter, false);

    visited_small_caves[loc_start] = true;
    auto visited_small_caves2 = visited_small_caves;
    cout << "sol1= " << visit(loc_start, visited_small_caves) << endl;
    cout << "sol2= " << visit2(loc_start, visited_small_caves2, false) << endl;

    return 0;
}