//
// Created by guillaume_guy on 12/24/21.
//

#ifndef AOC2021_DAY24_H
#define AOC2021_DAY24_H

#endif //AOC2021_DAY24_H


#include "vector"
#include "iostream"
#include "macro.h"
#include <climits>


using namespace std;


unordered_map<string, string> symbol_lookup = {
        {"add", "+"},
        {"sub", "-"},
        {"div", "/"},
        {"mul", "*"},
        {"mod", "%"},
        {"eql", "=="}
};


// these are obtained with the `get_constants` function
const vector<ll> C4_constants = {1, 1, 1, 1, 26, 1, 26, 1, 26, 26, 26, 26, 1, 26};
const vector<ll> C5_constants = {14, 11, 11, 14, -11, 12, -1, 10, -3, -4, -13, -8, 13, -11};
const vector<ll> C15_constants = {12, 8, 7, 4, 4, 1, 10, 8, 12, 10, 15, 4, 10, 9};

void mul(int &i, int m) {
    i *= m;
}

void add(int &i, int m) {
    i += m;
}


void div1(int &i, int m) {
    i /= m;
}

void mod(int &i, int m) {
    i = i % m;
}

void eql(int &i, int &b) {
    i = (i == b);
}

int index(char c) {
    return c - 'a';
}

int is_valid(string &input, vector<string> &inst, int start = 0, int z0 = -1, int end = -1) {
    vector<int> variables(26, 0);
    int ind = 0;

    if (z0 != -1) variables[25] = z0;
    if (end == -1) end = (int) inst.size();

    //cout << input << endl;

    for (int i = start; i < end; ++i) {
        auto in = split(inst[i], " ");
        //cout << inst[i] << endl;
        if (in[0] == "inp") {
            variables[index(in[1][0])] = input.at(ind++) - '0';
        } else {
            char arg1 = in[1][0];

            // variable name or integer?
            string arg2 = in[2];
            char last_char = arg2[arg2.size() - 1];
            int var2 = (last_char >= '0' && last_char <= '9') ? stoi(arg2, nullptr, 10) : variables[index(arg2[0])];

            if (in[0] == "add") {
                add(variables[index(arg1)], var2);
            } else if (in[0] == "mul") {
                mul(variables[index(arg1)], var2);
            } else if (in[0] == "div") {
                div1(variables[index(arg1)], var2);
            } else if (in[0] == "mod") {
                mod(variables[index(arg1)], var2);
            } else if (in[0] == "eql") {
                eql(variables[index(arg1)], var2);
            } else {
                what_is1(in[0]);
                throw runtime_error("not implemented yet");
            }
        }
    };
    //print_vec(variables);

    return variables[25];

}


void replace(string &input, const string &from, const string &to) {
    // utility to replace a substring with another
    auto pos = 0;
    while (true) {
        size_t startPosition = input.find(from, pos);
        if (startPosition == string::npos)
            return;
        input.replace(startPosition, from.length(), to);
        pos = startPosition + to.length();
    }
}

int get_constants() {
    vector<string> inst;
    string s;
    while (getline(cin, s)) {
        inst.push_back(s);
    }


    // The instructions are made of blocks of 18 lines. They only differ by a constant on line 5 % 18
    int SIZE = 18; // size of a block
    vector<int> C4, C5, C15; // additive

    for (int i = 4; i < inst.size(); i += SIZE) {
        auto in = split(inst[i], " ");
        cout << "C4-" << in[2] << endl;
        C4.push_back(stoi(in[2]));
    }

    for (int i = 5; i < inst.size(); i += SIZE) {
        auto in = split(inst[i], " ");
        cout << "C5-" << in[2] << endl;
        C5.push_back(stoi(in[2]));
    }


    for (int i = 15; i < inst.size(); i += SIZE) {
        auto in = split(inst[i], " ");
        cout << "C15" << in[2] << endl;
        C15.push_back(stoi(in[2]));
    }

    cout << " CONSTANTS " << endl;
    print_vec(C4);
    print_vec(C5);
    print_vec(C15);

    return 0;
};

int generate_equation(vector<string> inst) {

    // take bottom first
    std::reverse(inst.begin(), inst.end());
    int ind = 14;

    string equation = "z";
    for (int i = 0; i < 18; ++i) {
        auto in = split(inst[i], " ");

        if (in[0] == "inp") {
            string upper_char = "S" + to_string(ind--);
            replace(equation, in[1], upper_char);
            continue;
        } else {
            string arg1 = in[1];
            string arg2 = in[2];

            assert(symbol_lookup.count(in[0]) == 1);

            if (arg2 == "0" && in[0] == "mul") {
                string replace_to = "0";
                replace(equation, arg1, replace_to);

            } else if (arg2 == "1" && in[0] == "div") {
                continue;
            } else {
                string replace_to = "(" + arg1 + " " + symbol_lookup[in[0]] + " " + arg2 + ")";
                replace(equation, arg1, replace_to);
            }
        }
    }

    cout << "equation= " << equation << endl;
    return 0;
}


// inp = step-th digit
ll next_z(ll z, int step, ll inp) {

    ll C4 = C4_constants[step];
    ll C5 = C5_constants[step];
    ll C15 = C15_constants[step];

    // this equation is obtained with `generate_equation` function
    ll res = ((z / C4) * (((0 + 25LL) * (((((0 + z) % 26LL) + C5) == inp) == 0)) + 1LL)) +
             (((0 + inp) + C15) * (((((0 + z) % 26LL) + C5) == inp) == 0));
    return res;
}


ll solve(unordered_map<ll, ll> &inps, int step, int task) {
    auto f = task == 1 ? [](ll a, ll b) { return max(a, b); } : [](ll a, ll b) { return min(a, b); };
    if (step == 14) {
        // no solution found
        if (inps.count(0LL) == 0) {
            ll default_num = -1LL;
            cout << " no solution";
            return default_num;
        }
        return inps[0LL];
    }

    // next step
    unordered_map<ll, ll> new_zs;
    for (const auto &[last_z, inp]: inps) {
        for (ll i = 1; i < 10; ++i) {
            ll new_z = next_z(last_z, step, i);
            if (new_zs.count(new_z)) {
                new_zs[new_z] = f(new_zs[new_z], 10LL * inp + i);
            } else {
                new_zs[new_z] = 10LL * inp + i;
            }
        }
    }

    cout << " solution space size at step=" << step << " =" << new_zs.size() << endl;

    return solve(new_zs, step + 1, task);
}


int task1() {

    vector<string> inst;
    string s;
    while (getline(cin, s)) {
        inst.push_back(s);
    }

    s = "9";

    for (int inp = 1; inp <= 9; ++inp) {
        s = to_string(inp);
        what_is2(inp, is_valid(s, inst, 234, 0, -1));
    }


    // This generates the base equation. Some arguments changes from 1 to another z-section so we extract the constants too
    // They now are in `CX_constants` where X = {4,5,14}
    generate_equation(inst);

    int initial_step = 0;

    // z -> best_inp
    unordered_map<ll, ll> initial_inps = {{0, 0}};

    ll s1 = solve(initial_inps, initial_step, 1);
    ll s2 = solve(initial_inps, initial_step, 2);
    cout << " sol 1 = " << s1 << endl;
    cout << " sol 2 = " << s2 << endl;

    return 0;
}
