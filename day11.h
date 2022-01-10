//
// Created by guillaume_guy on 12/11/21.
//

#ifndef AOC2021_DAY11_H
#define AOC2021_DAY11_H

#endif //AOC2021_DAY11_H


#include "vector"
#include "array"
#include "string"
#include "stack"
#include "iostream"
#include "queue"

using namespace std;

typedef long long ll;

struct Move {
    int x, y;
};

array<Move, 8> moves = {{{0, -1}, {1, 0}, {-1, 0}, {0, 1}, // non-diag
                         {-1, -1}, {1, 1}, {1, -1}, {-1, 1}, // diag
                        }};


struct Task {

    inline bool is_valid(int r, int c, int rows, int cols) {
        return r >= 0 && c >= 0 && r < rows && c < cols;
    }

    int rows = 0;
    int cols = 0;

    vector<vector<int>> parse_data() {

        string s;

        getline(cin, s);
        cols = s.size();
        vector<vector<int>> vec(0, vector<int>(cols));
        rows = 0;
        do {
            vector<int> v;
            v.reserve(cols);
            for (const char c: s) {
                v.push_back(c - '0');
            }
            vec.push_back(std::move(v));
            rows++;
        } while (getline(cin, s));

        return vec;
    }

    int run_task(vector<vector<int>> &vec) {

        ll tot_cnt = 0;

        for (int step = 0; step < 1000; ++step) {
            ll cnt = 0;
            vector<vector<int>> new_vec(rows, vector<int>(cols));
            queue<pair<int, int>> flashing;
            vector<vector<bool>> has_flashed(rows, vector<bool>(cols, false));
            for (int r = 0; r < rows; ++r) {
                for (int c = 0; c < cols; ++c) {
                    int new_val = 1 + vec[r][c];
                    new_vec[r][c] = new_val;
                    if (new_val == 10) {
                        // keep track of these flashing
                        flashing.emplace(r, c);
                        new_vec[r][c] = 0; // reset
                        has_flashed[r][c] = true;
                        cnt++;
                    }
                }
            }

            while (!flashing.empty()) {
                pair<int, int> f = flashing.front();
                flashing.pop();
                has_flashed[f.first][f.second] = true;

                for (const Move &m: moves) {
                    int x = f.first + m.x;
                    int y = f.second + m.y;
                    if (is_valid(x, y, rows, cols) && !has_flashed[x][y]) {
                        if (++new_vec[x][y] >= 10) {
                            new_vec[x][y] = 0;
                            flashing.emplace(x, y);
                            cnt++;
                            has_flashed[x][y] = true; // this octopus hasn't flashed yet, but we know that it is enqueued for flashing
                            // so we can mark it to avoid updates from neighbors
                        }
                    }
                }
            }

            tot_cnt += cnt;

            vec = std::move(new_vec);

            if (cnt == cols * rows) {
                cout << "PART 2 SOL=" << step + 1 << endl;;
                break;
            }

        }
        return 0;
    }
};
