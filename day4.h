//
// Created by guillaume_guy on 12/4/21.
//

#ifndef AOC2021_DAY4_H
#define AOC2021_DAY4_H

#endif //AOC2021_DAY4_H

#include <array>
#include "vector"
#include <unordered_map>
#include "unordered_set"

using namespace std;

constexpr int N = 5;

typedef array<array<int, N>, N> Grid;
typedef int grid_id_t;
typedef vector<int> vi;


struct Location{
    grid_id_t grid;
    int row;
    int col;
};

void print_grid(Grid &g) {
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            cout << g[r][c] << " ";
        }
        cout << endl;
    }
}

int compute_score(Grid &g, unordered_set<int> &visited) {
    int cnt = 0;
    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (visited.count(g[r][c]) == 0) {
                cnt += g[r][c];
            }
        }
    }
    return cnt;
}

std::pair<vector<Grid>, vi> parse_input() {

    // Parse first line
    std::string s;
    std::cin >> s;

    vi numbers(0);

    auto pos = s.find(',');
    while (pos != std::string::npos) {
        numbers.push_back(stoi(s.substr(0, pos), nullptr, 10));
        s.erase(0, pos + 1);
        pos = s.find(',');
    }

    // Parse grids
    vector<Grid> grids;
    while (cin) {
        Grid grid;
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                cin >> grid[r][c];
            }
        }
        grids.push_back(grid);
    }

    return {grids,numbers};
}

int tasks() {
    /*
     * The idea here is to check for the grid AND the row AND the column of any number drawn through a Map
     * Time complexity is linear:  O(N numbers * 5 elements * 2 (row/col)) ~ O(N)
     *
     * Without this lookup, it would be O(N number * grid_cnt * 5 row * 2 (row/col) * 5 elements)
     * The speed-up is therefore: grid_cnt * 5
     */

    auto input = parse_input();
    auto grids = input.first;
    auto numbers = input.second;

    vector<bool> won(grids.size(),false);
    int won_cnt = 0;

    /* This map back-references the (grid,row,col) for a given drawn number for fast access
     * Key:
     *      number drawn (int)
     * Value: loc (Location)
     */
    unordered_map<int, vector<Location>> cnt_lookup;

    for (int grid_id = 0; grid_id < grids.size(); ++grid_id) {
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                cnt_lookup[grids[grid_id][r][c]].push_back({grid_id, r, c});
            }
        }
    }

    unordered_set<int> visited;
    for (const auto &bingo_num: numbers) {
        if (cnt_lookup.count(bingo_num) == 0) {
            continue; // no match
        }
        for (const auto &match: cnt_lookup[bingo_num]) {

            visited.insert(bingo_num); // visited
            auto grid_id = match.grid;
            int r = match.row;
            int c = match.col;

            if(won[grid_id]){
                // this grid was already won. Let's move on
                continue;
            }

            // Is the row filled?
            bool row_filled = true;
            for (int c1 = 0; c1 < N; ++c1) {
                if (visited.count(grids[grid_id][r][c1]) == 0) {
                    row_filled = false;
                    break;
                }
            }

            // Is the col filled?
            bool col_filled = true;
            for (int r1 = 0; r1 < N; ++r1) {
                if (visited.count(grids[grid_id][r1][c]) == 0) {
                    col_filled = false;
                    break;
                }
            }

            if (row_filled || col_filled) {
                if(won_cnt == 0)
                    cout << "sol 1=" << compute_score(grids[grid_id], visited) << " " << bingo_num << endl;

                won[grid_id] = true;
                won_cnt++;

                // all grids won
                if(won_cnt == grids.size()){
                    cout << "sol 2=" << compute_score(grids[grid_id], visited) << " " << bingo_num << endl;
                }
            }
        }
    }

    return 0;

}
