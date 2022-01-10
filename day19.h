//
// Created by guillaume_guy on 12/20/21.
//

#ifndef AOC2021_DAY19_H
#define AOC2021_DAY19_H

#endif //AOC2021_DAY19_H

#include "vector"
#include "iostream"
#include "macro.h"
#include "array"
#include "queue"

using namespace std;

// Pair1 : Point1, Point2 (P11,P12)
// Pair2 : Point1, Point2 (P21,P22)

constexpr int SIZE = 3;

constexpr int MIN_OVERLAP_BEACONS = 12;

int square(int x) {
    return x * x;
}

template<typename K, typename V>
pair<K, V> peak(unordered_map<K, V> &v) {
    for (const auto &item: v)
        return {item.first, item.second};

    throw runtime_error("empty map. Can't peak");
}

struct Point {
    int x;
    int y;
    int z;

    friend inline bool operator==(const Point &lhs, const Point &rhs) {
        return (lhs.x == rhs.x &&
                lhs.y == rhs.y &&
                lhs.z == rhs.z);
    }


    array<int, SIZE> minus(Point &rhs) {
        /* diff along 3 axes
         *
         */
        return {
                (this->x - rhs.x),
                (this->y - rhs.y),
                (this->z - rhs.z)
        };
    }

    array<int, SIZE> delta(Point &rhs) {
        /* absolute diff along 3 axes
         *
         */
        return {
                abs(this->x - rhs.x),
                abs(this->y - rhs.y),
                abs(this->z - rhs.z)
        };
    }

    ll dist(Point &p1) {
        return square(this->x - p1.x) + square(this->y - p1.y) + square(this->z - p1.z);
    }

    ll manhattan(Point &p1) {
        return abs(this->x - p1.x) + abs(this->y - p1.y) + abs(this->z - p1.z);
    }

    ull dist_hash(Point &p1) {
        array<ull, 3> tmp = {(ull) abs(this->x - p1.x), (ull) abs(this->y - p1.y), (ull) abs(this->z - p1.z)};
        std::sort(tmp.begin(), tmp.end(), greater<ull>());
        return (tmp[0] << 26) + (tmp[1] << 13) + tmp[2];
    }

    friend std::ostream &operator<<(std::ostream &os, const Point &p) {
        os << "{" << p.x << "," << p.y << "," << p.z << "}" << endl;

        return os;
    }

};


template<typename T>
typename std::enable_if<std::is_signed<T>::value, int>::type
inline constexpr signum(T const x) {
    return (T(0) < x) - (x < T(0));
}


struct Transform {

    array<int, SIZE> rotation;
    array<int, SIZE> translation;

    Transform(array<int, SIZE> &trans_, array<int, SIZE> &rot_) {
        rotation = rot_;
        translation = trans_;
    }


    friend std::ostream &operator<<(std::ostream &os, const Transform &t) {
        os << "Translation = {" << t.translation[0] << "," << t.translation[1] << "," << t.translation[2]
           << "}- Rotation {";
        os << t.rotation[0] << "," << t.rotation[1] << "," << t.rotation[2] << "}" << endl;
        return os;
    }


    /* Notes:
     * Rotation of {2,1,-3} means:
     * 2  ==> x goes to axis =2 (y) with positive sign (not flipped)
     * 1  ==> y goes to axis =1 (x) with positive sign (not flipped)
     * -3 ==> z goes to axis =3 (z) with negative sign (flipped)
     * */

    Point transform(Point &p) {
        array<int, SIZE> res{p.x, p.y, p.z};
        int x = translation[0] + signum(rotation[0]) * res[abs(rotation[0]) - 1];
        int y = translation[1] + signum(rotation[1]) * res[abs(rotation[1]) - 1];
        int z = translation[2] + signum(rotation[2]) * res[abs(rotation[2]) - 1];
        return {x, y, z};
    }

