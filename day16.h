//
// Created by guillaume_guy on 12/14/21.
//

#ifndef AOC2021_DAY16_H
#define AOC2021_DAY16_H

#endif //AOC2021_DAY16_H

#include "vector"
#include "iostream"
#include "macro.h"
#include "array"
#include "queue"

using namespace std;

struct Packet { // with risk
    ll version;
    ll type_id;
    ll len;
    vector<Packet> reference_packet;
    ll lit_val = -1;

    friend std::ostream &operator<<(std::ostream &os, const Packet &p) {
        std::cout << p.version << " " << p.type_id << " start=" << p.len << " " << p.reference_packet.size() << " " << p.lit_val << endl;
        return os;
    }
};

array<bool, 4> hex_to_bin(char c) {
    int dec = c <= '9' ? c - '0' : 10 + (c - 'A');
    array<bool, 4> v;
    v.fill(false);
    for (int bit = 3; bit >= 0; --bit) {
        if (dec & 1) {
            v[bit] = true;
        }
        dec >>= 1;
    }
    return v;
}

ll read_binary(vector<bool> &v, ll beg, ll end) {
    int r = 0;
    ll size = end - beg; // 3 - 0 = 3;
    for (int bit = 0; bit < size; ++bit) {
        r |= v[bit + beg] << (size - bit - 1);
    }
    return r;
}

ll compute_lit_val_and_advance(vector<bool> &v, ll &pos) {
    // compute a literal value starting from `pos`
    // Advance pos to end of the sequence
    ll lit_val = 0;
    ll padding_bit_index = pos;
    bool padding_bit; // first bit
    constexpr static ll byte_size = 4;

    do {
        padding_bit = v.at(padding_bit_index);
        lit_val <<= byte_size; // shift
        ll byte = read_binary(v, padding_bit_index + 1, padding_bit_index + 1 + byte_size);
        lit_val |= byte;
        padding_bit_index += 5;
        pos += 5;
    } while (padding_bit);

    return lit_val;
}


Packet parse_input(vector<bool> &v, ll pos) {

    ll initial_pos = pos;
    ll version = read_binary(v, pos, pos + 3);
    ll type_id = read_binary(v, pos + 3, pos + 6);

    pos += 6LL; // this track the overall position in the packet;

    if (type_id == 4) {
        // literal value
        ll lit_val = compute_lit_val_and_advance(v, pos);
        return {version, type_id, pos-initial_pos, {}, lit_val};
    }
    /////////////////////////////////////
    ////////////// operators ////////////
    ////////////////////////////////////
    bool length_type_id = v.at(pos);
    pos++;
    ll packet_size_bits = length_type_id ? 11 : 15;
    vector<Packet> packets = {};

    // fill out the widths according to the relevant loc
    if (length_type_id == 0) {
        // # of bits-based processing
        ll tot_subpacket_size_bits = read_binary(v, pos, pos + packet_size_bits);

        pos += packet_size_bits;
        ll sum_bits = 0LL;
        while(sum_bits < tot_subpacket_size_bits){
            Packet packet = parse_input(v, pos);
            sum_bits += packet.len;
            pos += packet.len;
            packets.emplace_back(std::move(packet));
        }

        assert(sum_bits == tot_subpacket_size_bits);

    } else {
        // packet-cnt processing
        ll packet_cnt = read_binary(v, pos, pos + packet_size_bits);

        pos += packet_size_bits;

        for (int p = 0; p < packet_cnt; p++) {
            Packet packet = parse_input(v, pos);
            packets.emplace_back(packet);
            pos += packet.len;
        }
    }

    // debug
    ll len = pos - initial_pos;

    return {version, type_id, len, packets, -1};

}


/*
 * Solution 2
 */
ll operate(Packet &p){

    if(p.lit_val != -1){
        // literal value
        return p.lit_val;
    }

    // otherwise ...
    switch(p.type_id){
        case 0:{
            ll total = 0LL;
            for (auto &child : p.reference_packet){
                total += operate(child);
            }
            return total;
        }

        case 1:{
            ll total = 1LL;
            for (auto &child : p.reference_packet){
                total *= operate(child);
            }
            return total;
        }

        case 2:{
            ll m = LONG_LONG_MAX;
            for (auto &child : p.reference_packet){
                m = min(m,operate(child));
            }
            return m;
        }

        case 3:{
            ll m = -LONG_LONG_MAX;
            for (auto &child : p.reference_packet){
                m = max(m,operate(child));
            }
            return m;
        }

        case 5:{
            ll m = operate(p.reference_packet.at(0)) > operate(p.reference_packet.at(1)) ? 1 : 0;
            return m;
        }

        case 6:{
            ll m = operate(p.reference_packet.at(0))  < operate(p.reference_packet.at(1)) ? 1 : 0;
            return m;
        }

        case 7:{
            ll m = operate(p.reference_packet.at(0)) == operate(p.reference_packet.at(1)) ? 1 : 0;
            return m;
        }
    }

    throw runtime_error("case not handled");
}

ll sum_version(Packet &p) {
    ll version_sum = p.version;
    for (Packet &child_packet: p.reference_packet) {
        version_sum += sum_version(child_packet);
    }
    return version_sum;
}


int task1() {

    string s;

    getline(cin, s);

    vector<bool> v = {};
    for (const char c: s) {
        for (const bool bit: hex_to_bin(c)) {
            v.push_back(bit);
        }
    }

    for (const auto &item : v)
        cout << item;

    Packet head_packet = parse_input(v, 0);

    cout << "sol1 = " << sum_version(head_packet) << endl;
    cout << "sol2 = " << operate(head_packet) << endl;
    return 0;
}