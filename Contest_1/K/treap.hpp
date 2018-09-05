/*
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <numeric>
//#include "treap.hpp"

#include <cstdlib>
#include <utility>
#include <cassert>
#include <iostream>

template<class T>
class treap {
    using int_t = uint64_t;
    struct node {
        T key;
        int_t priority;
        int_t size;

        node *parent;
        node *left;
        node *right;

        node(T key)
                : key(key), priority(std::rand()), size(0),
                  parent(nullptr), left(nullptr), right(nullptr)
        {}

        node(T key, int_t priority)
                : node(key)
        {
            this->priority = priority;
        }

        std::string to_string() {
//            return std::to_string(key) + " " + std::to_string(size);
            return key + " " + std::to_string(priority);
        }

        int_t get_priority() {
            return priority;
        }
    };

public:

    treap() : root_(nullptr)
    {}

    void insert(T key) {
        insert_rec(root_, key);
    }

    void insert(T key, int_t priority) {
        insert_rec(root_, key, priority);
    }

    bool search(T key) {
        return search_rec(root_, key);
    }

    node* get(T key) {
        return get_rec(root_, key);
    }

    void erase(T key) {
        erase_rec(root_, key);
    }

    int_t nth_element(int_t n) {
        return nth_element(root_, n);
    }

    void traverse() {
        traverse_rec(root_, "");
    }

    int_t lower_bound(T key) {
        if (root_ == nullptr) {
            return -1;
        }
        return lower_bound_rec(root_, key);
    }

    int_t num_less_or_equal(T key) {
        return num_less_or_equal_rec(root_, key);
    }

    int_t get_head_priority() {
        return root_->priority;
    }

    std::string get_head_key() {
        return root_->key;
    }

private:

    int_t num_less_or_equal_rec(node *v, T key) {
        return -1;
    }


    int_t lower_bound_rec(node *v, T key) {
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

    void traverse_rec(node *v, std::string pad) {
        if (v != nullptr) {
            std::cout << pad << v->to_string() << std::endl;
            std::cout << pad + "left" << std::endl;
            traverse_rec(v->left, pad + "  ");
            std::cout << pad + "right" << std::endl;
            traverse_rec(v->right, pad + "  ");
        }
    }

    int_t nth_element(node *v, int_t n) {
        assert(v != nullptr);

        int_t root_number = get_size(v->left);

        if (root_number == n)
            return v->key;
        if (n < root_number)
            return nth_element(v->left, n);
        else
            return nth_element(v->right, n - root_number - 1);
    }

    node* get_rec(node *v, T key) {
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

    bool search_rec(node *v, T key)
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

    std::pair<node*, node*> split(node *root, T key) {
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

    node* merge(node *left, node *right)
    {
        if (left == nullptr || right == nullptr) {
            return right == nullptr ? left : right;
        } if (left->priority > right->priority) {
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

    void insert_rec(node *&root, T key, int_t priority = rand())
    {
        std::pair<node*, node*> splitted = split(root, key);
        root = merge(merge(splitted.first, new node(key, priority)), splitted.second);
    }

    void insert_rec_2(node *&root, node *vertex) {
        if (root == nullptr) {
            root = vertex;
            return;
        }
        if (root->priority > vertex->priority) {
            if (root->key > vertex->key) {
                insert_rec_2(root->left, vertex);
            } else {
                insert_rec_2(root->right, vertex);
            }
            return;
        }
    }

    void erase_rec(node *&root, T key)
    {
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

    int_t get_size(node *v)
    {
        return v == nullptr ? 0 : v->size;
    }

    void set_parent(node *v, node *parent) {
        if (v != nullptr) {
            v->parent = parent;
        }
    }

    void update_up(node *v) {
        if (v == nullptr) return;

        v->size = get_size(v->left) + get_size(v->right) + 1;

        while (v->parent != nullptr) {
            v = v->parent;
            v->size = get_size(v->left) + get_size(v->right) + 1;
        }
    }

    void update_down(node *v)
    {
        if (v == nullptr) return;
        v->size = get_size(v->left) + get_size(v->right) + 1;
    }

    node *root_;
};


using namespace std;

void test_treap() {
    std::vector<int> v(10, 0);
    std::iota(v.begin(), v.end(), 0);
    treap<int> tr;

    srand(1);

    for (auto& item : v) {
        tr.insert(item, item);
    }

//    tr.traverse();

    for (auto& item : v) {
        assert(tr.search(item));
        assert(tr.nth_element(item) == item);
    }

    tr.erase(5);
    assert(!tr.search(5));


}

int main() {
    using int_t = uint64_t;

    test_treap();

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    std::map<string, string> person_to_city;
    std::map<string, int_t> person_fortune;

    freopen("input", "r", stdin);

    int_t n = 0;
    cin >> n;
    string name;
    string city;
    int_t fortune = 0;

    treap<string> city_to_money;

    for (size_t i = 0; i < n; ++i) {
        cin >> name;
        cin >> city;
        cin >> fortune;

//        cout << name << " " << city << " " << fortune << endl;

        person_to_city.insert(make_pair(name, city));
        person_fortune.insert(make_pair(name, fortune));

        auto city_node = city_to_money.get(city);

        if (city_node != nullptr) {
            fortune += city_node->get_priority();
            city_to_money.erase(city);
        }
        city_to_money.insert(city, fortune);
    }

//    city_to_money.traverse();

    int_t days = 0;
    int_t days_with_info = 0;

    std::map<string, int_t> richests;

    cin >> days;
    cin >> days_with_info;
    int day = -1;


    cin >> day;
    cin >> name;
    cin >> city;
    int_t counter = 1;

    for (int i = 1; i < days + 1; ++i) {
        if (i == day) {

            while (i == day && counter <= days_with_info) {
                string old_city = person_to_city[name];
                auto node = city_to_money.get(old_city);
                int_t old_money = node->get_priority();
                city_to_money.erase(old_city);
                city_to_money.insert(old_city, old_money - person_fortune[name]);

                node = city_to_money.get(city);
                if (node != nullptr) {
                    old_money = node->get_priority();
                    city_to_money.erase(city);
                } else {
                    old_money = 0;
                }

                city_to_money.insert(city, old_money + person_fortune[name]);

                cin >> day;
                cin >> name;
                cin >> city;
                ++counter;
            }
        }

        string richest_city = city_to_money.get_head_key();
        int_t times = 0;

        auto founded = richests.find(richest_city);

        if (founded != richests.end()) {
            times = founded->second + 1;
//            richests.erase(founded);
        }

        cout << i << " " << richest_city << " " << times << endl;
        richests[richest_city] = times;
    }

    for(auto& rich : richests) {
        cout << rich.first << " " << rich.second << endl;
    }
}*/
