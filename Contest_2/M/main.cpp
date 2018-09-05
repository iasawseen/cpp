#define TESTS

#ifdef TESTS
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#endif

#include <iostream>
#include <vector>

using namespace std;
using uint_t = uint64_t;
using int_t = int64_t;

struct node {

    enum direction {
        left,
        right,
        root
    };

    node() : parent_(nullptr),
             left_(nullptr),
             right_(nullptr),
             path_parent_(nullptr),
             reversed_(false),
             size_(1)
    {
        keep_parent();
    }

    void keep_parent() {
        if (left_ != nullptr) {
            left_->parent_ = this;
        }
        if (right_ != nullptr) {
            right_->parent_ = this;
        }
    }

    void push() {
        if (!reversed_) {
            return;
        }
        if (left_ != nullptr) {
            left_->reversed_ ^= true;
        }
        if (right_ != nullptr) {
            right_->reversed_ ^= true;
        }
        std::swap(left_, right_);
        reversed_ = false;
    }

    direction get_up_dir() {
        if (parent_ == nullptr) {
            return direction::root;
        }
        if (parent_->left_ == this) {
            return direction::left;
        } else {
            return direction::right;
        }
    }

    node* get_child(direction dir) {
        if (dir == direction::left) {
            return left_;
        } else {
            return right_;
        }
    }

    direction opp_dir(direction dir) {
        if (dir == direction::right) {
            return direction::left;
        } else {
            return direction::right;
        }
    }

    void set_child(direction dir, node* new_node) {
        if (dir == direction::left) {
            left_ = new_node;
        } else {
            right_ = new_node;
        }
    }

    void zig(direction dir) {
        node* child = get_child(dir);

        if (child->parent_ = parent_) {
            parent_->set_child(get_up_dir(), child);
        }

        set_child(dir, child->get_child(opp_dir(dir)));
        child->set_child(opp_dir(dir), this);

        keep_parent();
        child->keep_parent();
        if (parent_ != nullptr) {
            parent_->keep_parent();
        }

        std::swap(path_parent_, child->path_parent_);

        light_update();
        child->light_update();
    }

    void zig_zig(direction dir) {
        node* child = get_child(dir);
        node* grand_child = child->get_child(dir);

        if (grand_child->parent_ = parent_) {
            parent_->set_child(get_up_dir(), grand_child);
        }

        set_child(dir, child->get_child(opp_dir(dir)));

        child->set_child(dir, grand_child->get_child(opp_dir(dir)));
        grand_child->set_child(opp_dir(dir), child);
        child->set_child(opp_dir(dir), this);

        keep_parent();
        child->keep_parent();
        grand_child->keep_parent();
        if (parent_ != nullptr) {
            parent_->keep_parent();
        }

        std::swap(path_parent_, grand_child->path_parent_);

        light_update();
        child->light_update();
        grand_child->light_update();
    }

    void zig_zag(direction dir) {
        node* child = get_child(dir);
        node* grand_child = child->get_child(opp_dir(dir));

        if (grand_child->parent_ = parent_) {
            parent_->set_child(get_up_dir(), grand_child);
        }

        set_child(dir, grand_child->get_child(opp_dir(dir)));

        child->set_child(opp_dir(dir), grand_child->get_child(dir));
        grand_child->set_child(opp_dir(dir), this);
        grand_child->set_child(dir, child);

        keep_parent();
        child->keep_parent();
        grand_child->keep_parent();
        if (parent_ != nullptr) {
            parent_->keep_parent();
        }

        std::swap(path_parent_, grand_child->path_parent_);

        light_update();
        child->light_update();
        grand_child->light_update();
    }

    node* splay() {
        push();
        while (parent_ != nullptr) {
            if (parent_->parent_ != nullptr) {
                parent_->parent_->push();
            }
            parent_->push();
            push();

            direction dir1 = get_up_dir();
            direction dir2 = parent_->get_up_dir();

            if (dir2 == direction::root) {
                parent_->zig(dir1);
            } else if (dir1 == dir2) {
                parent_->parent_->zig_zig(dir1);
            } else {
                parent_->parent_->zig_zag(dir2);
            }
        }
        light_update();
        return this;
    }