    vector<Point> transform(vector<Point> &v) {
        vector<Point> v1;
        v1.reserve(v.size());
        for (const auto &p: v) {
            array<int, SIZE> res{p.x, p.y, p.z};
            int x = translation[0] + signum(rotation[0]) * res[abs(rotation[0]) - 1];
            int y = translation[1] + signum(rotation[1]) * res[abs(rotation[1]) - 1];
            int z = translation[2] + signum(rotation[2]) * res[abs(rotation[2]) - 1];
            v1.push_back({x, y, z});
        }
        return v1;
    }
};

struct Scanner {
    vector<Point> readings;
    vector<unordered_multiset<ull>> signatures; // A set of distances to the Scanner's beacons
    int n;

    Scanner(vector<Point> &pts_) {
        readings = pts_;
        n = readings.size();
        signatures.resize(n);

        int reading_cnt = readings.size();
        for (int i = 0; i < reading_cnt; ++i) {
            for (int j = i + 1; j < reading_cnt; ++j) {
                ll d = readings[i].dist_hash(readings[j]);
                signatures[i].insert(d);
                signatures[j].insert(d);
            }
        }
    }

    unordered_map<int, int> beacon_mapping(Scanner &rhs) {

        // Since there are 12 overlapping we will find that every
        unordered_map<int, int> mapping;

        for (int i = 0; i < this->n; ++i) {
            for (int j = 0; j < rhs.n; ++j) {
                int cnt = multiset_intersect(this->signatures[i], rhs.signatures[j]);
                if (cnt >= 11) mapping[j] = i;
            }
        }

        if (mapping.size() >= 12) return mapping;
        return {};
    };

    Transform get_transform(Scanner &rhs) {
        /*
         * From 2 overlapping scanners, get the transform from rhs using `this` as reference
         * We do it in two steps:
         * 1- Find rotation (absolute)
         * 2- Find translation
         */
        assert(this->beacon_mapping(rhs).size() >= 12);

        unordered_map<int, int> m = this->beacon_mapping(rhs);

        array<int, SIZE> index_this;
        array<int, SIZE> index_rhs;
        {
            int i = 0;
            for (auto &e: m) {
                if (i == 3) break;
                index_rhs[i] = e.first;
                index_this[i] = e.second;
                i++;
            }
        }

        // 1- Find rotation

        // We will infer absolute rotation (e.g. x->y) without the sign
        array<int, SIZE> base_rotation = {1, 2, 3};

        Point p11 = this->readings[index_this[0]];
        Point p12 = this->readings[index_this[1]];
        array<int, SIZE> deltas_this = p11.delta(p12);
        int ndist = count_distinct<int, 3>(deltas_this);

        if (ndist != SIZE) {
            cout << "skip point pair due to ndist != 3" << endl;
            throw runtime_error("bad pair");
        } // 3 different readings make things easier to differentiate.

        Point p21 = rhs.readings[index_rhs[0]];
        Point p22 = rhs.readings[index_rhs[1]];
        array<int, SIZE> deltas_rhs = p21.delta(p22);

        // We ensure that delta can be matched
        if (!sorted_equal<int, SIZE>(deltas_this, deltas_rhs)) {
            print_array<int, SIZE>(deltas_this);
            print_array<int, SIZE>(deltas_rhs);
            throw runtime_error("each of the 3 dimension needs to be different");
        }

        // find mapping between this's axes and rhs axes
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (deltas_rhs[i] == deltas_this[j]) { // axis i is actually axis j+1 in the `this` reference
                    base_rotation[j] = i + 1;
                    break;
                }
            }
        }

        // 2- Find translation now

        // we don't know about the sign. So we try all 2*2*2 = 8 possibilities
        array<int, SIZE> no_translate = {0, 0, 0};
        for (int x_s: {-1, 1}) {
            for (int y_s: {-1, 1}) {
                for (int z_s: {-1, 1}) {
                    array<int, SIZE> signed_rotation = {base_rotation[0] * x_s,
                                                        base_rotation[1] * y_s,
                                                        base_rotation[2] * z_s
                    };

                    auto t = Transform(no_translate, signed_rotation);

                    // Calibrate the translation from looking at the first mapping
                    auto m1 = peak(m);
                    Point p = rhs.readings[m1.first];
                    Point p1 = t.transform(p);

                    Point p1_truth = this->readings.at(m1.second);
                    array<int, 3> inf_translation = p1_truth.minus(p1);

                    // update the translation with the new
                    t = Transform(inf_translation, signed_rotation);

                    bool all_equal = true;

                    // validate it's working on all overlapping points
                    int i = 0;
                    for (const auto &rhs_to_this: m) {
                        Point p = rhs.readings[rhs_to_this.first];
                        Point p1 = t.transform(p);
                        Point p1_truth = this->readings[rhs_to_this.second];
                        i++;
                        if (!(p1 == p1_truth)) {
                            all_equal = false;
                            break;
                        }
                    }
                    if (all_equal)
                        return t;
                }
            }
        }
        // no solution found. this should not happen
        throw runtime_error(" no transform found");
    }


};


