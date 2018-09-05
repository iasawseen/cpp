#define TESTS

#include <cstdint>

#ifdef TESTS
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#endif

#include <cstdlib>
#include <utility>
#include <cassert>
#include <iostream>
#include <limits>
#include <regex>

using namespace std;
using uint_t = uint32_t;
using int_t = int64_t;

//
// Created by ivan on 28.03.17.
//

class treap {
    struct node {
        int_t level;
        int_t priority;
        int_t size;
        int_t sum;
        int_t post_update;

        node *parent;
        node *left;
        node *right;

        node(int_t level)
                : level(level), priority(std::rand()) ,
                  size(1), sum(level), post_update(0),
                  parent(nullptr), left(nullptr), right(nullptr)
        {}

        std::string to_string() {
            return "lev: " + std::to_string(level) + " s: " +
                   std::to_string(size) + " sum: " +
                   std::to_string(sum) + " post: " +
                   std::to_string(post_update) + " ch: " +
                   std::to_string(left != nullptr) + " " +
                   std::to_string(right != nullptr);
        }
    };

public:

    treap() : root_(nullptr)
    {}

    void insert(int key) {
        insert_rec(root_, key);
    }

    bool search(int key) {
        return search_rec(root_, key);
    }

    node* get(int key) {
        return get_rec(root_, key);
    }

/*    void erase(int key) {
        erase_rec(root_, key);
    }*/

    int nth_element(int n) {
        return nth_element(root_, n);
    }

    void traverse() {
        traverse_rec(root_, "");
    }

    int lower_bound(int key) {
        if (root_ == nullptr) {
            return -1;
        }
        return lower_bound_rec(root_, key);
    }

    int num_less_or_equal(int key) {
        return num_less_or_equal_rec(root_, key);
    }


    int_t num_less_or_equal_rec(node* v, int key) {
        return -1;
    }


    int_t lower_bound_rec(node* v, int key) {
        if (v->level == key) {
            return v->level;
        }
        if (key < v->level) {
            if (v->left == nullptr) {
                return v->level;
            }
            return lower_bound_rec(v->left, key);
        }
        else {
            if (v->right == nullptr) {
                return -1;
            }
            return lower_bound_rec(v->right, key);
        }
    }

    void in_order() {
        in_order_rec(root_);
    }

    void in_order_rec(node* v) {
        if (v!= nullptr) {
            in_order_rec(v->left);
            std::cout << v->level << " ";
            in_order_rec(v->right);
        }
    }

    void traverse_rec(node* v, std::string pad) {
        if (v != nullptr) {
            std::cout << pad << v->to_string() << std::endl;
            std::cout << pad + "left" << std::endl;
            traverse_rec(v->left, pad + "  ");
            std::cout << pad + "right" << std::endl;
            traverse_rec(v->right, pad + "  ");
        }
    }

    int_t nth_element(node* v, int_t n) {
        assert(v != nullptr);

        int_t root_number = get_size(v->left);

        if (root_number == n)
            return v->level;
        if (n < root_number)
            return nth_element(v->left, n);
        else
            return nth_element(v->right, n - root_number - 1);
    }

    node* get_rec(node* v, int key) {
        if (v == nullptr) {
            return v;
        }
        if (v->level == key) {
            return v;
        }
        if (key < v->level) {
            return get_rec(v->left, key);
        }
        else {
            return get_rec(v->right, key);
        }
    }

    bool search_rec(node* v, int key)
    {
        if (v == nullptr) {
            return false;
        }
        if (v->level == key) {
            return true;
        }
        if (key < v->level) {
            return search_rec(v->left, key);
        }
        else {
            return search_rec(v->right, key);
        }
    }

    void insert_into(int_t pos, int_t key) {
        insert_into_rec(root_, pos, key);
    }