    node* last() {
        push();
        if (right_ != nullptr) {
            return right_->last();
        } else {
            return splay();
        }
    }

    node* first() {
        push();
        if (left_ != nullptr) {
            return left_->first();
        } else {
            return splay();
        }
    }


    void light_update() {
        size_ = 1;

        if (left_ != nullptr) {
            size_ += left_->size_;
        }
        if (right_ != nullptr) {
            size_ += right_->size_;
        }
    }


    void in_order() {
        if (left_ != nullptr) {
            left_->in_order();
        }
        cout << size_ << " - ";
        if (right_ != nullptr) {
            right_->in_order();
        }
        cout << endl;
    }

    node* parent_;
    node* left_;
    node* right_;
    node* path_parent_;
    bool reversed_;
    uint_t size_;
};


struct link_cut {
    link_cut(uint_t n)
            : nodes(n) {}

    void link(uint_t first, uint_t second) {
        make_root(second);
        nodes[second].path_parent_ = &nodes[first];
    }

    void cut(uint_t first, uint_t second) {
        make_root(first);
        nodes[second].splay();

        if (nodes[second].path_parent_ != nullptr) {
            nodes[second].path_parent_ = nullptr;
        } else {
            nodes[second].left_->parent_ = nullptr;
            nodes[second].left_ = nullptr;
            nodes[second].keep_parent();
        }
        nodes[second].light_update();
    }

    bool connected(uint_t first, uint_t second) {
        node* first_upd = expose(first)->first();
        node* second_upd = expose(second)->first();
        return first_upd == second_upd;
    }

    void make_root(uint_t vertex) {
        expose(vertex);
        nodes[vertex].splay();

        if(nodes[vertex].left_ != nullptr) {
            nodes[vertex].left_->parent_ = nullptr;
            nodes[vertex].left_->reversed_ ^= true;
            nodes[vertex].left_->path_parent_ = &nodes[vertex];
            nodes[vertex].left_ = nullptr;
            nodes[vertex].keep_parent();
        }

        nodes[vertex].light_update();
    }

    node* expose(uint_t vertex) {
        node* x = nodes[vertex].splay();
        node* path_parent = nullptr;

        while (x->path_parent_ != nullptr) {
            path_parent = x->path_parent_->splay();
            x->path_parent_ = nullptr;
            if (path_parent->right_ != nullptr) {
                path_parent->right_->parent_ = nullptr;
                path_parent->right_->path_parent_ = path_parent;
            }
            path_parent->right_ = x;
            path_parent->keep_parent();
            x = path_parent;

            x->light_update();
        }

        x->light_update();
        x->splay();
        return x;
    }

    int_t get_distance(uint_t first, uint_t second) {
        if (!connected(first, second)) {
            return -1;
        }

        node* node1 = &nodes[first];
        node* node2 = &nodes[second];

        make_root(first);
        expose(second);

        return depth(second);

    }

    int_t depth(uint_t v) {
        node* vertex = expose(v);

        if (vertex->left_ != nullptr) {
            return vertex->left_->size_;
        }
        return 0;
    }

    std::vector<node> nodes;
};


#define TEST_UNIT1
#define TEST_UNIT2
#define TEST_UNIT3
#define TEST_UNIT4
#define TEST_UNIT5

#ifdef TESTS

TEST_CASE("Unit_1") {
#ifdef TEST_UNIT1
    link_cut forest(3);
    CHECK(forest.get_distance(1 - 1, 2 - 1) == -1);
    forest.link(1 - 1, 2 - 1);
    CHECK(forest.get_distance(1 - 1, 2 - 1) == 1);
    forest.cut(1 - 1, 2 - 1);
    CHECK(forest.get_distance(1 - 1, 2 - 1) == -1);
    forest.link(1 - 1, 2 - 1);
    CHECK(forest.get_distance(1 - 1, 2 - 1) == 1);
#endif // TEST_UNIT1
}

