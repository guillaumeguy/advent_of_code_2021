//
// Created by guillaume_guy on 12/25/21.
// I implemented it as a Dijkstra algo using Zobrist hashing for game state (apparently, quite popular in chess)
//

#ifndef AOC2021_DAY23_P2_H
#define AOC2021_DAY23_P2_H

#endif //AOC2021_DAY23_P2_H

#include "vector"
#include "iostream"
#include "macro.h"
#include "array"
#include "queue"
#include <climits>
#include <random>

using namespace std;

constexpr bool VERBOSE = false;

typedef unsigned long long ull;

typedef int room_t;

vector<int> energy(256, -INT_MAX / 2); // only a few cells used

constexpr int DEPTH = 4;
constexpr int HALLWAY_SIZE = 11;

struct Room {
    // A room is vertical and connects to the hallway. It's basically a stack
    /*
     * <---- HALLWAY ---->
     * #| |# --> 3 (closest to hallway)
     * #| |# --> 2
     * #| |# --> 1
     * #| |# --> 0 (lowest level)
     *
     */
    array<char, DEPTH> elements;
    int cursor = DEPTH; // also element cnt
    char c = 'A';

    bool full() {
        return cursor == DEPTH;
    }

    bool can_enter(char ch) {
        return c == ch && !full();
    }

    int add_to_room(char ch) {
        // also returns the steps required to insert the element
        elements[cursor++] = ch;
        return 1 + DEPTH - cursor;
    }

    char top() {
        return elements.at(cursor - 1);
    }

    bool empty() {
        return cursor == 0;
    }

    int dist() {
        // For top element to get out
        return 1 + DEPTH - cursor;
    }

    char pop() {
        char ch = this->top();
        cursor--;
        elements.at(cursor) = '.';
        return ch;
    }

    bool is_finished() {
        if (cursor != DEPTH) return false;

        for (int i = 0; i < cursor; ++i) {
            if (elements[i] != c) return false;
        }
        return true;
    }

    bool clean() {
        /*
         * Only contains matching antipods
         */
        for (int i = 0; i < cursor; ++i) {
            if (elements[i] != c) return false;
        }
        return true;
    }

};

struct State {
    ll score = 0LL;
    ll cost = 0LL;
    ll extra_cost;
    array<char, HALLWAY_SIZE> hallway; // no #
    array<Room, 4> rooms;

    State(array<char, HALLWAY_SIZE> hallway_, array<array<char, DEPTH>, 4> &_rooms) : hallway(hallway_) {
        for (int i = 0; i < 4; ++i) {
            reverse(_rooms[i].begin(), _rooms[i].end());
            rooms[i].elements = _rooms[i];
            rooms[i].c = 'A' + i;
        }
        refresh_extra_cost();
        refresh_score();
    }

    bool hallway_occupied_at(int r) {
        return hallway[r] >= 'A' && hallway[r] <= 'D';
    }

    int room_entrance(char c) {
        return 2 + (c - 'A') * 2;
    }

    room_t target_room(char c) {
        return (c - 'A');
    }

    room_t room(int r) {
        return (r - 2) / 2;
    }

    bool is_room_entrance(int r) {
        if (r < 2 || r > 8) return false;
        return r % 2 == 0;
    }

    bool operator==(const State &t) const {
        if (t.hallway != this->hallway) return false;

        for (int room = 0; room < 4; ++room) {
            if (rooms[room].elements != t.rooms[room].elements) return false;
        }

        return true;
    }

    void refresh_score() {
        score = -(cost + extra_cost);
    }

    void print() {

        for (auto c: hallway)
            cout << c;
        cout << endl;
        for (int r = 0; r < DEPTH; ++r) {
            cout << "##";
            for (int i = 0; i < 4; ++i)
                cout << rooms[i].elements[3 - r] << "#";
            cout << "#" << endl;
        }
    }

