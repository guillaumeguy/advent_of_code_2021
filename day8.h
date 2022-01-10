//
// Created by guillaume_guy on 12/8/21.
//

#ifndef AOC2021_DAY8_H
#define AOC2021_DAY8_H

#endif //AOC2021_DAY8_H


#include <array>
#include "vector"
#include "string"
#include "iostream"
#include "unordered_map"
#include "unordered_set"

using namespace std;


/////////////////////////////////////////////
///// DATA NEEDED TO SET UP THE PROBLEM /////
/////////////////////////////////////////////

// 0 : a b c e f g
vector<vector<char>> segments = {
        {'a', 'b', 'c', 'e', 'f', 'g'}, // 0
        {'c', 'f'}, // 1
        {'a', 'c', 'd', 'e', 'g'}, // 2
        {'a', 'c', 'd', 'f', 'g'}, // 3
        {'b', 'c', 'd', 'f'}, // 4
        {'a', 'b', 'd', 'f', 'g'}, //5
        {'a', 'b', 'd', 'e', 'f', 'g'}, // 6
        {'a', 'c', 'f'}, // 7
        {'a', 'b', 'c', 'd', 'e', 'f', 'g'}, // 8
        {'a', 'b', 'c', 'd', 'f', 'g'}, // 9
};

unordered_set<char> all_sol = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};


constexpr bool VERBOSE = false;

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


unordered_set<char> intersect(unordered_set<char> &a, unordered_set<char> &b) {
    /*
     * Intersection of two sets
     */
    unordered_set<char> inter;
    for (const auto &c: a) {
        if (b.count(c) == 1) {
            inter.insert(c);
        }
    }
    return inter;
}

string to_string(unordered_set<char> a) {
    string s;
    for (const auto &item: a) {
        s.push_back(item);
        s.push_back(' ');
    }
    return s;
}

string to_string(vector<char> a) {
    string s;
    for (const auto &item: a) {
        s.push_back(item);
        s.push_back(' ');
    }
    return s;
}


/////////////////////////////////////////////
///// SOLUTIONNING /////
/////////////////////////////////////////////

struct State {

    void ingest_words(vector<string> v) {
        for (int w = 0; w < v.size(); ++w) {
            string &word = v[w];
            int N = word.size();
            int digit = -1;
            if (N == 2) { // 1
                // restrict the solutions now we know
                digit = 1;
            } else if (N == 3) {
                digit = 7;
            } else if (N == 4) {
                digit = 4;
            } else if (N == 7) {
                digit = 8;
            }
            if (digit != -1) {
                auto &segment_lit_up = segments[digit]; // digit uses these segments;
                for (char segment: segment_lit_up) {
                    // update every segment
                    auto restricted_candidates = unordered_set(word.begin(), word.end());
                    auto inter = intersect(candidates[segment], restricted_candidates);;
                    cout << word << " - segment = " << segment << " has possible candidates: "
                         << to_string(restricted_candidates)
                         << " inter = " << to_string(inter) << endl;
                    candidates[segment] = intersect(candidates[segment], restricted_candidates);
                }
            }
        }
    }

    char next_segment() {
        char c = ' ';
        int candidate_cnt = INT_MAX;
        for (const auto &segment: candidates) {
            int c_cnt = segment.second.size();
            if (c_cnt <= candidate_cnt && c_cnt >= 2) {
                c = segment.first;
                candidate_cnt = c_cnt;
            }
        }
        return c;
    }

    State(vector<string> &_inputs, vector<string> &_outputs) {
        inputs = _inputs;
        outputs = _outputs;

        for (const auto c: all_sol)
            candidates[c] = all_sol;

        ingest_words(inputs);
        ingest_words(outputs);
    }

    void select_candidate(char segment, char candidate) {
        candidates[segment] = {candidate};
        solution[segment] = candidate;
        path.emplace_back(segment, candidate);

        // Now the candidate has been placed, it's unavailable for the other segments
        for (auto &candidate_it: candidates) {
            if (candidate_it.first != segment) {
                candidate_it.second.erase(candidate);
                if (candidate_it.second.size() == 1 &&
                    solution.count(candidate_it.first) == 0) { // Nice! A new solution
                    if (VERBOSE) cout << "FOUND A NEW SOLUTION" << endl;
                    select_candidate(candidate_it.first, *candidate_it.second.begin());
                }
            }
        }
    }

    vector<string> inputs;
    vector<string> outputs;
    unordered_map<char, unordered_set<char>> candidates;
    unordered_map<char, char> solution; // chosen solution
    vector<pair<char, char>> path;
    unordered_map<string, int> decoder; // encoded word -> digit