    void insert_into_rec(node*& root, int_t pos, int_t key) {
        auto splitted = implicit_split(root_, pos);

        root = merge(merge(splitted.first, new node(key)), splitted.second);
    }

/*    void print_vertex(node* v) {
        if (v == nullptr) {
            std::cout << "null" << std::endl;
            return;
        }

        std::cout << "k: " << v->level << " s: "
                  << v->size << " m: " << v->sum
                  << " " << (v->left != nullptr)
                  << " " << (v->right != nullptr) << std::endl << std::endl;
    }*/

/*    int_t get_sum_at(int_t pos) {

        auto splitted1 = implicit_split(root_, pos - 1);
        update(splitted1.first);
        update(splitted1.second);

        auto splitted2 = implicit_split(splitted1.second, 1);
        update(splitted2.first);
        update(splitted2.second);

        int_t sum = splitted2.first->sum;

        return sum;
    }*/

/*    void forward(int_t pos1, int_t pos2) {
        int_t adj_pos2 = pos2 - pos1 + 1;
        auto splitted1 = implicit_split(root_, pos1 - 1);

        auto splitted2 = implicit_split(splitted1.second, adj_pos2);

        root_ = merge(splitted2.first, merge(splitted1.first, splitted2.second));
    }*/

    std::pair<node*, node*> implicit_split(node* root, int_t pos) {
        if (root == nullptr) {
            return std::make_pair<node*, node*>(nullptr, nullptr);
        }

        push(root);

        int_t root_size = get_size(root->left);

        if (root_size < pos) {
            std::pair<node*, node*> splitted = implicit_split(root->right, pos - root_size - 1);
            root->right = splitted.first;

            set_parent(splitted.first, root);
            set_parent(splitted.second, nullptr);

            push(root);
            push(splitted.second);

            update_down(root);
            update_down(splitted.second);

            return std::make_pair(root, splitted.second);
        } else {

            std::pair<node*, node*> splitted = implicit_split(root->left, pos);
            root->left = splitted.second;

            set_parent(splitted.second, root);
            set_parent(splitted.first, nullptr);

            push(root);
            push(splitted.first);

            update_down(root);
            update_down(splitted.first);

            return std::make_pair(splitted.first, root);
        }
    };

    std::pair<node*, node*> split(node *root, int key) {
        if (root == nullptr) {
            return std::make_pair<node*, node*>(nullptr, nullptr);
        }

        if (root->level < key) {

            std::pair<node*, node*> splitted = split(root->right, key);
            root->right = splitted.first;

            set_parent(splitted.first, root);
            set_parent(splitted.second, nullptr);

            update_down(root);
            update_down(splitted.second);

            return std::make_pair(root, splitted.second);
        } else {

            std::pair<node*, node*> splitted = split(root->left, key);
            root->left = splitted.second;

            set_parent(splitted.second, root);
            set_parent(splitted.first, nullptr);

            update_down(root);
            update_down(splitted.first);

            return std::make_pair(splitted.first, root);
        }
    }

    node* merge(node *left, node *right) {
        if (left == nullptr || right == nullptr) {
            return right == nullptr ? left : right;
        }

        push(left);
        push(right);

        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            set_parent(left->right, left);
            update_down(left);

            return left;
        } else {
            right->left = merge(left, right->left);
            set_parent(right->left, right);
            update_down(right);

            return right;
        }
    }

    void insert_rec(node*& root, int key) {
        std::pair<node*, node*> splitted = split(root, key);
        root = merge(merge(splitted.first, new node(key)), splitted.second);
    }