int task1() {

    array<int, 3> v = {1, 2, 3};
    int res = count_distinct<int, 3>(v);
    assert(res == 3);
    v = {1, 1, 3};
    res = count_distinct<int, 3>(v);
    assert(res == 2);

    string s;
    vector<vector<Point>> scanner_readings;
    int i = -1;
    while (getline(cin, s)) {
        vector<Point> scanner_tmp;
        if (s.empty()) {

            continue;
        }
        if (s[1] == '-') {
            // we assume scanners are ordered.
            // new scanner
            i++;
            scanner_readings.emplace_back(); // initialize vector<Point>
            continue;
        }

        auto v = split(s, ",");
        scanner_readings[i].push_back({stoi(v[0]), stoi(v[1]), stoi(v[2])});
    }

    int n = (int) scanner_readings.size();

    vector<Scanner> scanners;
    scanners.reserve(n);
    for (int j = 0; j < n; ++j)
        scanners.emplace_back(scanner_readings[j]);

    vector<bool> transformed(n, false);
    transformed[0] = true;
    int cnt = 1; // get this to n;

    vector<vector<Point>> scanner_readings_in_space0;
    scanner_readings_in_space0 = scanner_readings; // copy
    Point p0 = {0, 0, 0};

    vector<Point> scanner_loc(n); // the location of each scanner in space 0

    while (cnt < n) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (j == i) continue;
                // transformed[i] means that the i-scanner is already in space0
                if (transformed[i] && !transformed[j]) {
                    auto mapping = scanners[i].beacon_mapping(scanners[j]);
                    if (mapping.size() >= 12) {
                        cout << "scanner i,j overlap" << i << " " << j << endl;

                        auto t = scanners[i].get_transform(scanners[j]);
                        auto space0_readings = t.transform(scanners[j].readings);

                        scanner_loc[j] = t.transform(p0);
                        cout << "scanner " << j << " is located at " << t.transform(p0) << endl;

                        scanner_readings_in_space0[j] = space0_readings;
                        transformed[j] = true;
                        // overwrite readings
                        scanners[j].readings = space0_readings;
                        cnt++;
                    }
                }
            }
        }
    }

    auto h = [](const Point &p) { return ((ll) p.x << 24) + ((ll) p.y << 12) + (ll) p.z; };

    unordered_set<Point, decltype(h)> point_set{0, h};
    for (const auto &scanner_reading: scanner_readings_in_space0) {
        for (const auto &p: scanner_reading) {
            point_set.insert(p);
        }
    }

    cout << "sol1=" << point_set.size() << endl;

    ll m = 0;
    for (int j = 0; j < n; ++j) {
        for (int k = 0; k < n; ++k) {
            if (j != k)
                m = max(m, scanner_loc[j].manhattan(scanner_loc[k]));
        }
    }
    cout << "sol2=" << m << endl;

    return 0;
}
