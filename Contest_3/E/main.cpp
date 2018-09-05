#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

//#define TEST

using namespace std;

using int_t = int64_t;
using uint_t = uint64_t;

struct suffix_tree {

    using map_t = std::map<char, uint_t>;
    using vec_t = std::vector<uint_t>;
    using maps = std::vector<map_t>;
    using str_vec = std::vector<std::string>;

    suffix_tree(std::string const& strings, uint64_t sizik) {
        strings_ = strings;
        maxlen_ = strings.size() * 3;
        size_1 = sizik;

        size = 2;
        pos_ = vec_t(maxlen_, 0);
        len_ = vec_t(maxlen_, 0);
        parent_ = vec_t(maxlen_, 0);
        to_ = maps(maxlen_);
        link_ = maps(maxlen_);
        pos_.at(1) = 0;
        len_.at(1) = 1;
        parent_.at(1) = 0;

        for (int c = 0; c < 256; c++)
            link_[0][c] = 1;
    }

    void build_tree() {
        for (int_t i = strings_.size() - 1; i >= 0; i--) {
            extend(i);
        }
    }

    void attach(uint_t child, uint_t parent, char c, uint_t child_len)
    {
        to_[parent][c] = child;
        len_[child] = child_len;
        parent_[child] = parent;
    }

    void extend(uint_t i)
    {
        uint_t v = 0;
        uint_t vlen = strings_.size() - i;
        uint_t old = size - 1;
        std::vector<uint_t> path;
        uint_t pst = 0;

        for (v = old; !link_[v].count(strings_[i]); v = parent_[v]) {
            vlen -= len_[v];
            path.push_back(v);
        }

        uint_t w = link_[v][strings_[i]];

        if (to_[w].count(strings_[i + vlen])) {

            uint_t u = to_[w][strings_[i + vlen]];

            for (pos_[size] = pos_[u] - len_[u]; strings_[pos_[size]] == strings_[i + vlen]; pos_[size] += len_[v]) {
                v = path.back();
                path.pop_back();
                vlen += len_[v];
            }

            attach(size, w, strings_[pos_[u] - len_[u]], len_[u] - (pos_[u] - pos_[size]));
            attach(u, size, strings_[pos_[size]], pos_[u] - pos_[size]);
            w = link_[v][strings_[i]] = size++;

        }

        link_[old][strings_[i]] = size;
        attach(size, w, strings_[i + vlen], strings_.size() - (i + vlen));
        pos_[size++] = strings_.size();
    }

    uint_t how_many_children(uint_t vertex) {
        return to_[vertex].size();
    }

    int get_genome_label(uint_t vertex) {
        uint_t pos = pos_[vertex] - len_[vertex];

        if (pos < size_1) {
            return 0;
        }
        if (pos > size_1) {
            return 1;
        }
    }

    bool has_diff_terminals(uint_t vertex) {
        uint_t first;
        uint_t second;

        auto it = to_[vertex].begin();
        first = it->second;
        ++it;
        second = it->second;

        if (how_many_children(first) != 0 || how_many_children(second) != 0) {
            return false;
        }

        int first_label = get_genome_label(first);
        int second_label = get_genome_label(second);

        return first_label != second_label;
    }

    uint_t get_dist_to_root(uint_t vertex) {
        uint_t dist = 0;
        while (parent_[vertex] != 0) {
            dist += len_[vertex];
            vertex = parent_[vertex];
        }
        return dist;
    }

    void traverse_tree() {
        uint_t max_len = 0;
        uint_t max_vertex = 0;

        for (uint_t i = 0; i <= size; ++i) {
            if ((how_many_children(i) >= 2) && (has_diff_terminals(i))) {
                if (get_dist_to_root(i) > max_len) {
                    max_len = get_dist_to_root(i);
                    max_vertex = i;
                }
            }
        }
        uint_t dist_to_root = get_dist_to_root(max_vertex);

        std::cout << strings_.substr(pos_[max_vertex] - dist_to_root, dist_to_root) << std::endl;
    }

private:
    uint_t maxlen_;
    std::string strings_;
    vec_t pos_;
    vec_t len_;
    vec_t parent_;
    maps to_;
    maps link_;
    uint_t size;
    uint_t size_1;
};

struct node {
    int_t link_;
    int_t length_;
    std::map<char, int_t> next_;
};

void extend(std::vector<node>& nodes, char ch, int_t& size, int_t& last_node) {
    int_t current = size;

    ++size;

    nodes[current].length_ = nodes[last_node].length_ + 1;
    int_t node1;

    for (node1 = last_node;
         node1 != -1 && !nodes[node1].next_.count(ch);
         node1 = nodes[node1].link_) {

        nodes[node1].next_[ch] = current;

    }

    if (node1 == -1) {
        nodes[current].link_ = 0;
    } else {
        int_t node2 = nodes[node1].next_[ch];

        if ((nodes[node1].length_ + 1) == nodes[node2].length_) {
            nodes[current].link_ = node2;
        } else {

            int_t dubl = size;
            ++size;

            nodes[dubl].length_ = nodes[node1].length_ + 1;
            nodes[dubl].next_ = nodes[node2].next_;
            nodes[dubl].link_ = nodes[node2].link_;

            while (node1 != -1 && nodes[node1].next_[ch] == node2) {
                nodes[node1].next_[ch] = dubl;
                node1 = nodes[node1].link_;
            }

            nodes[node2].link_ = nodes[current].link_ = dubl;
        }
    }

    last_node = current;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    std::string str1;
    std::string str2;
    size_t size;

    cin >> size;
    cin >> str1 >> str2;

    std::vector<node> nodes(size * 2);
    int_t size1 = 0;
    int_t last_node = 0;
    nodes[0].length_ = 0;
    nodes[0].link_ = -1;
    ++size1;

    for(size_t i = 0; i < str1.size(); ++i) {
        extend(nodes, str1[i], size1, last_node);
    }

    int_t cur = 0;
    size_t cur_len = 0;
    size_t max_len = 0;
    size_t best_pos = 0;

    for (size_t i = 0; i < str2.size(); ++i) {
        while (cur != 0 && nodes[cur].next_.count(str2[i]) == 0) {
            cur = nodes[cur].link_;
            cur_len = nodes[cur].length_;
        }
        if (nodes[cur].next_.count(str2[i])) {
            cur = nodes[cur].next_[str2[i]];
            ++cur_len;
        }
        if (cur_len > max_len) {
            max_len = cur_len;
            best_pos = i;
        }
    }
    cout << str2.substr(best_pos - max_len + 1, max_len);

    return 0;
}