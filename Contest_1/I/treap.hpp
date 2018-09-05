//
// Created by ivan on 28.03.17.
//

#include <cstdlib>
#include <utility>
#include <cassert>

#ifndef I_TREAP_HPP
#define I_TREAP_HPP

#endif //I_TREAP_HPP

class treap {
    struct node {
        int key;
        int priority;
        int size;

        node *parent;
        node *left;
        node *right;

        node(int key)
                : key(key), priority(std::rand()), size(0),
                  parent(nullptr), left(nullptr), right(nullptr)
        {}

        std::string to_string() {
            return std::to_string(key) + " " + std::to_string(size);
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

private:

    int num_less_or_equal_rec(node *v, int key) {
        return -1;
    }


    int lower_bound_rec(node *v, int key) {
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

    int nth_element(node *v, int n) {
        assert(v != nullptr);

        int root_number = get_size(v->left);

        if (root_number == n)
            return v->key;
        if (n < root_number)
            return nth_element(v->left, n);
        else
            return nth_element(v->right, n - root_number - 1);
    }

    node* get_rec(node *v, int key) {
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

    bool search_rec(node *v, int key)
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

    void insert_rec(node *&root, int key)
    {
        std::pair<node*, node*> splitted = split(root, key);
        root = merge(merge(splitted.first, new node(key)), splitted.second);
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

    void erase_rec(node *&root, int key)
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

    int get_size(node *v)
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
