//
// Created by guillaume_guy on 12/18/21.
//

#ifndef AOC2021_DAY18_H
#define AOC2021_DAY18_H

#endif //AOC2021_DAY18_H

#include "vector"
#include "string"
#include "iostream"
#include "unordered_map"
#include "unordered_set"
#include "macro.h"
#include "stack"

using namespace std;

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

constexpr int SPLIT_MIN = 10;
constexpr int LEVEL_MIN = 4;
constexpr bool VERBOSE = false;

struct Node {
    int val = -1;
    Node *left = nullptr;
    Node *right = nullptr;
    Node *parent = nullptr;

    Node(int _val, Node *_left, Node *_right, Node *_parent) {
        val = _val;
        left = _left;
        right = _right;
        parent = _parent;
    }


    Node(string &s, int &ind) {
        /*
         * This is a recursive constructor. It will advance `ind`
         */
        while (ind < s.size()) {
            if (s[ind] == '[') {
                // opening of a new node
                ind++;
                left = new Node(s, ind);
                left->parent = this;
            } else if (is_digit(s[ind])) { // end
                val = 0;
                while (is_digit(s[ind])) { // al
                    val = 10 * val + (s[ind++] - '0');
                }
                return;
            }

            assert(s[ind] == ',');
            ind++;

            // right node
            right = new Node(s, ind);
            right->parent = this;
            ind++;

            return;
        }
    }

    ~Node() {
        // if (VERBOSE) cout << "deleting " << *this << endl;
        delete left;
        delete right;
    }

    bool is_leaf() {
        return this->val != -1;
    }

    bool is_pair() {
        if (this->left && this->right)
            return this->left->is_leaf() && this->right->is_leaf();
        return false;
    }

    friend std::ostream &operator<<(std::ostream &os, const Node &n) {
        int cnt = (n.left != nullptr) + (n.right != nullptr);
        os << "val = " << n.val << " with " << cnt << " neighbors";
        if (n.left) {
            os << ": [" << n.left->val << "-" << n.right->val << "]";
        }

        os << endl;

        return os;
    }

    Node *add(Node &_right) {
        Node *new_head = new Node(-1, this, &_right, nullptr);

        // update the parents
        this->parent = new_head;
        _right.parent = new_head;

        return new_head;

    }

    void pretty_print() {
        string s;
        pretty_print_helper(s);
        cout << "tree= " << s << endl;
    }

    void pretty_print_helper(string &s) {
        if (this->is_leaf()) {
            s += to_string(val);
            return;
        }
        s += "[";
        left->pretty_print_helper(s);
        s += ",";
        right->pretty_print_helper(s);
        s += "]";
    }


    Node *find_left_helper() {
        // Find the leftmost value of the tree
        if (this->is_leaf()) {
            return this;
        }

        auto pl = this->left->find_left_helper();
        if (pl) {
            return pl;
        }

        auto pr = this->right->find_left_helper();
        if (pr) {
            return pr;
        }

        return nullptr;
    }


    Node *find_right_helper() {
        // Find the rightmost value of the tree
        if (this->is_leaf()) {
            return this;
        }

        auto pr = this->right->find_right_helper();
        if (pr) {
            return pr;
        }

        auto pl = this->left->find_right_helper();
        if (pl) {
            return pl;
        }
        return nullptr;

    }

    Node *find_right_neighbor() {
        /*
        * Find the neighbor directly to the right of `this`
         * Go up the tree as needed
         */

        if (this->is_leaf()) {
            return this;
        }

        Node *n = this;
        while (n->parent) {
            if (n != n->parent->right) {
                Node *right_neigh = n->parent->right->find_left_helper();
                if (right_neigh) {
                    return right_neigh;
                }
            }
            n = n->parent; // iterate
        }
        return nullptr;
    }


    Node *find_left_neighbor() {
        /*
        * Find the neighbor directly to the left of `this`
         * Go up the tree as needed
         */

        if (this->val != -1) {
            return this;
        }

        Node *n = this;
        while (n->parent) {
            if (n != n->parent->left) {
                Node *left_neigh = n->parent->left->find_right_helper();
                if (left_neigh) {
                    return left_neigh;
                }
            }
            n = n->parent; // iterate
        }
        return nullptr;
    }


