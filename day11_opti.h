//
// Created by guillaume_guy on 12/11/21.
// Optimized version for speed
//

#ifndef AOC2021_DAY11_H
#define AOC2021_DAY11_H

#endif //AOC2021_DAY11_H


#include "vector"
#include "array"
#include "string"
#include "iostream"
#include "numeric"
#include "queue"

using namespace std;

typedef long long ll;

static constexpr int N = 16; // max_col and max_row
static constexpr int M = 8; // numbers of moves


constexpr array<int, M> generate_move(int n) {
    assert(__builtin_popcount(n) == 1); // n needs to be a power of two
    array<int, M> sol{};
    int i = 0;
    for (int r = -1; r <= 1; ++r) {
        for (int c = -1; c <= 1; ++c) {
            if (!(r == 0 && c == 0))
                sol[i++] = r * n + c;
        }
    }
    return sol;
}

static constexpr array<int, M> moves = generate_move(N);

struct Task {
    int rows = 0; // will be overwritten when parse_data() is called
    int cols = 0;  // will be overwritten when parse_data() is called

    static constexpr int e = 4; // exponent

    // used for modulo.
    // Creates a bitmask with 1 in the first `e-1` bits.  modulo(index,N) = mask & index
    static constexpr int mask = (1 << e) - 1;
    static constexpr int ARRAY_SIZE = N * N;

    static_assert(N == (1 << e));

    array<int, ARRAY_SIZE> new_vec{};
    array<bool, ARRAY_SIZE> has_flashed{};

    inline int col(int index) {
        // e = exponent
        // This is used to return the column value given an index
        return index & mask;
    }

    inline int row(int index) {
        // e = exponent
        return index >> e;
    }

    inline bool is_valid(int index, int rows, int cols) {
        int r = row(index);
        int c = col(index);
        return r >= 0 && c >= 0 && r < rows && c < cols;
    }

    inline int index(int r, int c) {
        return (r << e) + c;
    }

    array<int, ARRAY_SIZE> parse_data() {
        string s;

        getline(cin, s);
        cols = (int) s.size();
        array<int, ARRAY_SIZE> vec{};

        assert(cols <= N);

        do {
            int col = 0;
            assert(rows <= N);
            for (const char c: s) {
                vec[index(rows, col)] = (c - '0');
                col++;
            }
            rows++;
        } while (getline(cin, s));


        return vec;
    }


    int run_task(array<int, ARRAY_SIZE> &vec) {

        assert(rows != 0 && cols != 0);

        ll tot_cnt = 0;

        for (int step = 0; step < 1000; ++step) {
            ll cnt = 0;
            queue<int> flashing;
            has_flashed.fill(false); // overwrite past steps
            for (int r = 0; r < rows; ++r) { // rows
                const int start_ind = index(r, 0);
                const int end_ind = start_ind + cols;
                for (int ind = start_ind; ind < end_ind; ++ind) { // columns
                    int new_val = 1 + vec[ind];
                    new_vec[ind] = new_val;
                    if (new_val == 10) {
                        // keep track of these flashing
                        flashing.emplace(ind);
                        new_vec[ind] = 0; // reset
                        has_flashed[ind] = true;
                        cnt++;
                    }
                }
            }

            while (!flashing.empty()) {
                int f = flashing.front();
                flashing.pop();
                has_flashed[f] = true;

                for (const int &move: moves) {
                    int ind = f + move;
                    if (is_valid(ind, rows, cols) && !has_flashed[ind]) {
                        if (++new_vec[ind] >= 10) {
                            new_vec[ind] = 0;
                            flashing.emplace(ind);
                            cnt++;
                            has_flashed[ind] = true; // this octopus hasn't flashed yet, but we know that it is enqueued for flashing
                            // so we can mark it to avoid updates from neighbors
                        }
                    }
                }
            }

            tot_cnt += cnt;

            swap(vec, new_vec);

            if (cnt == cols * rows) {
                cout << "PART 2 SOL=" << step + 1 << endl;
                break;
            }

        }
        return 0;
    }

};