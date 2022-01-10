//
// Created by guillaume_guy on 12/11/21.
//

#ifndef AOC2021_DAY13_H
#define AOC2021_DAY13_H

#endif //AOC2021_DAY13_H

#include "vector"
#include "string"
#include "iostream"
#include "unordered_map"
#include "unordered_set"
#include "macro.h"
#include "numeric"
using namespace std;

typedef vector<vector<bool>> Matrix;

struct Point {
    int x, y;

    friend std::ostream &operator<<(std::ostream &os, const Point &p) {
        os << p.x << " " << p.y << endl;
        return os;
    }
};

Matrix foldY(Matrix &m, int v) {
    // fold along y = v
    Matrix m1(m.size(), vector<bool>(v));

    for (int r = 0; r < m1.size(); ++r) {
        for (int c = 0; c < m1[0].size(); ++c) {
            // do not overrun the vector
            int c1 = 2 * v - c; // symmetrical index
            bool symetric_val = c1 < m[0].size() && c1 >= 0 && m.at(r).at(c1);
            m1.at(r).at(c) = m.at(r).at(c) || symetric_val;

        }
    }
    return m1;
}


Matrix foldX(Matrix &m, int v) {
    // fold along x = v
    int cols = m[0].size();
    Matrix m1(v, vector<bool>(cols));

    for (int r = 0; r < m1.size(); ++r) {
        for (int c = 0; c < m1[0].size(); ++c) {
            int r1 = 2 * v - r; // symmetrical index
            bool symmetrical_val = r1 < m.size() && r1 >= 0 && m.at(r1).at(c);
            m1.at(r).at(c) = m.at(r).at(c) || symmetrical_val;
        }
    }
    return m1;
}


int task1() {
    string s;
    vector<Point> points;
    vector<pii > instructions;

    int xmax = 0, ymax = 0;

    bool instruction = false;
    while (getline(cin, s)) {

        // Separate instructions from points
        if (instruction || s.empty()) {
            if (s.empty()) { // skip empty line
                getline(cin, s);
                instruction = true;
            }

            auto pos = s.find('=');
            pii instruction = {
                    s[pos - 1] == 'y',
                    stoi(s.substr(pos + 1, string::npos), nullptr, 10)
            };

            cout << instruction << endl;

            instructions.push_back(std::move(instruction));


        } else {

            auto pos = s.find(',');
            Point p = {
                    stoi(s.substr(0, pos), nullptr, 10),
                    stoi(s.substr(pos + 1, string::npos), nullptr, 10)
            };

            xmax = max(xmax, p.x);
            ymax = max(ymax, p.y);

            cout << p << endl;

            points.push_back(p);
        }
    }


    Matrix m(xmax + 1, vector<bool>(ymax + 1, false));
    for (const auto &p: points) {
        m[p.x][p.y] = true;
    }

    int i = 0;

    cout << "dim" << m.size() << "-" << m[0].size() << endl;

    for (const auto &inst: instructions) {
        if (inst.first == 1) {
            // y-trans
            m = foldY(m, inst.second);
        } else {
            m = foldX(m, inst.second);
        }

        long long squares = 0;

        cout << "step=" << ++i << endl;
        for (auto &row: m) {
            squares += accumulate(row.begin(),row.end(),0LL);
        }
        cout << "dim" << m.size() << "-" << m[0].size() << endl;
        cout << "squares=" << squares << endl;
    }

    for (auto &row: m) {
        print_vec(row);
    }

};