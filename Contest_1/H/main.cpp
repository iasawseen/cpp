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
             reversed_(false)
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

    node* parent_;
    node* left_;
    node* right_;
    node* path_parent_;
    bool reversed_;
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
    }

    bool connected(uint_t first, uint_t second) {
        node* first_upd = access(first)->first();
        node* second_upd = access(second)->first();
        return first_upd == second_upd;
    }

    void make_root(uint_t vertex) {
        access(vertex);
        nodes[vertex].splay();

        if(nodes[vertex].left_ != nullptr) {
            nodes[vertex].left_->parent_ = nullptr;
            nodes[vertex].left_->reversed_ ^= true;
            nodes[vertex].left_->path_parent_ = &nodes[vertex];
            nodes[vertex].left_ = nullptr;
            nodes[vertex].keep_parent();
        }
    }

    node* access(uint_t vertex) {
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
        }
        return x;

    }

    std::vector<node> nodes;
};


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
            cout << forest.connected(first - 1, second - 1);
        }
    }

    return 0;
}