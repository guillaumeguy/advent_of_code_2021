//
// Created by guillaume_guy on 12/9/21.
//

#ifndef AOC2021_MACRO_H
#define AOC2021_MACRO_H

#endif //AOC2021_MACRO_H

#include "unordered_set"
#include <emmintrin.h>
#include "unordered_map"
#include <algorithm>


#define what_is0(x) cerr << #x << " is " << x << endl;
#define what_is1(x) cerr << #x << " is " << x << endl;
#define what_is2(a, b) cerr << #a << "=" << a  << " " << #b << " = " << b << endl;
#define what_is3(a, b, c) cerr << #a << "=" << a  << ", " << #b << "=" << b << ", " << #c << "=" << c << endl;
#define what_is4(a, b, c, d) cerr << #a << "=" << a  << ", " << #b << "=" << b << ", " << #c << "=" << c <<  ", " << #d << "=" << d << endl;
#define what_is5(a, b, c, d, e) cerr << #a << "=" << a  << ", " << #b << "=" << b << ", " << #c << "=" << c <<  ", " << #d << "=" << d << ", " << #e << "=" << e << endl;
#define what_is6(a, b, c, d, e, f) cerr << #a << "=" << a  << ", " << #b << "=" << b << ", " << #c << "=" << c <<  ", " << #d << "=" << d << ", " << #e << "=" << e << ", " << #f << "=" << f << endl;

#define pii pair<int,int>
#define tii tuple<int,int,int>
#define vi vector<int>

typedef long long ll;
typedef unsigned long long ull;

#define all(a) (a).begin(),(a).end()




template<typename T>
std::ostream &operator<<(std::ostream &os, const std::pair<T, T> &e) {
    os << "(" << e.first << "-" << e.second << ")";
    return os;
}


template<typename T>
void print_vec(std::vector<T> &v) {
    std::cout << "[";
    for (int r = 0; r < v.size(); ++r)
        std::cout << v[r] << ",";
    std::cout << "]";
    std::cout << std::endl;
}


template<typename T, int N>
int count_distinct(std::array<T, N> &v) {
    std::unordered_set<T> s;
    int cnt = 0;
    for (auto &e: v) {
        if (s.count(e) == 0) {
            s.insert(e);
            cnt++;
        }
    }
    return cnt;
}


template<typename T, int N>
bool sorted_equal(std::array<T, N> v1, std::array<T, N> v2) {
    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    for (int i = 0; i < N; ++i) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }
    return true;

}


/*
template<typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    std::cout << "[";
    for (const auto &item : v)
        os << item  << "," ;
    std::cout << "]";
    return os;
}
*/


template<typename K, typename V>
void print_map(std::unordered_map<K, V> &m) {
    for (auto &e: m) {
        std::cout << e.first << "->" << e.second << std::endl;
    }
    std::cout << std::endl;
}

template<typename V, int N>
void print_array(std::array<V, N> &v) {
    std::cout << "[";
    for (int r = 0; r < v.size(); ++r)
        std::cout << v[r] << ",";
    std::cout << "]";
    std::cout << std::endl;
}

template<typename T>
int multiset_intersect(std::unordered_multiset<T> &a, std::unordered_multiset<T> &b) {
    int cnt = 0;
    for (const auto &e: a) {
        cnt += b.count(e);
    }
    return cnt;
}


template<typename T, typename std::enable_if<!std::is_same<T, bool>::value>>
ll sum(std::vector<T> &v) {
    ll s = 0LL;
    for (int i = 0; i < v.size(); ++i) {
        s += v[i];
    }
    return s;
}

template<typename T, typename std::enable_if<std::is_same<T, bool>::value>>
T sum(std::vector<T> &v) {
    T s = T(0);
    for (int i = 0; i < v.size(); ++i) {
        s += v[i];
    }
    return s;
}

template<typename T>
long long sum(std::unordered_set<T> &v) {
    long long s = 0LL;
    for (auto &a: v) {
        s += a;
    }
    return s;
}

template<typename T, int N>
T sum(std::array<T, N> &v) {
    T s = 0;
    for (int i = 0; i < N; ++i) {
        s += v[i];
    }
    return s;
}


template<typename T>
T mult(std::vector<T> &v) {
    T m = 1;
    for (int i = 0; i < v.size(); ++i) {
        m *= v[i];
    }
    return m;
}

template<typename T, int N>
T mult(std::array<T, N> &v) {
    T s = 1;
    for (int i = 0; i < N; ++i) {
        s *= v[i];
    }
    return s;
}


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