/*    void erase_rec(node*& root, int key) {
        assert(root != nullptr);

        if (key < root->level) {
            erase_rec(root->left, key);
        } else if (key > root->level) {
            erase_rec(root->right, key);
        } else {
            node *parent = root->parent;
            root = merge(root->left, root->right);
            set_parent(root, parent);
            update_up(parent);
        }
    }*/

    int_t get_size(node* v) {
        return v == nullptr ? 0 : v->size;
    }

    int_t get_sum(node* v) {
        return v == nullptr ? 0 : v->sum;
    }

    void set_parent(node *v, node *parent) {
        if (v != nullptr) {
            v->parent = parent;
        }
    }

    void update(node* v) {
        update_down(v);
        update_up(v);
    }

    void update_up(node* v) {
        if (v == nullptr) return;

        while (v->parent != nullptr) {
            v = v->parent;
            v->size = get_size(v->left) + get_size(v->right) + 1;
            v->sum = v->level + get_sum(v->left) + get_sum(v->right);
        }
    }

    void update_down(node* v) {
        if (v == nullptr) return;
        if (v->left == nullptr && v->right == nullptr) {
            v->sum = v->level;
            v->size = 1;
            return;
        }

        v->size = get_size(v->left) + get_size(v->right) + 1;
        v->sum = v->level + get_sum(v->left) + get_sum(v->right);
    }

    void update_down_rec(node* v) {
        if (v == nullptr) return;
        if (v->left == nullptr && v->right == nullptr) {
            v->sum = v->level;
            v->size = 1;
            return;
        }

        int_t left_sum = 0;
        int_t right_sum = 0;
        int_t left_size = 0;
        int_t right_size = 0;

        if (v->left != nullptr) {
            update_down_rec(v->left);
        }
        if (v->right != nullptr) {
            update_down_rec(v->right);
        }

        v->size = get_size(v->left) + get_size(v->right) + 1;
        v->sum = v->level + get_sum(v->left) + get_sum(v->right);
    }

    void set_min_level(int_t pos1, int_t pos2, int_t new_level) {
        int_t adj_pos2 = pos2 - pos1 + 1;

        auto splitted1 = implicit_split(root_, pos1 - 1);
        auto splitted2 = implicit_split(splitted1.second, adj_pos2);

        splitted2.first->post_update = std::max(splitted2.first->post_update, new_level);
        push(splitted2.first);

        root_ = merge(splitted1.first, merge(splitted2.first, splitted2.second));
    }

    std::string get_average(int_t pos1, int_t pos2) {
        int_t adj_pos2 = pos2 - pos1 + 1;
        auto splitted1 = implicit_split(root_, pos1 - 1);

        auto splitted2 = implicit_split(splitted1.second, adj_pos2);

        propagate_rec(splitted2.first);
        update_down_rec(splitted2.first);

        int_t sum = splitted2.first->sum;

        root_ = merge(splitted1.first, merge(splitted2.first, splitted2.second));

        int_t gcd = __gcd(sum, pos2 - pos1 + 1);

        if ((sum == 0)) {
            return "0";
        }

        if (gcd == (pos2 - pos1 + 1)) {
            return std::to_string(sum / gcd);
        }
        return std::to_string(sum / gcd) + "/" + std::to_string((pos2 - pos1 + 1) / gcd);
    }

/*    void reverse_range(int_t pos1, int_t pos2) {
        int_t adj_pos2 = pos2 - pos1 + 1;

        auto splitted1 = implicit_split(root_, pos1 - 1);
        auto splitted2 = implicit_split(splitted1.second, adj_pos2);

        splitted2.first->reversed = true;


        root_ = merge(splitted1.first, merge(splitted2.first, splitted2.second));
    }*/

/*    void push_rev(node *v) {
        if (v == nullptr || !v->reversed) {
//            cout << "kek" << endl;
            return;
        }
        if (v->left != nullptr) {
            v->left->reversed ^= true;
        }
        if (v->right != nullptr) {
            v->right->reversed ^= true;
        }
        std::swap(v->left, v->right);
        v->reversed = false;
    }*/

    void push(node* v) {
        if (v == nullptr) return;
        if (v->left != nullptr) {
            v->left->post_update = std::max(v->left->post_update, v->post_update);
        }
        if (v->right != nullptr) {
            v->right->post_update = std::max(v->right->post_update, v->post_update);
        }
        v->level = std::max(v->level, v->post_update);
        v->post_update = 0;
    }

    void propagate() {
        propagate_rec(root_);
    }

    void propagate_rec(node* v) {
        if (v == nullptr) return;
        push(v);
        if (v->left != nullptr) {
            propagate_rec(v->left);
        }
        if (v->right != nullptr) {
            propagate_rec(v->right);
        }
    }

    node *root_;
};

#define TEST_UNIT1
#define TEST_UNIT2
#define TEST_UNIT3
#define TEST_UNIT4
#define TEST_UNIT5

#ifdef TESTS

TEST_CASE("Unit_1") {
#ifdef TEST_UNIT1
    treap tr;
    uint_t n = 10;

    for (uint_t i = 0; i < n; ++i ) {
        tr.insert_into(i, 0);
    }

    CHECK(tr.get_average(1, 10) == "0");
    tr.set_min_level(1, 10, 1);
    CHECK(tr.get_average(1, 10) == "1");
    tr.set_min_level(2, 3, 10);
    tr.set_min_level(3, 4, 5);
    CHECK(tr.get_average(2, 2) == "10");
    CHECK(tr.get_average(3, 3) == "10");
    CHECK(tr.get_average(4, 4) == "5");
    CHECK(tr.get_average(1, 5) == "27/5");
    CHECK(tr.get_average(1, 10) == "16/5");
#endif // TEST_UNIT1
}

