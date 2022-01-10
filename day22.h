//
// Created by guillaume_guy on 12/22/21.
//

#ifndef AOC2021_DAY22_H
#define AOC2021_DAY22_H

#endif //AOC2021_DAY22_H

#include "vector"
#include "iostream"
#include "macro.h"
#include "array"
#include "omp.h"
#include <algorithm>


using namespace std;

constexpr bool VERBOSE = false;

struct Rule {
    bool on;
    std::array<pii, 3> coords;


    Rule() {
    }

    Rule(bool on_, pii a, pii b, pii c) {
        on = on_;
        coords = {a, b, c};
    }

};

pii parse_dim(string &s) {
    auto x = split(s, "..");
    auto x1 = stoi(split(x[0], "=")[1]);
    auto x2 = stoi(x[1]);
    return {x1, x2};
}


vector<Rule> parse() {
    string s;
    vector<Rule> rules;
    while (getline(cin, s)) {
        //"on x=10..12,y=10..12,z=10..12"


        auto dims = split(s.substr(3, string::npos), ",");
        Rule r = {s[2] == ' ',
                  parse_dim(dims[0]),
                  parse_dim(dims[1]), parse_dim(dims[2])
        };
        rules.push_back(r);

    }
    return rules;
}

int task1() {

    auto rules = parse();

    int n = 101;
    vector<vector<vector<bool>>> v(n, vector<vector<bool >>(n, vector<bool>(n, false)));

    for (const auto &r: rules) {
        cout << "doing rule" << r.coords[0] << r.coords[1] << r.coords[2] << r.on << endl;
        for (int x = max(-50, r.coords[0].first); x <= min(50, r.coords[0].second); ++x) {
            for (int y = max(-50, r.coords[1].first); y <= min(50, r.coords[1].second); ++y) {
                for (int z = max(-50, r.coords[2].first); z <= min(50, r.coords[2].second); ++z) {
                    v[x + 50][y + 50][z + 50] = r.on;
                }
            }
        }
    }

    int cnt = 0;
    for (int x = -50; x <= 50; ++x) {
        for (int y = -50; y <= 50; ++y) {
            for (int z = -50; z <= 50; ++z) {
                cnt += v[x + 50][y + 50][z + 50];
            }
        }
    }

    cout << cnt << endl;

    return 0;
}

int task2() {

    vector<Rule> v = parse();

    vector<ll> X, Y, Z;


    for (const auto &r: v) {
        X.push_back(r.coords[0].first);
        X.push_back(r.coords[0].second + 1); // we need to increment by 1 so that we have an exclusive uppoer bound

        Y.push_back(r.coords[1].first);
        Y.push_back(r.coords[1].second + 1);

        Z.push_back(r.coords[2].first);
        Z.push_back(r.coords[2].second + 1);
    }

    int N = Z.size();
    cout << "N=" << N << endl;

    vector<vector<vector<bool>>> grids(N, vector<vector<bool>>(N, vector<bool>(N, false)));

    sort(X.begin(), X.end());
    sort(Y.begin(), Y.end());
    sort(Z.begin(), Z.end());

    auto index = [](vector<ll> &ve, ll e) -> int {
        return int(lower_bound(ve.begin(), ve.end(), e) - ve.begin());
    };


    for (const auto &rule: v) {
        int x0 = index(X, rule.coords[0].first);
        int x1 = index(X, rule.coords[0].second + 1);
        int y0 = index(Y, rule.coords[1].first);
        int y1 = index(Y, rule.coords[1].second + 1);
        int z0 = index(Z, rule.coords[2].first);
        int z1 = index(Z, rule.coords[2].second + 1);

        //what_is6(x0, x1, y0, y1, z0, z1)
        #pragma omp parallel
        for (int x = x0; x < x1; ++x) {
            for (int y = y0; y < y1; ++y) {
                for (int z = z0; z < z1; ++z) {
                    grids[x][y][z] = rule.on;
                }
            }
        }
    }


    int num_threads = omp_get_max_threads();
    std::cout << "thread cnt" << num_threads << std::endl;

    ll global_cnt = 0LL;
    #pragma omp parallel for num_threads(num_threads) reduction(+:global_cnt)
    for (int x = 0; x < N - 1; ++x) {
        for (int y = 0; y < N - 1; ++y) {
            for (int z = 0; z < N - 1; ++z) {
                global_cnt += grids[x][y][z] * (ll) (X[x + 1] - X[x]) * (Y[y + 1] - Y[y]) * (ll) (Z[z + 1] - Z[z]);
            }
        }
    }

    cout << global_cnt << endl;

    return 0;
}