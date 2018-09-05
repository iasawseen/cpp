//
// Created by ivan on 17.04.17.
//

#ifndef GRAPH_SHORTEST_PATH_H
#define GRAPH_SHORTEST_PATH_H

#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <list>
#include <set>

namespace au {

template<class iterator>
struct hash_iter {
    using value_type = typename iterator::value_type;
    struct hash {
        size_t operator() (iterator const& it) const {
            return std::hash<value_type>()(*it);
        }
    };

    struct compare {
        bool operator() (iterator const& lhs,
                         iterator const& rhs) const {
            return *lhs == *rhs;
        }
    };
};


template<class graph, class edge_len, class path_visitor>

bool find_shortest_path(graph const& graph_,
                        typename graph::vertex_const_iterator from,
                        typename graph::vertex_const_iterator to,
                        edge_len && len_functor,
                        path_visitor&& visitor) {

    using vertex_iterator = typename graph::vertex_const_iterator;
    using edge_iterator = typename graph::edge_const_iterator;
    using distance = double;
    using pair = std::pair<distance, vertex_iterator>;
    distance infinity = std::numeric_limits<distance>::max();

    auto compare = [](pair const& lhs, pair const& rhs) { return lhs.first < rhs.first;};

    std::multiset<pair, decltype(compare)> priority_queue(compare);

    std::unordered_map<vertex_iterator, distance,
            typename hash_iter<vertex_iterator>::hash,
            typename hash_iter<vertex_iterator>::compare> distances;

    std::unordered_map<vertex_iterator, vertex_iterator,
            typename hash_iter<vertex_iterator>::hash,
            typename hash_iter<vertex_iterator>::compare> prevs;

    if (from == graph_.vertex_end() || to == graph_.vertex_end()) {
        return false;
    }

    for (auto it = graph_.vertex_begin(); it != graph_.vertex_end(); ++it) {
        distances[it] = infinity;
    }

    priority_queue.insert(std::make_pair(0, from));
    distances[from] = 0;

    while (!priority_queue.empty()) {
        pair cur_pair = *(priority_queue.begin());
        priority_queue.erase(priority_queue.begin());

        vertex_iterator from_v = cur_pair.second;

        for (auto cur_edge = graph_.edge_begin(from_v); cur_edge != graph_.edge_end(from_v); ++cur_edge) {
            vertex_iterator to_v = cur_edge.to();

            if (distances[to_v] > (distances[from_v] + len_functor(*cur_edge))) {
                if (distances[to_v] != infinity) {
                    priority_queue.erase(priority_queue.find(std::make_pair(distances[to_v], to_v)));
                }
                distances[to_v] = distances[from_v] + len_functor(*cur_edge);
                priority_queue.insert(std::make_pair(distances[to_v], to_v));
                prevs[to_v] = from_v;
            }
        }
    }

    if (distances[to] == infinity) {
        return false;
    }

    vertex_iterator end = to;
    vertex_iterator start = from;

    std::list<edge_iterator> path;

    while (end != start) {
        vertex_iterator prev = prevs[end];
        auto edge = graph_.find_edge(prev, end);

        path.push_front(edge);
        end = prev;
    }

    for (edge_iterator& edge : path) {
        visitor(edge);
    }

    return true;
};

} // au
#endif //GRAPH_SHORTEST_PATH_H