    vector<State> find_states(int r, bool from_hallway = true) {
        vector<State> v;

        room_t room_id = room(r); // identify room it's from coming
        if (!from_hallway) assert(!this->rooms[room_id].empty());
        char ch = from_hallway ? hallway[r] : this->rooms[room_id].top();
        int e = energy[ch];

        int room_entrance_row = room_entrance(ch);

        // Forward
        if (VERBOSE) cout << "FORWARD" << endl;
        for (int r1 = r + 1; r1 < HALLWAY_SIZE; ++r1) {
            if (hallway_occupied_at(r1)) {
                break;
            }

            // enter a room?
            auto room = this->rooms[target_room(ch)];
            if (room_entrance_row == r1 && !room.full() && room.clean()) {
                State s = *this;

                int steps = s.rooms[target_room(ch)].add_to_room(ch); // to get in
                s.cost += e * (abs(r1 - r) + steps);
                if (from_hallway) {
                    s.hallway[r] = '.';
                } else {
                    int extra_steps = s.rooms[room_id].dist(); // to get out
                    s.cost += e * extra_steps;
                    s.rooms[room_id].pop();
                }
                refresh_extra_cost();
                refresh_score();
                v.push_back(s);
                continue;
            }

            // once in the hallway, amphipods can't move to another spot in the hallway
            if (from_hallway) continue;

            // entrance but not the desired one
            if (is_room_entrance(r1)) {
                continue;
            }

            State s = *this;
            s.cost += e * abs(r1 - r);

            if (from_hallway) {
                swap(s.hallway[r1], s.hallway[r]);
            } else {
                s.hallway[r1] = ch;
                int extra_steps = s.rooms[room_id].dist(); // to get out
                s.cost += e * extra_steps;
                s.rooms[room_id].pop();
            }
            refresh_extra_cost();
            refresh_score();
            v.push_back(s);

        }

        if (VERBOSE) cout << "BACKWARD" << endl;
        // Backward
        for (int r1 = r - 1; r1 >= 0; --r1) {
            if (VERBOSE) cout << " backward col r=" << r1 << endl;
            if (hallway_occupied_at(r1)) {
                break;
            }

            // enter a room?
            auto room = this->rooms[target_room(ch)];
            if (room_entrance_row == r1 && !room.full() && room.clean()) {
                State s = *this;

                int steps = s.rooms[target_room(ch)].add_to_room(ch); // to get in
                s.cost += e * (abs(r1 - r) + steps);
                if (from_hallway) {
                    s.hallway[r] = '.';
                } else {
                    int extra_steps = s.rooms[room_id].dist(); // to get out
                    s.cost += e * extra_steps;
                    s.rooms[room_id].pop();
                }
                s.refresh_extra_cost();
                s.refresh_score();
                v.push_back(s);
                continue;
            }

            // once in the hallway, amphipods can't move to another spot in the hallway
            if (from_hallway) continue;

            // entrance but not the desired one
            if (is_room_entrance(r1)) {
                continue;
            }

            State s = *this;
            s.cost += e * abs(r1 - r);

            if (from_hallway) {
                swap(s.hallway[r1], s.hallway[r]);
            } else {
                s.hallway[r1] = ch;
                int extra_steps = s.rooms[room_id].dist(); // to get out
                s.cost += e * extra_steps;
                s.rooms[room_id].pop();
            }
            refresh_extra_cost();
            refresh_score();
            v.push_back(s);
        }
        return v;
    }

    vector<State> next_states() {
        vector<State> v;

        // iterate over hallway
        for (int c = 0; c < HALLWAY_SIZE; ++c) {
            if (hallway[c] == '.') continue; // nothing to move

            vector<State> v1 = find_states(c, true);
            v.insert(v.end(), v1.begin(), v1.end());
        }

        // now over room
        for (int room_id = 0; room_id < 4; ++room_id) {
            if (!rooms[room_id].empty()) {
                vector<State> v1 = find_states(2 + 2 * room_id, false);

                v.insert(v.end(), v1.begin(), v1.end());
            }
        }
        return v;
    }

    void refresh_extra_cost() {
        /*
        * putting all non-placed items back into their spot
        */
        extra_cost = 0;

        for (int r = 0; r < HALLWAY_SIZE; ++r) {
            if (hallway_occupied_at(r)) {
                int dst_row = room_entrance(hallway[r]);
                extra_cost += (abs(dst_row - r) + 1) * energy[hallway[r]];
            }
        }

        // now the rooms
        for (int room = 0; room < 4; ++room) {
            for (int i = 0; i < rooms[room].cursor; ++i) {
                char expected_letter = 'A' + room;
                char ch = rooms[room].elements[i];
                if (ch != expected_letter) {
                    int dst_row = room_entrance(expected_letter);
                    int current_row = room_entrance(ch);
                    int steps_out = 4 - i;
                    extra_cost += (abs(current_row - dst_row) + 1 + steps_out) * energy[ch];
                }
            }
        }
    }

    bool finished_all() {
        for (int i = 0; i < 4; ++i) {
            if (!rooms[i].is_finished()) return false;
        }
        return true;
    }

};

vector<ull> generate_bitstring(int l) {
    int SEED = 132;
    std::default_random_engine generator(SEED);
    std::uniform_int_distribution<ull> distribution(0, LONG_LONG_MAX);
    vector<ull> v(l);
    std::generate(v.begin(), v.end(), [&]() -> ull { return distribution(generator); });
    return v;
}

