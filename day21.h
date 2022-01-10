//
// Created by guillaume_guy on 12/21/21.
//

#ifndef AOC2021_DAY21_H
#define AOC2021_DAY21_H

#endif //AOC2021_DAY21_H

#include "vector"
#include "iostream"

using namespace std;

constexpr bool VERBOSE = false;

typedef unsigned long long ull;

typedef pair<ull, ull> puu;

puu zeroes = {0, 0};

// PART 2: Frequency outcomes for the ~27 combinations possible (3*3*3)
vector<int> occurrences = {1, 3, 6, 7, 6, 3, 1};
vector<int> scores = {3, 4, 5, 6, 7, 8, 9};

struct Dice {
    int pos = 1;
    int faces = 100;
    int rolls = 0;

    Dice(int faces_) {
        faces = faces_;
    }

    int Roll() {
        rolls++;

        int sum = 0;
        if (pos + 3 <= faces) {
            sum = 3 * (pos + 1);
            if (VERBOSE) cout << pos << " " << pos + 1 << " " << pos + 2 << endl;
            pos += 3;
        } else {
            for (int i = 0; i < 3; ++i) {
                if (pos == faces + 1) {
                    pos = 1;
                }
                if (VERBOSE) cout << pos << " ";
                sum += pos;
                pos++;
            }
            if (pos == faces + 1) {
                pos = 1;
            }
            if (VERBOSE) cout << endl;
        }
        return sum;
    }


};

int position(int i) {
    if (i <= 10) {
        return i;
    }
    return i % 10;
}


int task1() {

    auto d = Dice(100);
    /*
     * for (int i = 0; i < 200; ++i) {
        int sum = d.Roll();
        what_is2(i, sum);
    }*/

    // 10,3
    int player1 = 4;
    int player2 = 8;

    player1 = 10;
    player2 = 3;

    int p1_score = 0;
    int p2_score = 0;
    while (true) {
        int sum1 = d.Roll();
        player1 = position(player1 + sum1);
        p1_score += player1;
        if (p1_score >= 1000) {
            cout << "player 1 won at round " << 3 * d.rolls << endl;
            cout << " sol1 =" << p2_score * 3 * d.rolls << endl;
            break;
        }

        int sum2 = d.Roll();
        player2 = position(player2 + sum2);
        p2_score += player2;

        if (p2_score >= 1000) {
            cout << "player 2 won at round " << 3 * d.rolls << endl;
            cout << " sol1 =" << p1_score * 3 * d.rolls << endl;
            break;
        }
    }
    return 0;
}


bool equal(puu &a, puu &b) {
    return a.first == b.first && a.second == b.second;
}

puu play(int p1, int p2, int s1, int s2, bool turn, vector<vector<vector<vector<vector<puu>>>>> &dp) {
    // turn means player 2 goes

    puu sol = {0ULL, 0ULL};

    if (s1 >= 21) {
        return {1ULL, 0};
    } else if (s2 >= 21) {
        return {0, 1ULL};
    }

    if (!equal(dp[s1][s2][p1][p2][turn], zeroes)) {
        return dp[s1][s2][p1][p2][turn];
    }


    if (!turn) {
        // turn = false ==> player 1

        // player 1
        for (int i = 0; i <= occurrences.size(); ++i) {
            int p1_updated = position(p1 + scores[i]);
            int s1_updated = s1 + p1_updated;
            puu t1 = play(p1_updated, p2, s1_updated, s2, !turn, dp);
            sol.first += occurrences[i] * t1.first;
            sol.second += occurrences[i] * t1.second;
        }

    } else {

        // player 2
        for (int i = 0; i <= occurrences.size(); ++i) {
            int p2_updated = position(p2 + scores[i]);
            int s2_updated = s2 + p2_updated;
            puu t1 = play(p1, p2_updated, s1, s2_updated, !turn, dp);
            sol.first += occurrences[i] * t1.first;
            sol.second += occurrences[i] * t1.second;
        }
    }
    dp[s1][s2][p1][p2][turn] = sol;
    return sol;
}


int task2() {

    // Position: 1..10
    // Score: 1...21
    // State S1,S2,P1,P2,PT
    // this will contain the outcomes of the game based on the respective state (position, score, and turn)
    vector<vector<vector<vector<vector<puu>>>>> dp;
    dp.reserve(22);

    for (int s1 = 21; s1 >= 0; --s1) { // score 1
        dp[s1].resize(22); // s2
        for (int s2 = 21; s2 >= 0; --s2) { // score 2
            dp[s1][s2].resize(11); // s2
            for (int p1 = 10; p1 >= 0; --p1) {
                dp[s1][s2][p1].resize(11); // p2
                for (int p2 = 10; p2 >= 0; --p2) {
                    dp[s1][s2][p1][p2].resize(2); // turn
                    for (int t: {0, 1}) {
                        dp[s1][s2][p1][p2][t] = {0ULL, 0ULL};
                    }
                }
            }
        }
    }

    int player1 = 4;
    int player2 = 8;

    player1 = 10;
    player2 = 3;

    puu sol = play(player1, player2, 0, 0, false, dp);

    cout << sol.first << " " << sol.second << endl;

}