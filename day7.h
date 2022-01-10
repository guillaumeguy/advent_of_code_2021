//
// Created by guillaume_guy on 12/7/21.
//

#ifndef AOC2021_DAY7_H
#define AOC2021_DAY7_H

#endif //AOC2021_DAY7_H

#include "vector"
#include "string"
#include "iostream"
#include "unordered_map"

using namespace std;

std::vector<std::string> split(std::string to_split, std::string delimiter) {
    size_t pos = 0;
    std::vector<std::string> matches{};
    do {
        pos = to_split.find(delimiter);
        int change_end;
        if (pos == std::string::npos) {
            pos = to_split.length() - 1;
            change_end = 1;
        } else {
            change_end = 0;
        }
        matches.push_back(to_split.substr(0, pos + change_end));

        to_split.erase(0, pos + delimiter.length());

    } while (!to_split.empty());
    return matches;

}

int compute_fuel2(vector<int> &v, int pos) {
    long long fuel = 0;
    for (const int &crab: v) {
        long long m = abs(crab - pos); // we are looking for sum(1...m)
        fuel += ((1 + m) * m) / 2;
    }
    return fuel;
}

int compute_fuel(vector<int> &v, int pos) {
    long long fuel = 0;
    for (const int &crab: v) {
        fuel += (long long) abs(crab - pos);
    }
    return fuel;
}


int task1() {

    string s;
    getline(cin, s);
    vector<string> input_str = split(s, ",");
    int N = input_str.size();
    vector<int> input(N);
    long long cnt = 0;
    for (int i = 0; i < N; ++i) {
        input[i] = stoi(input_str[i], nullptr, 10);
        cnt += input[i];
    }

    /*
     * The solution to Part 1 is the median
     * A way to intuit it:
     * Imagine 2 points
     * A ---------- B
     * Any chosen point in [A,B] returns the same fuel consumption
     * However, anything outside [A,B] will increase fuel consumption.
     * So you need as many points on one side as the other.
     * i.e. median
     */
    sort(input.begin(), input.end());

    cout << "SOL 1=" << compute_fuel(input, input[N / 2]) << endl;

    /*
     * Solution 2:
     * It's the average!
     * Give or take. This is not exactly a L2 norm but quite similar
     * https://stats.stackexchange.com/questions/34613/l1-regression-estimates-median-whereas-l2-regression-estimates-mean
     */
    cout << "SOL 2=" << (double) cnt / (double) N << " " << compute_fuel2(input, (double) cnt / (double) N) << endl;


}