    void print_path() {
        cout << " ----- PATH ---------" << endl;
        int i = 0;
        for (const auto &step: path) {
            cout << i++ << " " << step.first << " " << step.second << " " << endl;
        }
        cout << " --------------------" << endl;
    }

    void print_solution() {
        cout << " ----- SOL ---------" << endl;
        for (const auto &item: solution)
            cout << " segment = " << item.first << " has possible candidates: " << item.second << endl;

    }

    void print_current_state() {
        for (const auto &item: candidates) {
            string s(item.second.begin(), item.second.end());
            cout << " segment = " << item.first << " has possible candidates: " << s << endl;
        }
    }

    bool is_solved() {
        for (const auto &c: all_sol) { // ensure that all solution has a match
            if (solution.count(c) == 0)
                return false;
        }
        return true;
    }

    bool is_failed() {
        for (const auto &candidate: candidates) { // ensure that all solution has a match
            if (candidate.second.empty()) {
                cout << candidate.first << " is empty" << endl;
                return true;
            }
        }
        return false;
    }

    bool solution_makes_sense() {
        // check that all output words make sense (they need to return a valid number)

        if (solution.size() != all_sol.size()) {
            return false;
        }

        for (int digit = 0; digit < segments.size(); ++digit) {
            string s = "";
            for (const auto &c: segments[digit]) {
                s.push_back(solution[c]);
            }
            std::sort(s.begin(), s.end());
            decoder[s] = digit;
            if (VERBOSE) cout << "digit=" << digit << " is represented as :" << s << endl;
        }

        for (string &word: outputs) {
            if (decoder.count(word) == 0) {
                return false;
            }
        }
        return true;
    }

    bool solve(unordered_map<string, int> &decoder) {

        if (VERBOSE) {
            print_path();
            print_current_state();
        }

        if (is_failed()) {
            return false;
        }

        if (is_solved()) {
            if (solution_makes_sense()) {
                decoder = this->decoder; // solution needed for the problem. Overwrite object at address passed as arg
                return true;
            }
            return false;
        }

        char c = next_segment(); // pick a segment to kill out
        if (c == ' ') {
            // no segment available, the attempt failed
            return false;
        }

        for (char candi_seg: candidates[c]) { // iterate over candidate
            State next_state = *this; // copy past state
            next_state.select_candidate(c, candi_seg);
            bool found_sol = next_state.solve(decoder);
            if (found_sol) {
                return true;
            }
            // otherwise, keep going
        }
        return false;
    }

};


int tasks() {

    // Parsing
    string s;

    vector<vector<string>> inputs;
    vector<vector<string>> outputs;
    while (getline(cin, s)) {
        cout << "line" << s << endl;
        vector<string> input_str = split(s, "|");
        vector<string> inputs_tmp;
        vector<string> outputs_tmp;

        for (auto &s1: split(input_str.at(0), " ")) {
            if (s1.size() > 0) {
                std::sort(s1.begin(), s1.end());
                inputs_tmp.push_back(s1);
            }
        }


        for (auto &s1: split(input_str.at(1), " ")) {
            if (s1.size() > 0) {
                std::sort(s1.begin(), s1.end());
                outputs_tmp.push_back(s1);
            }
        }
        inputs.push_back(std::move(inputs_tmp));
        outputs.push_back(std::move(outputs_tmp));
    }


    vector<string> segments_str;

    unordered_set<string> segment_set;
    int i = 0;
    for (auto &seg: segments) {
        std::sort(seg.begin(), seg.end());
        string s(seg.begin(), seg.end());
        segments_str.push_back(s);
        segment_set.insert(s);
        cout << i++ << " " << s << " " << s.size() << endl;
    }

    int cnt1 = 0;
    for (const auto &line: outputs) {
        for (const auto &word: line) {
            int N = word.size();
            if (N == 2 || N == 3 || N == 4 || N == 7) {
                cnt1++;
            }
        }
    }

    cout << "sol 1=" << cnt1 << endl;

    // Part 2

    long long score = 0;

    for (int line_idx = 0; line_idx < inputs.size(); ++line_idx) {

        auto state = State(inputs[line_idx], outputs[line_idx]);
        unordered_map<string, int> decoder;
        state.print_current_state();

        bool solved = state.solve(decoder);
        if (!solved) {
            throw runtime_error("FAILED at =" + inputs[line_idx][0]);
        }

        long long cnt = 0;
        for (const auto &word: outputs[line_idx]) {
            cnt = 10 * cnt + decoder[word];
        }
        score += cnt;
        cout << "SOL = " << cnt << endl;
        cout << endl;
    }

    cout << " final sol = " << score << endl;

    return 0;

};