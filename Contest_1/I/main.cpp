#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include "treap.hpp"

using namespace std;
using int_t = uint32_t;


struct point {
    explicit point(int_t x, int_t y) {
        this->x = x;
        this->y = y;
    }

    int_t x;
    int_t y;
};


vector<int_t> sort_indices(vector<int_t> const& v) {
    vector<int_t> idx(v.size());
    std::iota(idx.begin(), idx.end(), 0);

    sort(idx.begin(), idx.end(),
         [&v](int_t i1, int_t i2) {return v[i1] < v[i2];});

    return idx;
}

bool is_in_between(point a, point b, point c) {
    uint64_t cross_prod = (c.y - a.y) * (b.x - a.x) - (c.x - a.x) * (b.y - a.y);
    if (cross_prod == 0) {
        return false;
    }

    uint64_t dot_prod = (c.x - a.x) * (b.x - a.x) + (c.y - a.y) * (b.y - a.y);
    if (dot_prod < 0) {
        return false;
    }

    uint64_t squared_len_ba = (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y);
//    if (dot_prod > squared_len_ba) {
//        return false;
//    }
//    return true;
    return dot_prod <= squared_len_ba;
}

//def isBetween(a, b, c):
//crossproduct = (c.y - a.y) * (b.x - a.x) - (c.x - a.x) * (b.y - a.y)
//if abs(crossproduct) > epsilon : return False   # (or != 0 if using integers)
//
//dotproduct = (c.x - a.x) * (b.x - a.x) + (c.y - a.y)*(b.y - a.y)
//if dotproduct < 0 : return False
//
//        squaredlengthba = (b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y)
//if dotproduct > squaredlengthba: return False
//
//return True

void test_is_in_between() {
    point a = point(0, 3);
    point b = point(3, 9);
    point c = point(2, 7);

//    assert(is_in_between(a, b, c));
}

void test_treap() {
    vector<int> v(10, 0);
    std::iota(v.begin(), v.end(), 0);
    treap tr;

    srand(1);

    for (auto& item : v) {
        tr.insert(item);
    }

//    tr.traverse();

    for (auto& item : v) {
        assert(tr.search(item));
        assert(tr.nth_element(item) == item);
    }

    tr.erase(5);
    assert(!tr.search(5));

    cout << endl;
    tr.erase(7);

    for (int i = 0; i < 8; ++i) {
        cout << tr.nth_element(i) << endl;
    }

    cout << endl;
//    tr.traverse();
    cout << tr.lower_bound() << endl;
}

using namespace std;

int main() {

    test_treap();

    return 0;
}