    Node *next_exploding_pair(int level) {
        if (this->is_leaf())
            return nullptr;

        assert(this->left && this->right);

        if (this->is_pair() && level >= LEVEL_MIN) {
            return this;
        }
        Node *l = this->left->next_exploding_pair(level + 1);
        if (l) {
            return l;
        }
        Node *r = this->right->next_exploding_pair(level + 1);
        if (r) {
            return r;
        }

        return nullptr;
    }

    bool can_explode() {
        if (this->is_leaf()) return false; // non-leave can't explode
        int cnt = 0;
        Node *n = this;
        while (n->parent) {
            cnt++;
            n = n->parent;
        }
        return cnt >= LEVEL_MIN;
    }

    Node *find_split() {
        /*
         * return the node to split (leftmost node over 10)
         */
        // deal with leaves first
        if (val >= SPLIT_MIN)
            return this;
        if (val != -1)
            return nullptr;

        // non-leaves
        auto pl = left->find_split();
        if (pl)
            return pl;

        auto pr = right->find_split();
        if (pr)
            return pr;

        return nullptr;
    }

    Node *head() {
        Node *n = this;
        while (n->parent) n = n->parent;
        return n;
    }

    void split() {

        assert(this->val >= SPLIT_MIN);

        // create 2 new nodes
        Node *left_ = new Node(this->val / 2, nullptr, nullptr, this);
        Node *right_ = new Node((this->val & 1) + this->val / 2, nullptr, nullptr, this);

        this->right = right_;
        this->left = left_;

        this->val = -1;

        if (VERBOSE) {
            cout << "new struct tree after split ";
            this->head()->pretty_print();
        }
    }

    void explode() {
        // explode `this`. Use next_exploding_pair(0) to determine `this`

        assert(this->is_pair() && this->can_explode());

        Node *l_neigh = this->find_left_neighbor();
        if (l_neigh) {
            if (VERBOSE) cout << "explode - add to left neighbor (was " << l_neigh->val << ")" << endl;
            l_neigh->val += this->left->val;
        }

        Node *r_neigh = this->find_right_neighbor();
        if (r_neigh) {
            if (VERBOSE) cout << "explode - add to right neighbor (was " << r_neigh->val << ")" << endl;
            r_neigh->val += this->right->val;
        }

        // clean up
        if (VERBOSE) cout << "clean up " << endl;
        delete this->left;
        delete this->right;
        this->left = nullptr; // reset
        this->right = nullptr;
        this->val = 0;

        if (VERBOSE) {
            cout << "new struct tree after explode ";
            this->head()->pretty_print();
        }
    }

    void process() {
        assert(!this->parent); // only run on head

        bool do_next = true; // run until no more rule apply
        while (do_next) {
            do_next = false;

            // 1- Explode
            Node *explode_pair = this->next_exploding_pair(0);
            if (explode_pair) {
                explode_pair->explode();
                do_next = true;
                continue;
            }

            // 2- Split (only when no more explodes are available)
            Node *splitting_pair = this->find_split();
            if (splitting_pair) {
                splitting_pair->split();
                do_next = true;
            }
        }

        if (VERBOSE) {
            cout << " final struct ";
            this->pretty_print();
        }

    }

    ll magnitude() {
        if (this->is_leaf()) {
            return (ll) this->val;
        } else {
            return (ll) this->left->magnitude() * 3LL + (ll) this->right->magnitude() * 2LL;
        }
    }

};

ll sum_snailfish(string &s1, string &s2) {
    int i = 0;
    Node *a = new Node(s1, i);
    i = 0;
    Node *b = new Node(s2, i);
    Node *c = a->add(*b);
    c->process();
    ll m = c->magnitude();
    delete c; // free up memory recursively (a + c)
    return m;
}


int task1() {
    string s;
    int i = 0;
    getline(cin, s);
    Node *n = new Node(s, i);
    n->process();

    while (getline(cin, s)) {
        i = 0;
        Node *n1 = new Node(s, i);
        n = n->add(*n1);
        n->process();
    }
    if (VERBOSE) cout << " magnitude =" << n->magnitude() << endl;

    delete n;

    return 0;
}

int task2() {

    string s;
    vector<string> v;

    while (getline(cin, s)) {
        if (!s.empty())
            v.push_back(s);
    }

    ll m = 0LL;
    for (int i = 0; i < v.size(); ++i) {
        for (int j = 0; j < v.size(); ++j) {
            if (i != j)
                m = max(m, sum_snailfish(v.at(i), v.at(j)));
        }
    }

    cout << " highest magnitude " << m << endl;

    return 0;
}
