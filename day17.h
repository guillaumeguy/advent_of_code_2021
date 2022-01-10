//
// Created by guillaume_guy on 12/14/21.

// Not super happy of the way I solved this. I wish I thought about the equation more
//

#ifndef AOC2021_DAY17_H
#define AOC2021_DAY17_H

#endif //AOC2021_DAY17_H

#include "vector"
#include "iostream"
#include "macro.h"
#include "array"
#include "queue"

using namespace std;

struct Box {
    int xmin;
    int xmax;
    int ymin;
    int ymax;

    bool is_in(int x, int y) {
        return x >= xmin && x <= xmax && y >= ymin && y <= ymax;
    }

};

//Box b = {20,30,-10,-5};
Box b = {156, 202, -110, -69};

int compute_max_altitude(int x, int y, Box &b) {
    /*
     * Max alt is achieved at x = y0;
     * Substituting, that gives max_alt = y0 * (y0+1) /2
     */
    int initial_y = y;
    int current_x = 0;
    int current_y = 0;
    bool reached = false;

    while (current_x < b.xmax && current_y > b.ymin) {
        current_x += x;
        current_y += y;
        if (b.is_in(current_x, current_y)) {
            reached = true;
        }

        x = x > 0 ? x - 1 : 0;
        y--;

        if (x == 0 && current_x < b.xmin) {
            // no chance of making any progress
            return -1;
        }
    }

    int max_alt = initial_y * (initial_y + 1) / 2;

    return (reached ? max_alt : -1);

}


int task1() {

    /*
      * Note: Only y follows a polynomial function of order 2
      * One can write: 2*ymax >= n + (2*yo+1) * n + 1 >= ymin*2 where n is the number of steps
      * One can then identify an interval for `n` to fulfill these 2 equations (no root)
      *
      *
      * At any rate ... a loop is easier :)
 */
    // how many x steps?
    int sum_x = 0;
    int min_x0 = INT_MAX; // minimum x launch
    int max_x = 0; // max x launch
    int i = 0;
    while (sum_x < b.xmax) {
        if (sum_x >= b.xmin) {
            min_x0 = min(i, min_x0);
            max_x = max(i, min_x0);
            cout << i << " " << sum_x << endl;
        }
        sum_x += ++i;
    }

    what_is2(min_x0, max_x);

    int record = 0;

    for (int y0 = 0; y0 < 100; ++y0) {
        for (int x0 = min_x0; x0 <= max_x; ++x0) {
            int max_alt = compute_max_altitude(x0, y0, b);
            what_is3(x0, y0, max_alt)
            if (max_alt > record) {
                record = max_alt;
            }
        }
    }

    cout << "sol1=" << record << endl;


}


int task2() {

    int sum_x = 0;
    int min_x0 = INT_MAX; // minimum x launch
    int max_x0 = 0; // max x launch
    int i = 0;
    while (sum_x < b.xmax) {
        if (sum_x >= b.xmin) {
            min_x0 = min(i, min_x0);
            max_x0 = max(i, min_x0);
            cout << i << " " << sum_x << endl;
        }
        sum_x += ++i;
    }

    what_is2(min_x0, max_x0);

    // compute the max number of steps:


    int cnt = 0;

    /*
     * range for x:
     *      min_x (otherwise, it will never reach b.xmin)
     *      b.max_x (that is, get there in 1 shot)
     *
     *  range for y:
     *      b.ymin (get there in 1 shot)
     *      -b.ymin
     *
     * ymax >= (y-0) + (y-1) + (y-2) + ... (y-N+1) >= ymin
     *  The middle becomes: n*y + n*(n-1)/2
     */

    for (int x0 = min_x0; x0 <= b.xmax; ++x0) {
        for (int y0 = b.ymin; y0 <= max(abs(b.ymin), abs(b.ymax)); ++y0) {
            int max_alt = compute_max_altitude(x0, y0, b);
            if (max_alt != -1) {
                cnt++;
            }
        }
    }

    cout << cnt << endl;

    cout << "sol2=" << cnt << endl;

}