TEST_CASE("Unit_2") {
#ifdef TEST_UNIT2
    treap tr;
    uint_t n = 5;

    for (uint_t i = 0; i < n; ++i ) {
        tr.insert_into(i, 0);
    }

    tr.set_min_level(1, 3, 3);
    CHECK(tr.get_average(1, 5) == "9/5");
    tr.set_min_level(1, 5, 2);
    CHECK(tr.get_average(1, 5) == "13/5");
    tr.set_min_level(2, 4, 5);
    CHECK(tr.get_average(1, 5) == "4");
    CHECK(tr.get_average(1, 1) == "3");
    CHECK(tr.get_average(2, 2) == "5");
    CHECK(tr.get_average(3, 3) == "5");
    CHECK(tr.get_average(4, 4) == "5");
    CHECK(tr.get_average(5, 5) == "2");
    tr.set_min_level(3, 3, 9);
    CHECK(tr.get_average(1, 5) == "24/5");
    tr.set_min_level(5, 5, 3);
    CHECK(tr.get_average(1, 5) == "5");

#endif // TEST_UNIT2
}

TEST_CASE("Unit_3") {
#ifdef TEST_UNIT3
    treap tr;
    uint_t n = 5;

    for (uint_t i = 0; i < n; ++i ) {
        tr.insert_into(i, 0);
    }

    tr.set_min_level(2, 2, 1);
    CHECK(tr.get_average(1, 5) == "1/5");
    CHECK(tr.get_average(1, 2) == "1/2");
    CHECK(tr.get_average(1, 1) == "0");
    tr.set_min_level(4, 4, 1);
    CHECK(tr.get_average(1, 5) == "2/5");
    CHECK(tr.get_average(1, 4) == "1/2");
    CHECK(tr.get_average(2, 4) == "2/3");
    tr.set_min_level(2, 2, 2);
    CHECK(tr.get_average(2, 4) == "1");
    tr.set_min_level(1, 5, 0);
    CHECK(tr.get_average(2, 4) == "1");
    CHECK(tr.get_average(2, 2) == "2");

#endif // TEST_UNIT3
}

TEST_CASE("Unit_4") {
#ifdef TEST_UNIT4
    treap tr;
    uint_t n = 6;

    for (uint_t i = 0; i < n; ++i ) {
        tr.insert_into(i, 0);
    }

    tr.set_min_level(1, 1, 5);
    CHECK(tr.get_average(1, 3) == "5/3");
    CHECK(tr.get_average(1, 5) == "1");
    CHECK(tr.get_average(1, 6) == "5/6");
    tr.set_min_level(4, 5, 2);
    CHECK(tr.get_average(4, 5) == "2");
    CHECK(tr.get_average(3, 3) == "0");
    CHECK(tr.get_average(6, 6) == "0");
    CHECK(tr.get_average(1, 6) == "3/2");
    tr.set_min_level(2, 3, 1);
    CHECK(tr.get_average(2, 3) == "1");
    tr.set_min_level(3, 4, 6);
    CHECK(tr.get_average(1, 6) == "10/3");
    CHECK(tr.get_average(2, 2) == "1");
    CHECK(tr.get_average(4, 5) == "4");

#endif // TEST_UNIT4
}

TEST_CASE("Unit_5") {
#ifdef TEST_UNIT5
    treap tr;
    uint_t n = 1;

    for (uint_t i = 0; i < n; ++i ) {
        tr.insert_into(i, 0);
    }

    CHECK(tr.get_average(1, 1) == "0");

#endif // TEST_UNIT5
}

#endif // TESTS


/*
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

//    freopen("input", "r", stdin);

    uint_t n = 0;
    uint_t k = 0;

    while (true) {
        cin >> n;
        cin >> k;

        if (n == 0 && k == 0) {
            break;
        }
        treap tr;

        char command = 0;
        uint_t pos1 = 0;
        uint_t pos2 = 0;
        uint_t update = 0;

        for (uint_t i = 0; i < n; ++i ) {
            tr.insert_into(i, 0);
        }

        for (uint_t i = 0; i < k; ++i) {
            cin >> command;
            if (command == '?') {
                cin >> pos1 >> pos2;
                cout << tr.get_average(pos1, pos2) << endl;
            } else if (command == '^') {
                cin >> pos1 >> pos2 >> update;
                tr.set_min_level(pos1, pos2, update);
            }
        }
    }


    return 0;
}
*/
