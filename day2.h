//
// Created by guillaume_guy on 12/2/21.
//

#ifndef AOC2021_DAY2_H
#define AOC2021_DAY2_H

#endif //AOC2021_DAY2_H


#include <array>
#include "unordered_map"

using namespace std;

enum class mov {
    forward = 0,
    backward = 1,
    up = 2,
    down = 3
};

void move(array<int, 2> &pos, mov m, int steps) {
    static constexpr array<std::pair<int, int>, 4> map = {
            {{1, 0},
             {-1, 0}, // not used today
             {0, -1},
             {0, 1}
            }
    };

    auto &add = map[static_cast<int>(m)];
    pos[0] += add.first * steps;
    pos[1] += add.second * steps;

    return;
}


void move2(array<int, 3> &pos, mov m, int steps) {
    static constexpr array<array<int, 3>, 4> map = {
            {{1, 0, 0},
             {-1, 0, 0}, // not used today
             {0, 0, -1},
             {0, 0, 1}
            }
    };

    auto &add = map[static_cast<int>(m)];
    pos[0] += add[0] * steps;
    pos[1] += m == mov::forward ? steps * pos[2] : 0;
    pos[2] += add[2] * steps;

    return;
}


unordered_map<string, mov> lookup = {
        {"forward", mov::forward},
        {"backward", mov::backward},
        {"up", mov::up},
        {"down", mov::down},
};


int task1() {
    array<int, 2> pos = {0, 0};

    string s;

    while (std::getline(std::cin, s)) {
        std::size_t ind = s.find(' ');
        if (ind == std::string::npos) {
            break;
        }
        mov direction = lookup[s.substr(0, ind)];
        int steps = stoi(s.substr(ind + 1, std::string::npos));
        move(pos, direction, steps);
    }

    cout << pos[0] << " " << pos[1] << " " << pos[0] * pos[1] << endl;

}

int task2() {
    array<int, 3> pos = {0, 0, 0};

    string s;

    while (std::getline(std::cin, s)) {
        std::size_t ind = s.find(' ');
        if (ind == std::string::npos) {
            break;
        }
        mov direction = lookup.at(s.substr(0, ind));
        int steps = stoi(s.substr(ind + 1, std::string::npos));
        move2(pos, direction, steps);
    }

    cout << pos[0] << " " << pos[1] << " " << pos[0] * pos[1] << endl;

}