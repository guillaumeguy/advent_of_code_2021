//
// Created by guillaume_guy on 12/14/21.
//

#ifndef AOC2021_DAY15_H
#define AOC2021_DAY15_H

#endif //AOC2021_DAY15_H

#include "vector"
#include "iostream"
#include "macro.h"
#include "array"
#include "queue"

using namespace std;

struct Loc { // with risk
    int r;
    int c;
    int risk; // cost
};

class ComparisonClass {
public:
    bool operator()(Loc a, Loc b) {
        return a.risk > b.risk;
    }
};

int nrows;
int ncols;

array<pii, 4> moves = {{
                               {0, 1},
                               {1, 0},
                               {-1, 0},
                               {0, -1}
                       }};

bool is_valid_move(int r, int c, int nrows, int ncols) {
    return r >= 0 && r < nrows && c >= 0 && c <= ncols;
}

vector<vector<int>> generate_bigger_map(vector<vector<int>> &v) {

    vector<vector<int>> new_vec(nrows * 5, vector<int>(ncols * 5));
    for (int i = 0; i < 5; ++i) { // row multiplier
        for (int j = 0; j < 5; ++j) { // col multiplier
            for (int r = 0; r < nrows; ++r) {
                for (int c = 0; c < ncols; ++c) {
                    int new_val = v[r][c] + i + j ;
                    // max = 9 + 4 + 4 = 17
                    new_vec[nrows * i + r][ncols * j + c] = new_val >= 10 ? new_val % 9 : new_val; // lookup[v[r][c] + i + j];
                }
            }
        }
    }
    return new_vec;
}


int dijkstra(pii final_loc, vector<vector<int>> &risk_vec, int nrows, int ncols) {

    vector<vector<int>> risk_tot(nrows, vector<int>(ncols, INT_MAX));
    priority_queue<Loc, vector<Loc>, ComparisonClass> q; // locations that needs visiting
    q.push({0, 0, 0});

    int final_weight = INT_MAX;

    while (!q.empty()) {
        Loc loc = q.top();
        q.pop();

        if (loc.risk > final_weight) {
            // since edges can't be negative, loc.risk is already the risk level of a better path
            return final_weight;
        }

        for (const auto &m: moves) {
            int x = loc.r + m.first;
            int y = loc.c + m.second;
            if (is_valid_move(x, y, nrows, ncols)) {
                int risk = loc.risk + risk_vec[x][y];
                if (risk < risk_tot[x][y]) { // shorter path!
                    risk_tot[x][y] = risk; // overwrite
                    q.push({x, y, risk});
                }

                if (x == final_loc.first && y == final_loc.second) {
                    final_weight = min(final_weight, risk);
                }
            }
        }
    }
    return final_weight;
}


int task1() {

    string s;

    vector<vector<int>> risk_vec;

    while (getline(cin, s)) {
        vector<int> row(s.size());
        int i = 0;
        for (const auto c: s) {
            row[i++] = c - '0';
        }
        risk_vec.push_back(std::move(row));
    }

    nrows = risk_vec.size();
    ncols = risk_vec[0].size();

    int sol1 = dijkstra({nrows - 1, ncols - 1}, risk_vec, nrows, ncols);

    auto bigger_risk_vec = generate_bigger_map(risk_vec);

    int sol2 = dijkstra({5 * nrows - 1, 5 * ncols - 1}, bigger_risk_vec, 5 * nrows, 5 * ncols);

    cout << sol1 << " " << sol2 << endl;
    return 0;
}