TEST_CASE("Unit_2") {
#ifdef TEST_UNIT2
    link_cut forest(5);
    forest.link(1 - 1, 2 - 1);
    forest.link(2 - 1, 3 - 1);
    forest.link(4 - 1, 3 - 1);
    forest.cut(3 - 1, 4 - 1);
    CHECK(forest.get_distance(1 - 1, 2 - 1) == 1);
    CHECK(forest.get_distance(1 - 1, 3 - 1) == 2);
    CHECK(forest.get_distance(1 - 1, 4 - 1) == -1);
    CHECK(forest.get_distance(2 - 1, 3 - 1) == 1);
    CHECK(forest.get_distance(2 - 1, 4 - 1) == -1);
    CHECK(forest.get_distance(3 - 1, 4 - 1) == -1);
#endif // TEST_UNIT2
}

TEST_CASE("Unit_3") {
#ifdef TEST_UNIT3
    link_cut forest(5);
    forest.link(1 - 1, 2 - 1);
    forest.link(2 - 1, 3 - 1);
    forest.link(4 - 1, 3 - 1);
    forest.link(5 - 1, 4 - 1);
    CHECK(forest.get_distance(5 - 1, 1 - 1) == 4);
    CHECK(forest.get_distance(5 - 1, 4 - 1) == 1);
    CHECK(forest.get_distance(4 - 1, 5 - 1) == 1);
    CHECK(forest.get_distance(5 - 1, 3 - 1) == 2);
#endif // TEST_UNIT3
}

TEST_CASE("Unit_4") {
#ifdef TEST_UNIT4
    link_cut forest(5);
    forest.link(1 - 1, 2 - 1);
    forest.link(2 - 1, 3 - 1);
    forest.link(4 - 1, 2 - 1);

    forest.link(5 - 1, 4 - 1);
    CHECK(forest.get_distance(5 - 1, 1 - 1) == 3);
    CHECK(forest.get_distance(5 - 1, 4 - 1) == 1);
    CHECK(forest.get_distance(4 - 1, 5 - 1) == 1);
    CHECK(forest.get_distance(5 - 1, 3 - 1) == 3);
    CHECK(forest.get_distance(4 - 1, 1 - 1) == 2);
    CHECK(forest.get_distance(1 - 1, 3 - 1) == 2);
    CHECK(forest.get_distance(3 - 1, 1 - 1) == 2);
    CHECK(forest.get_distance(3 - 1, 1 - 1) == 2);

#endif // TEST_UNIT4
}

TEST_CASE("Unit_5") {
#ifdef TEST_UNIT5
    // star graph
    link_cut forest(7);
    forest.link(1 - 1, 2 - 1);
    forest.link(1 - 1, 3 - 1);
    forest.link(1 - 1, 4 - 1);
    forest.link(1 - 1, 5 - 1);
    forest.link(1 - 1, 6 - 1);

    CHECK(forest.get_distance(5 - 1, 1 - 1) == 1);
    CHECK(forest.get_distance(1 - 1, 5 - 1) == 1);
    CHECK(forest.get_distance(4 - 1, 1 - 1) == 1);
    CHECK(forest.get_distance(1 - 1, 4 - 1) == 1);
    CHECK(forest.get_distance(3 - 1, 1 - 1) == 1);
    CHECK(forest.get_distance(1 - 1, 3 - 1) == 1);
    CHECK(forest.get_distance(2 - 1, 1 - 1) == 1);
    CHECK(forest.get_distance(1 - 1, 2 - 1) == 1);
    CHECK(forest.get_distance(1 - 1, 1 - 1) == 0);
#endif // TEST_UNIT5
}

#endif // TESTS


/*
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

//    freopen("input", "r", stdin);

    uint_t n = 0;
    uint_t m = 0;

    cin >> n;
    cin >> m;

    link_cut forest(n);

    string command = "";
    uint_t first = 0;
    uint_t second = 0;

    for (uint_t i = 0; i < m; ++i) {
        cin >> command >> first >> second;

        if (command == "link") {
            forest.link(first - 1, second - 1);
        }
        if (command == "cut") {
            forest.cut(first - 1, second - 1);
        }
        if (command == "get") {
            cout << forest.get_distance(first - 1, second - 1) << endl;
        }
    }

    return 0;
}
*/
