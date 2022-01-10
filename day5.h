//
// Created by guillaume_guy on 12/5/21.
//

#ifndef AOC2021_DAY5_H
#define AOC2021_DAY5_H

#endif //AOC2021_DAY5_H

#include "vector"
#include <iostream>
#include "math.h"

using namespace std;

constexpr bool VERBOSE = false;

struct Point {
    int x, y;

    friend bool operator==(const Point &lhs, const Point &rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

struct Line {
    Line(Point _P1, Point _P2) : P1(_P1), P2(_P2), max_y(max(_P1.y, _P2.y)), max_x(max(_P1.x, _P2.x)) {};
    Point P1;
    Point P2;
    int max_y;
    int max_x;

    bool vertical() {
        return P1.x == P2.x;
    }

    bool horizontal() {
        return P1.y == P2.y;
    }

    friend ostream &operator<<(ostream &os, const Line &line) {
        os << "(" << line.P1.x << " " << line.P1.y << ") -" "(" << line.P2.x << " " << line.P2.y
           << ")" << endl;
        return os;
    }
};


template<typename T>
typename std::enable_if<std::is_signed<T>::value, int>::type
inline constexpr signum(T const x) {
    return (T(0) < x) - (x < T(0));
}


int task(bool part_1) {

    // Parse first line
    std::string s;

    std::vector<Line> lines = {};

    int xmax = 0;
    int ymax = 0;

    while (std::getline(cin, s)) {
        cout << s << endl;
        auto pos1 = s.find(',');
        auto pos2 = s.find(' ');
        auto pos3 = s.find('>');
        auto pos4 = s.find(',', pos3);

        int x1 = stoi(s.substr(0, pos1), nullptr, 10);
        int y1 = stoi(s.substr(pos1 + 1, pos2), nullptr, 10);
        int x2 = stoi(s.substr(pos3 + 2, pos4), nullptr, 10);
        int y2 = stoi(s.substr(pos4 + 1, std::string::npos), nullptr, 10);

        ymax = max({y1, ymax, y2});
        xmax = max({x1, xmax, x2});

        Line line = {{x1, y1},
                     {x2, y2}};

        lines.push_back(line);
        cout << line << endl;
    }

    cout << xmax << endl;
    cout << ymax << endl;

    int cnt = 0;
    vector<vector<int>> covered(xmax + 1, vector<int>(ymax + 1, 0));

    bool PART_1 = true;
    if (PART_1) {
        // remove diagonal lines
        auto it = remove_if(lines.begin(), lines.end(), [](Line &l) { return !l.horizontal() && !l.vertical(); });
        lines.erase(it, lines.end());
    }

    for (const auto &l: lines) {
        int delta_y = (l.P2.y) - (l.P1.y);
        int delta_x = (l.P2.x) - (l.P1.x);
        int dy = signum(delta_y);
        int dx = signum(delta_x);
        cout << dx << " " << dy << endl;
        int x = l.P1.x;
        int y = l.P1.y;
        while (!(x == l.P2.x && y == l.P2.y)) {
            if (VERBOSE) cout << l << " " << x << " " << y << endl;
            if (covered[x][y]++ == 1) {
                cnt++;
            }
            x += dx;
            y += dy;
        }
        // last point (P2)
        if (covered[x][y]++ == 1) {
            cnt++;
        }
    }

    cout << "answer=" << cnt << endl;
    // part1= 4421
    // part2= 18674

    return 0;

}

int tasks() {
    bool part_1 = true;
    return task(part_1);
}