struct Zobrist {
    // stores the random bitstrings for each position / occupant on it
    vector<ull> hallway_bitstring;
    vector<ull> room_bitstring;
    array<int, 256> indices;
    int symbol_cnt;

    Zobrist(int hallway_size, int _symbol_cnt) : symbol_cnt(_symbol_cnt) {
        int room_cnt = 4;

        indices.fill(0);
        int i = 0;
        for (auto c: {'A', 'B', 'C', 'D'}) {
            indices[c] = i++;
        }

        hallway_bitstring = generate_bitstring(hallway_size * symbol_cnt);
        room_bitstring = generate_bitstring(room_cnt * DEPTH * symbol_cnt);
    }
};


auto zobrist_hash = [](State &s, Zobrist &z) -> ull {
    // Compute the hash of a state given the Zobrist bistrings
    ull h = 0ULL;

    // hash hallway
    for (int i = 0; i < HALLWAY_SIZE; ++i) {
        if (s.hallway[i] != '.') {
            int ind = z.indices[s.hallway[i]];
            assert(ind >= 0 && ind <= 3);
            h ^= z.hallway_bitstring.at(i * z.symbol_cnt + ind);
        }
    }

    // hash rooms
    for (int room = 0; room < 4; ++room) {
        for (int i = 0; i < s.rooms[room].cursor; ++i) {
            int ind = z.indices[s.rooms[room].elements[i]];
            assert(ind >= 0 && ind <= 3);
            h ^= z.room_bitstring.at((i + room * DEPTH) * z.symbol_cnt + ind);
        }
    }
    return h;
};


int task2() {

    int symbol_cnt = 4;

    energy['A'] = 1;
    energy['B'] = 10;
    energy['C'] = 100;
    energy['D'] = 1000;

    string s;
    getline(cin, s); // skip first row

    array<char, HALLWAY_SIZE> hallway;

    array<array<char, DEPTH>, 4> rooms;

    int row = 1;
    while (getline(cin, s)) {
        if (row == 1) {
            for (int i = 0; i < HALLWAY_SIZE; ++i)
                hallway[i] = s[i + 1];
        } else if (row < DEPTH + 2) {
            for (int i = 0; i < 4; ++i) {
                rooms[i][row - 2] = s[3 + i * 2];
            }
        }
        row++;
    }

    State state = State(hallway, rooms);

    // pick greater score first
    auto comp = [](State &lhs, State &rhs) -> bool { return lhs.score < rhs.score; };

    priority_queue<State, vector<State>, decltype(comp)> q(comp); // locations that need visiting

    q.emplace(state);

    ll best_cost = INT_MAX / 2;

    Zobrist zobrist = Zobrist(HALLWAY_SIZE, symbol_cnt);
    auto zobrist_hash_specialized = [&](State s) -> ull { return zobrist_hash(s, zobrist); };

    unordered_map<State, ll, decltype(zobrist_hash_specialized)> costs{0, zobrist_hash_specialized};

    int visited = 0;
    bool has_sol = false;
    State solution_state = q.top();

    // Find all the states that fill out
    while (!q.empty()) {

        State s = q.top();
        q.pop();

        if (visited++ <= 100 || visited % 10000 == 0) {
            s.print();
            cout << "State visited" << visited << " " << s.score << "raw cost=" << s.cost << ". Tot cost="
                 << s.cost + s.extra_cost << endl;
            cout << "   Queue size=" << q.size() << " map size" << costs.size() << endl;
            cout << "   Best cost=" << best_cost << endl;
        }

        if (s.cost + s.extra_cost >= best_cost) {
            continue;
        }

        for (auto &next_state: s.next_states()) {
            if (next_state.finished_all() && next_state.cost < best_cost) {
                // Update
                best_cost = next_state.cost;
                what_is2(best_cost, next_state.cost);
                next_state.print();
                solution_state = next_state;
                costs[next_state] = next_state.cost;
                has_sol = true;
                continue;
            }

            // Is it better than the current path?
            if (costs.count(next_state) == 1) {
                // better solution!
                if (costs[next_state] > next_state.cost) {
                    costs[next_state] = next_state.cost;
                    q.emplace(next_state);
                } else {
                    // worse solution, no action
                }
            } else {
                // no solution yet
                costs[next_state] = next_state.cost;
                q.emplace(next_state);
            }
        }
    }

    if (has_sol) {
        cout << "--- final answer --- " << endl;
        solution_state.print();
        cout << best_cost << endl;
    }

    return 0;
};
