#include <iostream>

using namespace std;
using uint_t = uint64_t;
using int_t = int64_t;

//
// Created by ivan on 28.03.17.
//

#include <cstdlib>
#include <utility>
#include <cassert>
#include <iostream>
#include <limits>


using int_t = int64_t;


class treap {
    struct node {
        int_t key;
        int_t priority;
        int_t size;
        int_t sum;
        bool reversed;

        node *parent;
        node *left;
        node *right;

        node(int_t key)
                : key(key), priority(std::rand()), size(1), sum(key),
                  reversed(false),
                  parent(nullptr), left(nullptr), right(nullptr)
        {}

        std::string to_string() {
            return "k: " + std::to_string(key) + " s: " +
                   std::to_string(size) + " sum: " +
                   std::to_string(sum) + " rev: " +
                   std::to_string(reversed) + " " +
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

    void erase(int key) {
        erase_rec(root_, key);
    }

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
        if (v->key == key) {
            return v->key;
        }
        if (key < v->key) {
            if (v->left == nullptr) {
                return v->key;
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
//            std::cout << v->key << " ";
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
            return v->key;
        if (n < root_number)
            return nth_element(v->left, n);
        else
            return nth_element(v->right, n - root_number - 1);
    }

    node* get_rec(node* v, int key) {
        if (v == nullptr) {
            return v;
        }
        if (v->key == key) {
            return v;
        }
        if (key < v->key) {
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
        if (v->key == key) {
            return true;
        }
        if (key < v->key) {
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

    void print_vertex(node* v) {
        if (v == nullptr) {
            std::cout << "null" << std::endl;
            return;
        }

        std::cout << "k: " << v->key << " s: "
                  << v->size << " m: " << v->sum
                  << " " << (v->left != nullptr)
                  << " " << (v->right != nullptr) << std::endl << std::endl;
    }

    int_t get_sum_at(int_t pos) {

        auto splitted1 = implicit_split(root_, pos - 1);
        update(splitted1.first);
        update(splitted1.second);

        auto splitted2 = implicit_split(splitted1.second, 1);
        update(splitted2.first);
        update(splitted2.second);

        int_t sum = splitted2.first->sum;

        return sum;
    }

    int_t get_sum_range(int_t pos1, int_t pos2) {
        int_t adj_pos2 = pos2 - pos1 + 1;
        auto splitted1 = implicit_split(root_, pos1 - 1);

        auto splitted2 = implicit_split(splitted1.second, adj_pos2);
        int_t sum = splitted2.first->sum;

        root_ = merge(splitted1.first, merge(splitted2.first, splitted2.second));
        return sum;
    }

    void forward(int_t pos1, int_t pos2) {
        int_t adj_pos2 = pos2 - pos1 + 1;
        auto splitted1 = implicit_split(root_, pos1 - 1);

        auto splitted2 = implicit_split(splitted1.second, adj_pos2);

        root_ = merge(splitted2.first, merge(splitted1.first, splitted2.second));
    }

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
            update(root);
            update(splitted.second);

            return std::make_pair(root, splitted.second);
        } else {

            std::pair<node*, node*> splitted = implicit_split(root->left, pos);
            root->left = splitted.second;

            set_parent(splitted.second, root);
            set_parent(splitted.first, nullptr);

            push(root);
            push(splitted.first);
            update(root);
            update(splitted.first);

            return std::make_pair(splitted.first, root);
        }
    };

    std::pair<node*, node*> split(node *root, int key) {
        if (root == nullptr) {
            return std::make_pair<node*, node*>(nullptr, nullptr);
        }

        if (root->key < key) {
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

    void erase_rec(node*& root, int key) {
        assert(root != nullptr);

        if (key < root->key) {
            erase_rec(root->left, key);
        } else if (key > root->key) {
            erase_rec(root->right, key);
        } else {
            node *parent = root->parent;
            root = merge(root->left, root->right);
            set_parent(root, parent);
            update_up(parent);
        }
    }

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
            v->sum = v->key + get_sum(v->left) + get_sum(v->right);
        }
    }

    void update_down(node* v) {
        if (v == nullptr) return;
        if (v->left == nullptr && v->right == nullptr) {
            v->sum = v->key;
            v->size = 1;
            return;
        }

        v->size = get_size(v->left) + get_size(v->right) + 1;
        v->sum = v->key + get_sum(v->left) + get_sum(v->right);
    }

    void reverse_range(int_t pos1, int_t pos2) {
        int_t adj_pos2 = pos2 - pos1 + 1;

        auto splitted1 = implicit_split(root_, pos1 - 1);

//        traverse_rec(splitted1.first, "");
//        cout << endl << endl;

        auto splitted2 = implicit_split(splitted1.second, adj_pos2);

//        cout << "pushing:" << endl;
//        push(splitted2.first);
        splitted2.first->reversed = true;
//        traverse_rec(splitted2.first, "");
//        cout << endl << endl;



        root_ = merge(splitted1.first, merge(splitted2.first, splitted2.second));
    }

    void push(node* v) {
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
    }

    void propagate() {
        propagate_rec(root_);
    }

    void propagate_rec(node* v) {
        if (v == nullptr) return;
        if (v->reversed) {
            std::swap(v->left, v->right);
//            cout << "kek" << endl;
            v->reversed = false;
        }
        if (v->left != nullptr) {
            v->left->reversed ^= true;
            propagate_rec(v->left);
        }
        if (v->right != nullptr) {
            v->right->reversed ^= true;
            propagate_rec(v->right);
        }
    }

    node *root_;
};


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    freopen("input", "r", stdin);

    uint_t n = 0;
    uint_t m = 0;

    cin >> n;
    cin >> m;

    treap tr;

    uint_t height = 0;
    uint_t command = 0;
    uint_t pos1 = 0;
    uint_t pos2 = 0;

    for (uint_t i = 0; i < n; ++i ) {
        cin >> height;
        tr.insert_into(i, height);
    }

    for (uint_t i = 0; i < m; ++i) {
        cin >> command >> pos1 >> pos2;
        if (command == 0) {
//            tr.propagate();
            cout << tr.get_sum_range(pos1, pos2) << endl;
        }
        if (command == 1) {
//            tr.propagate();
            tr.reverse_range(pos1, pos2);
        }
    }

//    cout << tr.get_sum_range(5, 5) << endl << endl;
//    tr.traverse();
//    cout << endl << endl;
//    tr.reverse_range(1, 5);
//    tr.traverse();
//    tr.propagate();
//    tr.in_order();
//    cout << endl << endl;
//    tr.traverse();

    return 0;
}