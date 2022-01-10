//
// Created by guillaume_guy on 12/1/21.
//

#ifndef AOC2021_DAY1_H
#define AOC2021_DAY1_H

#endif //AOC2021_DAY1_H

#include <array>

using namespace std;

int task1() {
    int last;
    cin >> last;
    int cnt = 0;
    int num;
    while (cin >> num) {
        if (num > last) cnt++;
        last = num;
    }
    cout << cnt << endl;

    return 0;
}


/*
 * This approach uses a circular buffer with fixed size instead of a queue to store the past 3 elements
 * The main benefit is speed and simplicity (no queue abstraction)
 * The main drawback is that std::queue has flexible size whereas this buffer does not (the size needs to be known at compile time)
 *
 */

int task2() {
    constexpr int QUEUE_SIZE = 3;

    array<int,QUEUE_SIZE> v{};
    int num;
    for (int i = 0; i < QUEUE_SIZE; ++i) {
        cin >> v[i];
    }

    int pointer = 0;
    int cnt = 0;
    while (cin >> num) {
        if (num > v[pointer])
            cnt++;
        v[pointer] = num;
        pointer = (pointer + 1) % QUEUE_SIZE; // if reaching the end of the array, restart
    }
    cout << cnt << endl;

    return 0;
}

