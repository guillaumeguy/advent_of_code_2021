//
// Created by guillaume_guy on 12/28/21.
//

#ifndef AOC2021_DAY25_H
#define AOC2021_DAY25_H

#endif //AOC2021_DAY25_H

#include "vector"
#include "iostream"
#include "macro.h"
#include "array"
#include "omp.h"
#include <algorithm>

using namespace std;

constexpr bool VERBOSE = false;

array<pii, 4> moves = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

array<pii, 4> neighbors(int r, int c, int nrows, int ncols) {
    array<pii, 4> m;
    int i = 0;
    for (const auto &move: moves) {
        auto[dr, dc] = move;
        m[i++] = {(r + nrows + dr) % nrows, (c + ncols + dc) % ncols};
    }
    return m;
}


int task1() {

    string s;
    vector<vector<char>> input;

    while (getline(cin, s)) {
        input.emplace_back(s.begin(), s.end());
    }

    print_vec(input[0]);

    int ncols = input[0].size();
    int nrows = input.size();

    int steps = 10000;

    auto state = input;

    cout << "initial state" << endl;
    for (int r = 0; r < nrows; ++r) {
        print_vec(state[r]);
    }

    for (int step = 1; step <= steps; ++step) {
        vector<vector<char>> new_state(nrows, vector<char>(ncols, '.'));
        int move_cnt = 0;

        // now do '>'
        for (int r = 0; r < nrows; ++r) {
            for (int c = 0; c < ncols; ++c) {
                if (state[r][c] == '>') {
                    if (state[r][(c + 1 + ncols) % ncols] == '.') {
                        new_state[r][(c + 1 + ncols) % ncols] = '>';
                        move_cnt++;
                    } else {
                        // can't move
                        new_state[r][c] = '>';
                    }
                }
            }
        }

        // now do 'v'
        for (int r = 0; r < nrows; ++r) {
            for (int c = 0; c < ncols; ++c) {
                if (state[r][c] == 'v') {
                    char below = state[(r + nrows + 1) % nrows][c];
                    char below_fwd = new_state[(r + nrows + 1) % nrows][c]; // make sure a ">'" didn't move into it
                    if (below != 'v' && below_fwd == '.') {
                        new_state[(r + nrows + 1) % nrows][c] = 'v';
                        move_cnt++;
                    } else {
                        new_state[r][c] = 'v';
                    }
                }
            }
        }

        if (move_cnt == 0){
            cout << "Step = " << step << endl;
            break;
        }

        swap(state, new_state);

        if(step == steps){
            cout << " no solution found" << endl;
        }
    }



    return 0;
}