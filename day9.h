//
// Created by guillaume_guy on 12/7/21.
//

#ifndef AOC2021_DAY9_H
#define AOC2021_DAY9_H

#endif //AOC2021_DAY9_H

#include "vector"
#include "string"
#include "array"
#include "queue"
#include "iostream"
#include "numeric"
#include "macro.h"

using namespace std;

array<pair<int, int>, 4> moves = {{{0, -1}, {1, 0}, {-1, 0}, {0, 1}}};
constexpr bool VERBOSE = false;


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

inline bool is_lowest(vector<vector<int>> &v, int row, int col) {
    int val = v[row][col];
    for (const auto &m: moves) {
        int x = row + m.first;
        int y = col + m.second;
        if (x >= 0 && x < v.size() && y >= 0 && y < v[0].size()) {
            if (v[x][y] <= val) {
                return false;
            }
        }
    }
    return true;
}


int basin_size(vector<vector<int>> &v, int row, int col) {
    int cnt = 1;
    queue<pair<int, int>> q;
    vector<vector<bool>> visited(v.size(), vector<bool>(v[0].size(), false));
    auto basin = visited; // copy

    basin[row][col] = true;
    q.emplace(row, col);
    if(VERBOSE) cout << "starting basin=" << row << " " << col << endl;
    while (!q.empty()) {
        pair<int, int> p = q.front();
        q.pop();
        if (!visited[p.first][p.second]) {
            for (const auto &m: moves) {
                // does this point (x,y) only flow 1 way?
                int x = p.first + m.first;
                int y = p.second + m.second;
                if (x >= 0 && x < v.size() && y >= 0 && y < v[0].size() && !basin[x][y] && v[x][y] != 9) {
                    cnt++;
                    if (VERBOSE) cout << "x=" << x << "-" << y << " added" << endl;
                    basin[x][y] = true;
                    q.emplace(x, y);
                }
            }
            visited[p.first][p.second] = true;
        }
    }
    return cnt;
}


int task1() {

    string s;
    getline(cin, s);
    int y_dim = s.size();
    vector<vector<int>> input;
    int row = 0;
    do {
        vector<int> v(y_dim);
        for (int i = 0; i < y_dim; ++i) {
            v[i] = s[i] - '0';
        }
        input.emplace_back(std::move(v));
        row++;
    } while (getline(cin, s));

    int x_dim = row;

    cout << x_dim << " " << y_dim << endl;


    vector<pair<int, int>> lowest_pts;
    // part 1
    long long sum = 0LL;
    for (int r = 0; r < x_dim; ++r) {
        for (int c = 0; c < y_dim; ++c) {
            if (is_lowest(input, r, c)) {
                sum += input[r][c] + 1;
                lowest_pts.emplace_back(r, c);
            }
        }
    }
    cout << sum << endl;

    // part 2
    vector<long long> largest_basins;
    for (const auto &lowest_pt: lowest_pts) {
        long long cnt = basin_size(input, lowest_pt.first, lowest_pt.second);
        if(VERBOSE) cout << "basin" << lowest_pt.first << "-" << lowest_pt.second << " has size= " << cnt << endl;
        largest_basins.emplace_back(cnt);
    }

    std::sort(largest_basins.begin(), largest_basins.end(), greater<>());

    assert(largest_basins.size() >= 3);

    cout << std::accumulate(largest_basins.begin(), largest_basins.begin()+3, 1, [](long long a, long long b){return a*b;}) << endl;
    //1045660

    pii a = {1,2};

    cout << a << endl;

    return 0;
}

