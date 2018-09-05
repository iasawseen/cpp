//
// Created by ivan on 24.03.17.
//

#ifndef MUM_SUFFIX_HPP
#define MUM_SUFFIX_HPP

#endif //MUM_SUFFIX_HPP

#include <iostream>
#include <vector>
#include <map>

class Suffix_tree {

    using map_t = std::map<char, uint64_t>;
    using vec_t = std::vector<uint64_t>;
    using maps = std::vector<map_t>;
    using str_vec = std::vector<std::string>;

public:
    Suffix_tree(std::string const& genomes, uint64_t genome_one) {
        genomes_ = genomes;
        maxlen_ = genomes.size() * 3;
        size_1 = genome_one;

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

        for (int64_t i = genomes_.size() - 1; i >= 0; i--) {
            extend(i);
        }
    }

    void attach(uint64_t child, uint64_t parent, char c, uint64_t child_len)
    {
        to_[parent][c] = child;
        len_[child] = child_len;
        parent_[child] = parent;
    }

    void extend(uint64_t i)
    {
        //в нашей реализации old – это последняя вершина
        uint64_t v = 0;
        uint64_t vlen = genomes_.size() - i;
        uint64_t old = size - 1;
        std::vector<uint64_t> path;
        uint64_t pst = 0;

        for (v = old; !link_[v].count(genomes_[i]); v = parent_[v]) {
            vlen -= len_[v];
            path.push_back(v);
        }

        uint64_t w = link_[v][genomes_[i]];

        if (to_[w].count(genomes_[i + vlen])) { //если w != w’

            uint64_t u = to_[w][genomes_[i + vlen]];
            //sz – это новая вершина w’, которую мы создаём
            //т.к. w!=w', условие s[pos[u] - len[u]] == s[i + vlen] на первой итерации цикла истинно

            for (pos_[size] = pos_[u] - len_[u]; genomes_[pos_[size]] == genomes_[i + vlen]; pos_[size] += len_[v]) {
                //очередной кандидат на v’
                v = path.back();
                path.pop_back();

                vlen += len_[v];
            }
            //присоединить w'(=sz) к w
            attach(size, w, genomes_[pos_[u] - len_[u]], len_[u] - (pos_[u] - pos_[size]));

            //присоединить u к w'(=sz)
            attach(u, size, genomes_[pos_[size]], pos_[u] - pos_[size]);

            //провести префиксную ссылку из v' в w'; установить w = w' для вставки нового листа
            w = link_[v][genomes_[i]] = size++;

        } //в этой точке переменная w содержит вершину w'

        //префиксная ссылка из старого листа к новому
        link_[old][genomes_[i]] = size;
        //присоединить новый лист sz к вершине w'
        attach(size, w, genomes_[i + vlen], genomes_.size() - (i + vlen));
        //pos для листьев всегда равен n
        pos_[size++] = genomes_.size();
    }

    uint64_t how_many_children(uint64_t vertex) {
        return to_[vertex].size();
    }

    int get_genome_label(uint64_t vertex) {
        uint64_t pos = pos_[vertex] - len_[vertex];

        if (pos < size_1) {
            return 0;
        }
        if (pos > size_1) {
            return 1;
        }
    }

    bool has_diff_terminals(uint64_t vertex) {
        uint64_t first;
        uint64_t second;

        auto it = to_[vertex].begin();
        first = it->second;
        ++it;
        second = it->second;

        if (how_many_children(first) != 0 || how_many_children(second) != 0) {
            return false;
        }

        int first_label = get_genome_label(first);
        int second_label = get_genome_label(second);

        // проверяем, чтобы лейблы были разные
        return (first_label ^ second_label) == 1;
    }

    uint64_t get_dist_to_root(uint64_t vertex) {
        uint64_t dist = 0;
        while (parent_[vertex] != 0) {
            dist += len_[vertex];
            vertex = parent_[vertex];
        }
        return dist;
    }

    void traverse_tree() {
        uint64_t max_len = 0;
        uint64_t max_vertex = 0;

        for (uint64_t i = 0; i <= size; ++i) {
            if ((how_many_children(i) == 2) && (has_diff_terminals(i))) {
                if (get_dist_to_root(i) > max_len) {
                    max_len = get_dist_to_root(i);
                    max_vertex = i;
                }
            }
        }
        uint64_t dist_to_root = get_dist_to_root(max_vertex);

        std::cout << genomes_.substr(pos_[max_vertex] - dist_to_root, dist_to_root) << std::endl;
    }
    uint64_t maxlen_;
    std::string genomes_;
    vec_t pos_;
    vec_t len_;
    vec_t parent_;
    maps to_;
    maps link_;
    uint64_t size;

    uint64_t size_1;
};