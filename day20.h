//
// Created by guillaume_guy on 12/21/21.
//

#ifndef AOC2021_DAY20_H
#define AOC2021_DAY20_H

#endif //AOC2021_DAY20_H

#include "vector"
#include "iostream"
#include "macro.h"
#include "array"
#include "queue"

using namespace std;


typedef vector<vector<bool>> Matrix;

string lookup;


ll sum(Matrix &m) {
    ll cnt = 0;
    for (const auto &rows: m) {
        for (const auto &e: rows) {
            cnt += e;
        }
    }
    return cnt;
}

bool transform(Matrix &m, int r, int c,bool outside_pix) {
    int sol = 0;
    for (int dr = -1; dr <= 1; ++dr) {
        int nr = r + dr;
        for (int dc = -1; dc <= 1; ++dc) {
            int nc = c + dc;
            sol <<= 1;
            if (nr >= 0 && nr < m.size() && nc >= 0 && nc < m[0].size()) {
                sol += m[nr][nc];
            }else{
                sol += outside_pix;
            }
        }
    }
    //what_is3(r,c,sol);

    return lookup[sol] == '#';
}

int task1() {

    getline(cin, lookup);

    string s;
    Matrix m;
    int n;

    while (getline(cin, s)) {
        if (!s.empty()) {
            n = s.size();
            vector<bool> v(n);
            for (int i = 0; i < n; ++i)
                v[i] = s[i] == '#';
            m.push_back(std::move(v));
        }
    }
    print_vec(m[0]);
    print_vec(m[m.size()-1]);

    int ncols = n;
    int nrows = m.size();
    what_is2(nrows,ncols)

    bool outside_pix = false;

    what_is1(transform(m, 2, 2,outside_pix));

    cout << sum(m) << endl;

    what_is1(outside_pix)

    for (int step = 1; step <= 50; ++step) {
        ncols += 4;
        nrows += 4;
        what_is3(step,nrows, ncols);
        Matrix nm(nrows, vector<bool>(ncols));
        for (int r = 0; r < nrows; ++r) {
            for (int c = 0; c < ncols; ++c) {
                nm[r][c] = transform(m, r - 2, c - 2,outside_pix);
            }
        }

        // recompute outside pixel
        outside_pix = (outside_pix ? lookup[(1 << 9)-1] : lookup[0]) == '#'; //
        what_is1(outside_pix)
        if(step==2)
            cout << " sol1=" << sum(nm) << endl;

        swap(m, nm);
    }

    cout << "sol2=" << sum(m) << endl;
}